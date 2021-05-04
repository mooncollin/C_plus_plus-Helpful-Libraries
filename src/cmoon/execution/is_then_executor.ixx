export module cmoon.execution.is_then_executor;

import <future>;
import <concepts>;
import <type_traits>;

import cmoon.type_traits;

import cmoon.execution.invocable_archetype;
import cmoon.execution.normal_type_impl;

namespace cmoon::execution
{
	template<class X, class F = invocable_archetype, class Future = std::future<void>>
	concept is_then_executor_impl = cmoon::is_future_v<Future> &&
									std::move_constructible<std::decay_t<F>> &&
		requires(X x, F&& f, Future fut)
	{
		{ x.then_execute(std::forward<F>(), fut) }; // TODO: P1244R0 does not make it clear how decltype(val) is defined
	};

	export
	template<class Executor>
	struct is_then_executor : std::bool_constant<is_then_executor_impl<Executor>> {};

	export
	template<class Executor>
	constexpr auto is_then_executor_v = is_then_executor<Executor>::value;
}