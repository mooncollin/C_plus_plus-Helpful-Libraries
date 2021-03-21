#pragma once

#include <type_traits>
#include <concepts>
#include <future>

#include "cmoon/utility.hpp"
#include "cmoon/execution/impl/is_future.hpp"
#include "cmoon/execution/impl/invocable_archetype.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class X, class F = invocable_archetype>
			concept is_twoway_executor_impl = std::move_constructible<std::decay_t<F>> &&
				requires(X x, F&& f)
			{
				 { x.twoway_execute(std::forward<F>(f)) } -> std::same_as<std::future<std::invoke_result_t<std::decay_t<F>>>>;
			};
		}

		template<class Executor>
		struct is_twoway_executor : std::bool_constant<details::is_twoway_executor_impl<Executor>> {};

		template<class Executor>
		constexpr auto is_twoway_executor_v = is_twoway_executor<Executor>::value;
	}
}