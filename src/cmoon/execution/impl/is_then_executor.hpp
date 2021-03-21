#pragma once

#include <future>
#include <concepts>
#include <type_traits>

#include "cmoon/execution/impl/is_future.hpp"
#include "cmoon/execution/impl/normal_type_impl.h"
#include "cmoon/execution/impl/invocable_archetype.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class X, class F = invocable_archetype, class Future = std::future<void>>
			concept is_then_executor_impl = details::is_future_v<Future> &&
											std::move_constructible<std::decay_t<F>> &&
				requires(X x, F&& f, Future fut)
			{
				{ x.then_execute(std::forward<F>(), fut) }; // TODO: P1244r0 does not make it clear how decltype(val) is defined
			};
		}

		template<class Executor>
		struct is_then_executor : std::bool_constant<details::is_then_executor_impl<Executor>> {};

		template<class Executor>
		constexpr auto is_then_executor_v = is_then_executor<Executor>::value;
	}
}