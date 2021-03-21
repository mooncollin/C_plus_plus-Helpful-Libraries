#pragma once

#include "cmoon/property.hpp"
#include "cmoon/execution/impl/twoway_t.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class Executor, class T>
		struct executor_future
		{
			using type = decltype(cmoon::require(std::declval<const Executor&>(), execution::twoway).twoway_execute(std::declval<T(*)()>()));
		};
	}
}