export module cmoon.execution.get_scheduler;

import <utility>;

import cmoon.functional;

import cmoon.execution.receiver;

namespace cmoon::execution
{
	export
	struct get_scheduler_t
	{
		template<receiver R>
			requires(requires(get_scheduler_t t, R&& r) {
				{ tag_invoke(t, std::forward<R>(r)) } noexcept;
			})
		constexpr decltype(auto) operator()(R&& r) const noexcept
		{
			return tag_invoke(*this, std::forward<R>(r));
		}
	};

	export
	inline constexpr get_scheduler_t get_scheduler{};
}