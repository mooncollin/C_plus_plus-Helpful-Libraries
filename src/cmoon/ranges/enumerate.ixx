export module cmoon.ranges.enumerate;

import <ranges>;
import <utility>;
import <iterator>;
import <cstddef>;

import cmoon.ranges.zip;

namespace cmoon::ranges
{
	namespace views
	{
		export
		template<std::ranges::range Rng>
		[[nodiscard]] auto enumerate(Rng&& rng, std::size_t count = 0)
		{
			return zip(std::forward<Rng>(rng), std::ranges::views::iota(count));
		}
	}

	export
	template<std::ranges::range Rng>
	using enumerate_t = decltype(views::enumerate(std::declval<Rng>()));
}