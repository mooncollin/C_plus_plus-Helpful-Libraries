export module cmoon.execution.get_allocator;

import <utility>;

import cmoon.functional;

import cmoon.execution.receiver;

namespace cmoon::execution
{
	export
	struct get_allocator_t
	{
		template<receiver R>
			requires(requires(get_allocator_t t, R&& r) {
				{ tag_invoke(t, std::forward<R>(r)) } noexcept;
			})
		constexpr decltype(auto) operator()(R&& r) const noexcept
		{
			return tag_invoke(*this, std::forward<R>(r));
		}
	};

	export
	inline constexpr get_allocator_t get_allocator{};
}