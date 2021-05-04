export module cmoon.execution.is_bulk_twoway_executor;

import <type_traits>;
import <concepts>;
import <future>;
import <utility>;

import cmoon.execution.invocable_archetype;
import cmoon.execution.executor_shape;

namespace cmoon::execution
{
	template<class X, class F = invocable_archetype>
	concept is_bulk_twoway_executor_impl = std::move_constructible<std::decay_t<F>> &&
		requires(X x, F&& f, executor_shape_t<X> n, int(*rf)(), int(*sf)())
	{
		{ x.bulk_twoway_execute(std::forward<F>(f), n, rf, sf) } -> std::same_as<std::future<int>>;
	};

	export
	template<class Executor>
	struct is_bulk_twoway_executor : std::bool_constant<is_bulk_twoway_executor_impl<Executor>> {};

	export
	template<class Executor>
	constexpr auto is_bulk_twoway_executor_v = is_bulk_twoway_executor<Executor>::value;
}