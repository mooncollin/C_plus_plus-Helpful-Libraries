export module cmoon.numbers;

import <concepts>;

namespace cmoon
{
	export
	template<std::floating_point F>
	constexpr auto sqrt_5_v = static_cast<F>(2.23606797749978969640917366873127623544061835961152572427089);

	export
	template<std::floating_point F>
	constexpr auto golden_ratio_v = static_cast<F>(1.61803398874989484820458683436563811772030917980576286213544862270526046281890244970720720418939113748475);

	export
	constexpr auto sqrt_5 = sqrt_5_v<double>;

	export
	constexpr auto golden_ratio = golden_ratio_v<double>;
}