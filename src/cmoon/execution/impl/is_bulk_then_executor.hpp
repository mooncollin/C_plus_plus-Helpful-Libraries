#pragma once

#include <future>
#include <concepts>
#include <type_traits>

#include "cmoon/execution/impl/executor_shape.hpp"
#include "cmoon/execution/impl/invocable_archetype.hpp"
#include "cmoon/execution/impl/normal_type_impl.h"
#include "cmoon/execution/impl/is_future.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class X, class F = invocable_archetype, class Fut = std::future<int>>
			concept is_bulk_then_executor_impl = std::move_constructible<std::decay_t<F>> &&
												 details::is_future_v<Fut> &&
				requires(X x, F&& f, Fut fut, executor_shape_t<X> n, int(*rf)(), int(*sf)())
			{
				{ x.bulk_then_execute(std::forward<F>(f), n, fut, rf, sf) } -> std::same_as<std::future<int>>;
			};
		}

		template<class Executor>
		struct is_bulk_then_executor : std::bool_constant<details::is_bulk_then_executor_impl<Executor>> {};

		template<class Executor>
		constexpr auto is_bulk_then_executor_v = is_bulk_then_executor<Executor>::value;
	}
}