export module cmoon.execution.set_done;

import <utility>;

import cmoon.functional;

namespace cmoon::execution
{
	export
	struct set_done_t
	{
		template<class R>
			requires(requires(set_done_t t, R&& r) {
				tag_invoke(t, std::forward<R>(r));
			})
		constexpr void operator()(R&& r) const noexcept(cmoon::nothrow_tag_invocable<set_done_t, R>)
		{
			tag_invoke(*this, std::forward<R>(r));
		}
	};

	export
	inline constexpr set_done_t set_done{};
}