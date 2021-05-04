export module cmoon.execution.operation_state;

import <type_traits>;
import <concepts>;

import cmoon.execution.start;

namespace cmoon::execution
{
	export
	template<class O>
	concept operation_state = std::destructible<O> &&
								std::is_object_v<O> &&
		requires(O& o)
	{
		{ execution::start(o) } noexcept;
	};
}