#pragma once

#include <type_traits>
#include <concepts>

#include "cmoon/execution/impl/start.hpp"

namespace cmoon
{
	namespace execution
	{
		template<class O>
		concept operation_state = std::destructible<O> &&
								  std::is_object_v<O> &&
			requires(O& o)
		{
			{ execution::start(o) } noexcept;
		};
	}
}