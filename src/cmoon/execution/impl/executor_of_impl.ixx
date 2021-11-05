export module cmoon.execution.impl.executor_of_impl;

import <utility>;

import cmoon.execution.impl.executor_impl;
import cmoon.execution.execute;

namespace cmoon::execution
{
	export
	template<class E, class F>
	concept executor_of_impl = executor_base_concept<E> &&
							   executor_fn_base_concept<F> &&
		requires(const E& e, F&& f)
	{
		execution::execute(e, std::forward<F>(f));
	};
}