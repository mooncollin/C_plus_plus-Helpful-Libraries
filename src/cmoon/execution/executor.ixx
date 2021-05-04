export module cmoon.execution.executor;

import <type_traits>;
import <concepts>;

import cmoon.execution.invocable_archetype;
import cmoon.execution.execute;

namespace cmoon::execution
{
	export
	template<class E, class F>
	concept executor_of = std::invocable<F> &&
							std::is_nothrow_copy_constructible_v<E> &&
							std::is_nothrow_destructible_v<E> &&
							std::equality_comparable<E> &&
			requires(const E& e, F&& f)
		{
			execution::execute(e, std::forward<F>(f));
		};

	export
	template<class E>
	concept executor = executor_of<E, execution::invocable_archetype>;
}