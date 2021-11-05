export module cmoon.stats.impl.invalid_input;

import <concepts>;
import <cmath>;
import <limits>;

namespace cmoon::stats
{
	export
	template<std::floating_point Result>
	[[nodiscard]] constexpr bool is_invalid_input(const Result v) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result neg_inf {-std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		return v == inf || v == neg_inf || std::isnan(v);
	}
}