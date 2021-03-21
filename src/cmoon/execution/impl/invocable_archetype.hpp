#pragma once

#include <concepts>

namespace cmoon
{
	namespace execution
	{
		using invocable_archetype = void(*)();

		static_assert(std::invocable<invocable_archetype&>);
	}
}