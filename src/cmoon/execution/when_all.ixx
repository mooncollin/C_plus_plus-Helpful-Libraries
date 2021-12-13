export module cmoon.execution.when_all;

import <utility>;
import <type_traits>;
import <tuple>;
import <optional>;
import <functional>;
import <variant>;
import <mutex>;
import <concepts>;

import cmoon.meta;
import cmoon.concepts;
import cmoon.thread;
import cmoon.functional;

import cmoon.execution.receiver;
import cmoon.execution.connect;
import cmoon.execution.sender;
import cmoon.execution.typed_sender;
import cmoon.execution.sender_traits;
import cmoon.execution.set_done;
import cmoon.execution.set_error;
import cmoon.execution.set_value;
import cmoon.execution.get_stop_token;
import cmoon.execution.into_variant;

namespace cmoon::execution
{
	template<class... Ts>
		requires(sizeof...(Ts) <= 1)
	using zero_or_one = void;

	template<class... Ts>
		requires(sizeof...(Ts) == 0)
	using zero = void;

	template<typed_sender... Si>
	struct value_types_generator : std::type_identity<cmoon::meta::type_list<>> {};

	template<typed_sender S1, typed_sender... Si>
		requires(value_types_of_t<S1, cmoon::meta::type_list, cmoon::meta::type_list>::size() != 0 &&
				 value_types_of_t<S1, cmoon::meta::type_list, cmoon::meta::type_list>::template type<0>::size() != 0 &&
				 ((value_types_of_t<Si, cmoon::meta::type_list, cmoon::meta::type_list>::size() != 0 &&
				   value_types_of_t<Si, cmoon::meta::type_list, cmoon::meta::type_list>::template type<0>::size() != 0) && ...))
	struct value_types_generator<S1, Si...> : std::type_identity<
		typename value_types_of_t<S1, cmoon::meta::type_list,
									  cmoon::meta::type_list>::template type<0>::template concatenate<
							typename value_types_generator<Si...>::type
						 >
	> {};

	template<template<class...> class Tuple, template<class...> class Variant, typed_sender... Si>
	using value_types_generator_t = Variant<typename value_types_generator<Si...>::type::template complete_type<Tuple>>;

	template<typename T>
	using augment_type = std::conditional_t<cmoon::specialization_of<std::remove_cvref_t<T>, std::reference_wrapper>,
											std::remove_cvref_t<T>,
											std::conditional_t<std::is_lvalue_reference_v<T>,
															   std::reference_wrapper<std::remove_reference_t<T>>,
															   std::optional<std::remove_cvref_t<T>>
															  >
											>;

	template<typename T>
	struct augment_type_s : std::type_identity<augment_type<T>> {};

	template<typed_sender... Ss>
	struct get_error_types : public std::type_identity<cmoon::meta::type_list<>> {};

	template<typed_sender S1, typed_sender... Ss>
	struct get_error_types<S1, Ss...> : public std::type_identity<
		typename sender_traits<S1>::template error_types<cmoon::meta::type_list>::template concatenate<
			typename get_error_types<Ss...>::type
		>
	> {};

	template<typed_sender... Ss>
	using get_error_types_t = typename get_error_types<Ss...>::type;

	struct stop_callback_fn
	{
		cmoon::in_place_stop_source& stop_src_;

		void operator()() noexcept
		{
			stop_src_.request_stop();
		}
	};

	template<typed_sender... Ss>
	class when_all_sender
	{
		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = value_types_generator_t<Tuple, Variant, Ss...>;

			template<template<class...> class Variant>
			using error_types = get_error_types_t<Ss...>::template append<std::exception_ptr>::unique::template complete_type<Variant>;

			static constexpr bool sends_done {true};

			static constexpr auto receiver_count {sizeof...(Ss)};
			static constexpr auto no_synchronization {receiver_count == 1};
		private:
			template<receiver R, std::size_t... Ints>
			class op
			{

				template<std::size_t I>
				class single_receiver
				{
					template<class T>
					[[nodiscard]] static constexpr auto unwrap_type(T&& t)
					{
						if constexpr (cmoon::specialization_of<T, std::optional>)
						{
							return std::forward<T>(t).value();
						}
						else
						{
							return std::forward<T>(t);
						}
					}

					constexpr inline void do_operations_left()
					{
						if constexpr (!no_synchronization)
						{
							// This receiver is the last one to finish.
							// At this point, we may have gotten
							if (owner_.get().state.operations_left.fetch_sub(1) == 1)
							{
								owner_.get().callback.reset();
								if constexpr (op::has_tuple_storage)
								{
									std::apply([this](auto&&... arg) mutable {
										execution::set_value(std::move(owner_.get().output_receiver),
																unwrap_type(std::forward<decltype(arg)>(arg))...);
									}, std::move(owner_.get().state.value_storage));
								}
								else
								{
									execution::set_value(std::move(owner_.get().output_receiver));
								}
							}
						}
					}

