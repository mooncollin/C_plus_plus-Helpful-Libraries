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
import cmoon.type_traits;

import cmoon.execution.receiver;
import cmoon.execution.connect;
import cmoon.execution.typed_sender;
import cmoon.execution.sender_traits;
import cmoon.execution.set_done;
import cmoon.execution.set_error;
import cmoon.execution.set_value;

namespace cmoon::execution
{
	template<class Si>
	concept has_single_value_type = typed_sender<Si> &&
									typename sender_traits<Si>::template value_types<cmoon::meta::type_list, std::variant>::size() == 1;

	template<typed_sender Si>
		requires(has_single_value_type<Si>)
	using single_value_t = typename sender_traits<Si>::template value_types<cmoon::meta::type_list, std::variant>::template type<0>;

	template<typename T>
	using augment_type = std::conditional_t<cmoon::specialization_of<std::remove_cvref_t<T>, std::reference_wrapper>,
											std::remove_cvref_t<T>,
											std::conditional_t<std::is_lvalue_reference_v<T>,
															   std::reference_wrapper<std::remove_reference_t<T>>,
															   std::optional<std::remove_cvref_t<T>>
															  >
											>;

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

	template<typed_sender... Ss>
		requires((has_single_value_type<Ss> && ...))
	class when_all_sender
	{
		template<receiver R, std::size_t... Ints>
		class op
		{
			template<std::size_t I>
			class single_receiver
			{
				template<class T>
				[[nodiscard]] static constexpr auto unwrap_type(T&& t)
				{
					if constexpr (cmoon::is_optional_v<T>)
					{
						return std::forward<T>(t).value();
					}
					else
					{
						return std::forward<T>(t);
					}
				}

				public:
					constexpr single_receiver(op& owner) noexcept
						: owner_{owner} {}

					template<class T>
					void set_value(T&& t)
					{
						std::get<I>(owner_.get().value_storage) = std::forward<T>(t);

						// We are the last one to set a value, and
						// all other values have been set
						if (owner_.get().values_left.fetch_sub(1) == 1)
						{
							std::apply([this](auto&&... arg) mutable {
								execution::set_value(std::move(owner_.get().output_receiver),
													 unwrap_type(std::forward<decltype(arg)>(arg))...);
							}, std::move(owner_.get().value_storage));
						}
					}

					template<class E>
					void set_error(E&& e) noexcept
					{
						std::call_once(owner_.get().flag,
									   [](op& owner, E&& e) {
											execution::set_error(
												std::move(owner.output_receiver),
												std::forward<E>(e)
											);
									   },
									   owner_,
									   std::forward<E>(e));
					}

					void set_done() noexcept
					{
						using f = decltype(execution::set_done);

						std::call_once(owner_.get().flag,
									   [](op& owner) {
											execution::set_done(
												std::move(owner.output_receiver)
											);
									   },
									   owner_);
					}
				private:
					std::reference_wrapper<op> owner_;
			};

			public:
				constexpr op(R&& r, std::index_sequence<Ints...>, Ss&&... ss2)
					: output_receiver{std::forward<R>(r)},
					  values_left{sizeof...(Ss)},
					  op_states{execution::connect(std::forward<Ss>(ss2), single_receiver<Ints>{*this})...} {}

				void start() && noexcept
				{
					std::apply([](auto&&... ops) {
						(execution::start(std::forward<decltype(ops)>(ops)), ...);
					}, std::move(op_states));
				}
			private:
				R output_receiver;
				std::atomic<std::size_t> values_left;
				std::tuple<connect_result_t<Ss, single_receiver<Ints>>...> op_states;
				std::tuple<augment_type<single_value_t<Ss>>...> value_storage;
				std::once_flag flag;

				template<std::size_t I>
				friend class single_receiver;
		};

		template<receiver R, std::size_t... Ints>
		op(R&&, std::index_sequence<Ints...>, Ss&&...) -> op<R, Ints...>;

		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Tuple<augment_type<single_value_t<Ss>>...>;

			template<template<class...> class Variant>
			using error_types = get_error_types_t<Ss...>::unique::template complete_type<Variant>;

			static constexpr bool sends_done {true};

			constexpr when_all_sender(Ss&&... ss)
				: senders{std::forward<Ss>(ss)...} {}

			template<receiver R>
			constexpr operation_state auto connect(R&& output_receiver) &&
			{
				return std::apply([&output_receiver, this](auto&&... s) mutable {
					return op{std::forward<R>(output_receiver), std::index_sequence_for<Ss...>{}, std::forward<decltype(s)>(s)...};
				}, std::move(senders));
			}
		private:
			std::tuple<Ss...> senders;
	};

	namespace when_all_ns
	{
		void when_all();

		class when_all_t
		{
			private:
				enum class state { default_fn, other };

				template<typed_sender... Ss>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(Ss&&... ss) {
						when_all(std::forward<Ss>(ss)...);
					})
					{
						return {state::default_fn, noexcept(when_all(std::declval<Ss>()...))};
					}
					else
					{
						return {state::other, noexcept(when_all_sender{std::declval<Ss>()...})};
					}
				}
			public:
				template<typed_sender... Ss>
				constexpr auto operator()(Ss&&... ss) const noexcept(choose<Ss...>().no_throw)
				{
					constexpr auto choice {choose<Ss...>()};

					if constexpr (choice.strategy == state::default_fn)
					{
						return when_all(std::forward<Ss>(ss)...);
					}
					else if constexpr (choice.strategy == state::other)
					{
						return when_all_sender{std::forward<Ss>(ss)...};
					}
				}
		};
	}

	export
	inline constexpr when_all_ns::when_all_t when_all {};
}