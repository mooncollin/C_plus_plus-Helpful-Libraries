export module cmoon.execution.executor_future;

import <utility>;

import cmoon.property;

import cmoon.execution.twoway_t;

namespace cmoon::execution
{
	export
	template<class Executor, class T>
	struct executor_future
	{
		using type = decltype(cmoon::require(std::declval<const Executor&>(), execution::twoway).twoway_execute(std::declval<T(*)()>()));
	};
}