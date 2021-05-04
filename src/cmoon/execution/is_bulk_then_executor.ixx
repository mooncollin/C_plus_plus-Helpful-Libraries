export module cmoon.execution.is_bulk_then_executor;

import <future>;
import <concepts>;
import <type_traits>;

import cmoon.type_traits;

import cmoon.execution.invocable_archetype;
import cmoon.execution.executor_shape;
import cmoon.execution.normal_type_impl;

namespace cmoon::execution
{
	template<class X, class F = invocable_archetype, class Fut = std::future<int>>
	concept is_bulk_then_executor_impl = std::move_constructible<std::decay_t<F>> &&
										 cmoon::is_future_v<Fut> &&
		requires(X x, F&& f, Fut fut, executor_shape_t<X> n, int(*rf)(), int(*sf)())
	{
		{ x.bulk_then_execute(std::forward<F>(f), n, fut, rf, sf) } -> std::same_as<std::future<int>>;
	};

	export
	template<class Executor>
	struct is_bulk_then_executor : std::bool_constant<is_bulk_then_executor_impl<Executor>> {};

	export
	template<class Executor>
	constexpr auto is_bulk_then_executor_v = is_bulk_then_executor<Executor>::value;
}