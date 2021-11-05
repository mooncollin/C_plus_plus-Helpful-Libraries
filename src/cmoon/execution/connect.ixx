export module cmoon.execution.connect;

import <utility>;
import <type_traits>;
import <exception>;

import cmoon.meta;
import cmoon.type_traits;

import cmoon.execution.impl.executor_of_impl;
import cmoon.execution.impl.as_invocable;
import cmoon.execution.as_receiver;
import cmoon.execution.set_error;
import cmoon.execution.operation_state;
import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.execute;

namespace cmoon::execution
{
	template<class S, class R>
	struct as_operation
	{
		std::remove_cvref_t<S> e_;
		std::remove_cvref_t<R> r_;

		void start() noexcept try
		{
			execution::execute(std::move(e_), as_invocable<std::remove_cvref_t<R>, S>{r_});
		}
		catch (...)
		{
			execution::set_error(std::move(r_), std::current_exception());
		}
	};

	namespace connect_ns
	{
		void connect();

		class connect_t
		{
			private:
				enum class state { none, member_fn, default_fn, as_operation_s };

				template<class S, class R>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (sender<S> && requires(S&& s, R&& r) {
						{ std::forward<S>(s).connect(std::forward<R>(r)) } -> operation_state;
					})
					{
						return {state::member_fn, noexcept(std::declval<S>().connect(std::declval<R>()))};
					}
					else if constexpr (sender<S> && requires(S&& s, R&& r) {
						{ connect(std::forward<S>(s), std::forward<R>(r)) } -> operation_state;
					})
					{
						return {state::default_fn, noexcept(connect(std::declval<S>(), std::declval<R>()))};
					}
					else if constexpr (!cmoon::is_specialization_v<R, as_receiver> &&
									   receiver_of<R> &&
									   executor_of_impl<std::remove_cvref_t<S>, as_invocable<std::remove_cvref_t<S>, S>>)
					{
						return {state::as_operation_s, std::is_nothrow_constructible_v<as_operation<S, R>, S, R>};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class S, class R>
					requires(choose<S, R>().strategy != state::none)
				constexpr operation_state auto operator()(S&& s, R&& r) const noexcept(choose<S, R>().no_throw)
				{
					constexpr auto choice {choose<S, R>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						return std::forward<S>(s).connect(std::forward<R>(r));
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						return connect(std::forward<S>(s), std::forward<R>(r));
					}
					else if constexpr (choice.strategy == state::as_operation_s)
					{
						return as_operation<S, R>{std::forward<S>(s), std::forward<R>(r)};
					}
				}
		};
	}

	export
	inline constexpr connect_ns::connect_t connect{};

	export
	template<class S, class R>
	using connect_result_t = std::invoke_result_t<decltype(connect), S, R>;
}