					// Needed because MSVC friend free functions cannot
					// access private members from members of *this,
					// even when this class is a friend of those members
					constexpr inline void set_done_helper() noexcept
					{
						// If there is only one receiver, then
						// we do not need to synchronize anything
						if constexpr (no_synchronization)
						{
							owner_.get().callback.reset();
							execution::set_done(std::move(owner_.get().output_receiver));
						}
						else
						{
							if (owner_.get().state.operations_left.fetch_sub(1) == 1)
							{
								// This receiver is the last one to finish.
								// We could have been set to done before this.
								// We could have been set to error before this.

								owner_.get().callback.reset();
								if (owner_.get().state.err)
								{
									std::visit([this](auto&& err) {
										execution::set_error(
											std::move(owner_.get().output_receiver),
											std::forward<decltype(err)>(err)
										);
									}, owner_.get().state.err.value());
								}
								else
								{
									execution::set_done(std::move(owner_.get().output_receiver));
								}
							}
							else
							{
								// There are other receivers still processing,
								// so we need to save that we are done until all other
								// receivers are done operating
								std::call_once(owner_.get().state.flag,
											   [this] {
												   owner_.get().state.set_done_called = true;
												   owner_.get().stop_source.request_stop();
											   });
							}
						}
					}

					// Needed because MSVC friend free functions cannot
					// access private members from members of *this,
					// even when this class is a friend of those members
					template<class T>
					constexpr inline void set_value_helper(T&& t)
					{
						if constexpr (no_synchronization)
						{
							owner_.get().callback.reset();
							execution::set_value(
								std::move(owner_.get().output_receiver),
								std::forward<T>(t)
							);
						}
						else if constexpr (has_tuple_storage)
						{
							std::get<I>(owner_.get().state.value_storage) = std::forward<T>(t);
						}
					}

					constexpr inline void set_value_helper()
					{
						if constexpr (no_synchronization)
						{
							owner_.get().callback.reset();
							execution::set_value(
								std::move(owner_.get().output_receiver)
							);
						}
					}

					// Needed because MSVC friend free functions cannot
					// access private members from members of *this,
					// even when this class is a friend of those members
					template<class E>
					constexpr inline void set_error_helper(E&& e)
					{
						// If there is only one receiver, then
						// we do not need to synchronize anything
						if constexpr (no_synchronization)
						{
							owner_.get().callback.reset();
							execution::set_error(
								std::move(owner_.get().output_receiver),
								std::forward<E>(e)
							);
						}
						else
						{
							if (owner_.get().state.operations_left.fetch_sub(1) == 1)
							{
								// This receiver is the last one to finish.
								// We could have been set to done before this.
								// We could have been set to error before this.
								owner_.get().callback.reset();
								if (owner_.get().state.set_done_called)
								{
									execution::set_done(std::move(owner_.get().output_receiver));
								}
								else if (owner_.get().state.err)
								{
									std::visit([this](auto&& err) {
										execution::set_error(
											std::move(owner_.get().output_receiver),
											std::forward<decltype(err)>(err)
										);
									}, owner_.get().state.err.value());
								}
								else
								{
									execution::set_error(
										std::move(owner_.get().output_receiver),
										std::forward<E>(e)
									);
								}
							}
							else
							{
								// There are other receivers still processing,
								// so we need to save the error until all other
								// receivers are done operating
								std::call_once(owner_.get().state.flag,
											   [this](E&& e) {
												   owner_.get().state.err.emplace(std::forward<E>(e));
												   owner_.get().stop_source.request_stop();
											   },
											   std::forward<E>(e));
							}
						}
					}

					// Needed because MSVC friend free functions cannot
					// access private members from members of *this,
					// even when this class is a friend of those members
					inline auto get_stop_token_helper()
					{
						return owner_.get().stop_source.get_token();
					}

					public:
						constexpr single_receiver(op& owner) noexcept
							: owner_{owner} {}

						template<class T>
						constexpr friend void tag_invoke(set_value_t, single_receiver&& r, T&& t)
						{
							r.set_value_helper(std::forward<T>(t));
							r.do_operations_left();
						}

						constexpr friend void tag_invoke(set_value_t, single_receiver&& r)
						{
							r.set_value_helper();
							r.do_operations_left();
						}

						template<class E>
						constexpr friend void tag_invoke(set_error_t, single_receiver&& r, E&& e) noexcept
						{
							r.set_error_helper(std::forward<E>(e));
						}

						constexpr friend void tag_invoke(set_done_t, single_receiver&& r) noexcept
						{
							r.set_done_helper();
						}

						friend auto tag_invoke(get_stop_token_t, single_receiver&& r) noexcept
						{
							return r.get_stop_token_helper();
						}
					private:
						std::reference_wrapper<op> owner_;
				};

				public:
					constexpr op(R&& r, std::index_sequence<Ints...>, Ss&&... ss2)
						: output_receiver{std::forward<R>(r)},
						  child_op{execution::connect(std::forward<Ss>(ss2), single_receiver<Ints>{*this})...},
						  state{.operations_left = sizeof...(Ss)} {}

