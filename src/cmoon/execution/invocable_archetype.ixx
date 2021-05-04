export module cmoon.execution.invocable_archetype

import <concepts>;

namespace cmoon::execution
{
	struct invocable_archetype_t
	{
		void operator()()
		{

		}
	};

	export
	using invocable_archetype = invocable_archetype_t;

	static_assert(std::invocable<invocable_archetype&>);
}