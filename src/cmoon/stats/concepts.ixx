export module cmoon.stats.concepts;

import <ranges>;
import <type_traits>;
import <concepts>;

namespace cmoon::stats
{
	export
	template<typename R, typename P>
	concept stats_range = std::ranges::input_range<R> &&
						  std::is_arithmetic_v<typename std::projected<std::ranges::iterator_t<R>, P>::value_type>;

	export
	template<typename R, typename P1, typename Weights, typename P2>
	concept weighted_stats_range = stats_range<R, P1> &&
								   stats_range<Weights, P2>;

	export
	template<class T>
	concept stats_ordered =
		requires(const std::remove_reference_t<T>& a, const std::remove_reference_t<T>& b)
	{
		{ a < b } -> std::convertible_to<bool>;
	};
}