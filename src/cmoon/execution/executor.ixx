export module cmoon.execution.executor;

import <type_traits>;
import <concepts>;
import <utility>;

import cmoon.execution.invocable_archetype;
import cmoon.execution.impl.executor_impl;
import cmoon.execution.impl.executor_of_impl;

namespace cmoon::execution
{
	export
	template<class E>
	concept executor = executor_of_impl<E, execution::invocable_archetype>;

	export
	template<class E, class F>
	concept executor_of = executor<E> &&
						  executor_of_impl<E, F>;
}