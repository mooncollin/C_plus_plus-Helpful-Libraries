export module cmoon.stats.stats_result_t;

import <ranges>;
import <iterator>;
import <concepts>;
import <type_traits>;

namespace cmoon::stats
{
	export
	template<std::ranges::input_range R, std::invocable<std::ranges::range_reference_t<R>> P>
	using stats_result_t = std::conditional_t<
								std::integral<typename std::projected<std::ranges::iterator_t<R>, P>::value_type>,
								double,
								typename std::projected<std::ranges::iterator_t<R>, P>::value_type>;
}