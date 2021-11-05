export module cmoon.execution.invocable_archetype;

import <concepts>;

namespace cmoon::execution
{
	export
	struct invocable_archetype
	{
		invocable_archetype() = delete;

		constexpr void operator()() const noexcept {}
	};

	static_assert(std::invocable<invocable_archetype&>);
}