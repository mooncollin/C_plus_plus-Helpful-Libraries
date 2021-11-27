export module cmoon.execution.start;

import <utility>;

import cmoon.functional;

namespace cmoon::execution
{
	export
	struct start_t
	{
		template<class O>
			requires(requires(start_t t, O&& o) {
				tag_invoke(t, o);
			})
		constexpr void operator()(O&& o) const noexcept(cmoon::nothrow_tag_invocable<start_t, O&>)
		{
			tag_invoke(*this, o);
		}
	};

	export
	inline constexpr start_t start{};
}