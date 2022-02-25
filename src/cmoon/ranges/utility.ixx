export module cmoon.ranges.utility;

import <ranges>;
import <iterator>;

namespace cmoon::ranges
{
	export
	template<std::ranges::bidirectional_range R>
		requires(std::ranges::borrowed_range<R>)
	constexpr decltype(auto) last_of(R&& r, std::ranges::range_size_t<R> n = 1)
	{
		auto end {std::ranges::end(r)};
		std::advance(end, -n);
		return *end;
	}

	export
	template<std::ranges::forward_range R>
		requires(std::ranges::borrowed_range<R>)
	constexpr decltype(auto) first_of(R&& r, std::ranges::range_size_t<R> n = 0)
	{
		auto begin {std::begin(r)};
		std::advance(begin, n);
		return *begin;
	}
}