export module cmoon.execution.transform;

import <functional>;
import <utility>;
import <exception>;
import <variant>;
import <type_traits>;
import <optional>;

import cmoon.meta;
import cmoon.type_traits;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.sender_base;
import cmoon.execution.sender_traits;
import cmoon.execution.typed_sender;
import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.operation_state;
import cmoon.execution.set_done;
import cmoon.execution.set_error;
import cmoon.execution.set_value;

namespace cmoon::execution
{
	template<sender S, class F>
	struct transform_sender_base : public sender_base {};

	template<typed_sender S, class F>
	struct transform_sender_base<S, F>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Tuple<typename sender_traits<S>::template value_types<cmoon::meta::type_list, std::variant>::template complete_type<std::invoke_result, F>::type>;

		template<template<class...> class Variant>
		using error_types = typename sender_traits<S>::template error_types<Variant>;

		static constexpr bool sends_done {true};
	};

	template<sender S, class F>
	class transform_sender : public transform_sender_base<S, F>
	{
		template<receiver R>
		class transform_receiver
		{
			public:
				constexpr transform_receiver(R&& output_receiver, F&& f)
					: output_receiver{std::forward<R>(output_receiver)},
					  f_{std::forward<F>(f)} {}

				template<class... Ts>
				void set_value(Ts&&... ts)
				{
					// set_value's exception should not be captured,
					// but if invoking the function throws, then
					// that should be caught.

					std::optional<std::invoke_result_t<F, Ts...>> v;
					try
					{
						v = std::invoke(std::move(f_), std::forward<Ts>(ts)...);
					}
					catch (...)
					{
						execution::set_error(std::move(output_receiver), std::current_exception());
						return;
					}

					execution::set_value(std::move(output_receiver), std::move(v.value()));
				}

				template<class E>
				void set_error(E&& e) noexcept
				{
					execution::set_error(std::move(output_receiver), std::forward<E>(e));
				}

				void set_done() noexcept
				{
					execution::set_done(std::move(output_receiver));
				}
			private:
				R output_receiver;
				F f_;
		};

		template<receiver R>
		class op2
		{
			public:
				constexpr op2(S&& s, F&& f, R&& r)
					: op_{execution::connect(std::forward<S>(s), transform_receiver<R>{std::forward<R>(r), std::forward<F>(f)})} {}

				constexpr void start() && noexcept
				{
					execution::start(std::move(op_));
				}
			private:
				connect_result_t<S, transform_receiver<R>> op_;
		};

		public:
			constexpr transform_sender(S&& s, F&& f)
				: s_{std::forward<S>(s)}, f_{std::forward<F>(f)} {}

			template<receiver R>
			constexpr auto connect(R&& r) &&
			{
				return op2<R>(std::move(s_), std::move(f_), std::forward<R>(r));
			}
		private:
			S s_;
			F f_;
	};

	namespace transform_ns
	{
		void transform();

		class transform_t
		{
			private:
				enum class state { member_fn, default_fn, other };

				template<sender S, class F>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(S&& s, F&& f) {
						std::forward<S>(s).transform(std::forward<F>(f));
					})
					{
						return {state::member_fn, noexcept(std::declval<S>().transform(std::declval<F>()))};
					}
					else if constexpr (requires(S&& s, F&& f) {
						transfor(std::forward<S>(s), std::forward<F>(f));
					})
					{
						return {state::default_fn, noexcept(transform(std::declval<S>(), std::declval<F>()))};
					}
					else
					{
						return {state::other};
					}
				}

				template<class F>
				struct transform_adapter
				{
					public:
						constexpr transform_adapter(F&& f) noexcept(std::is_nothrow_move_constructible_v<F>)
							: f_{std::forward<F>(f)} {}

						template<sender S>
						constexpr friend auto operator|(S&& s, transform_adapter&& a)
						{
							return transform_t{}(std::forward<S>(s), std::move(a.f_));
						}
					private:
						F f_;
				};
			public:
				template<sender S, class F>
				constexpr auto operator()(S&& s, F&& f) const noexcept(choose<S, F>().no_throw)
				{
					constexpr auto choice {choose<S, F>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						return std::forward<S>(s).transform(std::forward<F>(f));
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						return transform(std::forward<S>(s), std::forward<F>(f));
					}
					else if constexpr (choice.strategy == state::other)
					{
						return transform_sender{std::forward<S>(s), std::forward<F>(f)};
					}
				}

				template<class F>
				constexpr auto operator()(F&& f) const noexcept(std::is_nothrow_constructible_v<transform_adapter<F>, F>)
				{
					return transform_adapter<F>{std::forward<F>(f)};
				}
		};
	}

	export
	inline constexpr transform_ns::transform_t transform {};
}