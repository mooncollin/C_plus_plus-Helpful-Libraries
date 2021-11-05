export module cmoon.execution.operation_state;

import <concepts>;
import <type_traits>;

import cmoon.execution.start;

namespace cmoon::execution
{
	export
	template<class O>
	concept operation_state = std::destructible<O> &&
							  std::is_object_v<O> &&
		requires(O&& o)
	{
		{ execution::start(std::forward<O>(o)) } noexcept;
	};
}