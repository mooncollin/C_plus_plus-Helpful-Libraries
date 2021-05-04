export module cmoon.execution.is_twoway_executor;

import <type_traits>;
import <concepts>;
import <future>;

import cmoon.utility;

import cmoon.execution.invocable_archetype;

namespace cmoon::execution
{
	template<class X, class F = invocable_archetype>
	concept is_twoway_executor_impl = std::move_constructible<std::decay_t<F>> &&
		requires(X x, F&& f)
	{
			{ x.twoway_execute(std::forward<F>(f)) } -> std::same_as<std::future<std::invoke_result_t<std::decay_t<F>>>>;
	};

	export
	template<class Executor>
	struct is_twoway_executor : std::bool_constant<is_twoway_executor_impl<Executor>> {};

	export
	template<class Executor>
	constexpr auto is_twoway_executor_v = is_twoway_executor<Executor>::value;
}