export module cmoon.ranges.adjacent;

import <ranges>;

import cmoon.ranges.zip;

namespace cmoon::ranges
{
	namespace views
	{
		export
		template<std::ranges::range Rng>
		constexpr auto adjacent(Rng&& rng)
		{
			return zip(rng, rng | std::ranges::views::drop(1));
		}

		export
		template<std::ranges::range Rng>
		using adjacent_t = decltype(views::adjacent(std::declval<Rng>()));
	}
}