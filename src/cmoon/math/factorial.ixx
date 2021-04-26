export module cmoon.math.factorial;

import <array>;
import <cstdint>;
import <cstddef>;

namespace cmoon
{
	inline constexpr std::array precomputed_factorials = {
		1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800,
		479001600, 6227020800, 87178291200, 1307674368000, 20922789888000,
		355687428096000, 6402373705728000, 121645100408832000, 2432902008176640000
	};

	export
	[[nodiscard]] constexpr std::uintmax_t factorial(std::size_t n) noexcept
	{
		if (n < precomputed_factorials.size())
		{
			return precomputed_factorials[n];
		}
		else
		{
			std::uintmax_t result {precomputed_factorials.back()};
			for(std::size_t i {std::size(precomputed_factorials)}; i <= n; ++i)
			{
				result *= i;
			}

			return result;
		}
	}
}