					constexpr op(R&& r, std::index_sequence<Ints...>, Ss&&... ss2) requires(no_synchronization)
						: output_receiver{std::forward<R>(r)},
						  child_op{execution::connect(std::forward<Ss>(ss2), single_receiver<Ints>{*this})...} {}

					constexpr friend void tag_invoke(start_t, op& o) noexcept
					{
						o.callback.emplace(execution::get_stop_token(o.output_receiver), stop_callback_fn{o.stop_source});

						if (o.stop_source.stop_requested())
						{
							execution::set_done(std::move(o.output_receiver));
						}

						std::apply([](auto&... ops) {
							(execution::start(ops), ...);
						}, o.child_op);
					}

				private:
					R output_receiver;
					cmoon::in_place_stop_source stop_source;
					std::tuple<connect_result_t<Ss, single_receiver<Ints>>...> child_op;
					std::optional<typename stop_token_of_t<R&>::template callback_type<stop_callback_fn>> callback;

					// We do not need these variables if there is no need for synchronization
					template<bool NoSync>
					struct state_t
					{
						std::once_flag flag;
						std::atomic<std::size_t> operations_left;
						bool set_done_called {false};
						std::optional<typename when_all_sender::template error_types<std::variant>> err;

						// This is what is happening:
						//   1. We need storage for any possible values that our many receivers
						//      can receive. We get the main list of types from when_all_sender::value_types,
						//		which is either empty (because one or more senders do not send anything)
						//      or in order the types that are sent from our senders. This can look like one of two options:
						//			a. cmoon::meta::type_list<cmoon::meta::type_list<>>
						//			b. cmoon::meta::type_list<cmoon::meta::type_list<sv1, sv2, ..., svN>>
						//	 2. The list from step 1 has its first type extracted (which is the inner type_list).
						//   3. The list is then transformed using augment_type. This converts references to
						//		reference_wrappers and values to optionals.
						//	 4. All the types in the list are then forwarded to std::tuple for the final storage
						//   5. If we have only one receiver, then no synchronization is needed, and there
						//		is no need to store these values
						typename when_all_sender::template value_types<cmoon::meta::type_list,
							cmoon::meta::type_list>::template type<0>::template transform<augment_type_s>::template complete_type<std::tuple> value_storage;
					};

					template<>
					struct state_t<true> {};

					state_t<no_synchronization> state;

					template<std::size_t I>
					friend class single_receiver;

					template<bool NoSync>
					struct has_tuple_storage_impl
					{
						static constexpr auto value {std::tuple_size_v<decltype(state.value_storage)> != 0};
					};

					template<>
					struct has_tuple_storage_impl<true>
					{
						static constexpr auto value {false};
					};

					static constexpr bool has_tuple_storage {has_tuple_storage_impl<no_synchronization>::value};
			};

			template<receiver R, std::size_t... Ints>
			op(R&&, std::index_sequence<Ints...>, Ss&&...) -> op<R, Ints...>;
		public:
			constexpr when_all_sender(Ss&&... ss)
				: senders{std::forward<Ss>(ss)...} {}

			template<receiver R>
			constexpr friend auto tag_invoke(connect_t, when_all_sender&& s, R&& output_receiver)
			{
				return std::apply([&output_receiver](auto&&... senders) mutable {
					return op{std::forward<R>(output_receiver), std::index_sequence_for<Ss...>{}, std::forward<decltype(senders)>(senders)...};
				}, std::move(s.senders));
			}
		private:
			std::tuple<Ss...> senders;
	};

	class when_all_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<typed_sender... Ss>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(when_all_t t, Ss&&... ss) {
					{ tag_invoke(t, std::forward<Ss>(ss)...) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<when_all_t, Ss...>};
				}
				else
				{
					return {state::other, std::is_nothrow_constructible_v<when_all_sender<Ss...>, Ss...>};
				}
			}
		public:
			template<typed_sender... Ss>
				requires(sizeof...(Ss) > 0 && 
							((requires
							{
								typename value_types_of_t<Ss, std::tuple, zero_or_one>;
							}) && ...)
						)
			constexpr decltype(auto) operator()(Ss&&... ss) const noexcept(choose<Ss...>().no_throw)
			{
				constexpr auto choice {choose<Ss...>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Ss>(ss)...);
				}
				else if constexpr (choice.strategy == state::other)
				{
					return when_all_sender<Ss...>{std::forward<Ss>(ss)...};
				}
			}
	};

	export
	inline constexpr when_all_t when_all{};

	export
	struct when_all_with_variant_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class... S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(when_all_with_variant_t t, S&&... s) {
					{ tag_invoke(t, std::forward<S>(s)...) } -> typed_sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<when_all_with_variant_t, S...>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<typed_sender... S>
			constexpr decltype(auto) operator()(S&&... s) const noexcept(choose<S...>().no_throw)
			{
				constexpr auto choice {choose<S...>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s)...);
				}
				else if constexpr (choice.strategy == state::other)
				{
					return execution::when_all(execution::into_variant(std::forward<S>(s))...);
				}
			}
	};

	export
	inline constexpr when_all_with_variant_t when_all_with_variant{};
}