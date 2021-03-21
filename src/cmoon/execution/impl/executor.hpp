#pragma once

#include <type_traits>
#include <concepts>

#include "cmoon/execution/impl/invocable_archetype.hpp"
#include "cmoon/execution/impl/execute.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class E, class F>
		concept executor_of = std::invocable<F> &&
							  std::is_nothrow_copy_constructible_v<E> &&
							  std::is_nothrow_destructible_v<E> &&
							  std::equality_comparable<E> &&
				requires(const E& e, F&& f)
			{
				execution::execute(e, std::forward<F>(f));
			};

		template<class E>
		concept executor = executor_of<E, execution::invocable_archetype>;
	}
}