#pragma once

#include <type_traits>
#include <concepts>
#include <future>

#include "cmoon/execution/impl/invocable_archetype.hpp"
#include "cmoon/execution/impl/executor_shape.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class X, class F = invocable_archetype>
			concept is_bulk_twoway_executor_impl = std::move_constructible<std::decay_t<F>> &&
				requires(X x, F&& f, executor_shape_t<X> n, int(*rf)(), int(*sf)())
			{
				{ x.bulk_twoway_execute(std::forward<F>(f), n, rf, sf) } -> std::same_as<std::future<int>>;
			};
		}

		template<class Executor>
		struct is_bulk_twoway_executor : std::bool_constant<details::is_bulk_twoway_executor_impl<Executor>> {};

		template<class Executor>
		constexpr auto is_bulk_twoway_executor_v = is_bulk_twoway_executor<Executor>::value;
	}
}