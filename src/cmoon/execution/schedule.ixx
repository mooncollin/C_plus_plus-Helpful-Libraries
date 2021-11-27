export module cmoon.execution.schedule;

import <utility>;
import <exception>;
import <type_traits>;

import cmoon.functional;

import cmoon.execution.sender;

namespace cmoon::execution
{
	export
	struct schedule_t
	{
		template<class S>
			requires(requires(schedule_t t, S&& s) {
				{ tag_invoke(t, s) } -> sender;
			})
		constexpr decltype(auto) operator()(S&& s) const noexcept(cmoon::nothrow_tag_invocable<schedule_t, S>)
		{
			return tag_invoke(*this, s);
		}
	};

	export
	inline constexpr schedule_t schedule{};
}