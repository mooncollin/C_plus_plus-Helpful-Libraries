export module cmoon.numbers;

import <concepts>;

namespace cmoon
{
	export
	template<std::floating_point F>
	inline constexpr auto sqrt_5_v = static_cast<F>(2.23606797749978969640917366873127623544061835961152572427089);

	export
	inline constexpr auto sqrt_5 = sqrt_5_v<double>;
}