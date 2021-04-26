export module cmoon.math.sign;

import <concepts>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr int sign(T val) noexcept
	{
		if constexpr (std::unsigned_integer<T>)
		{
			return 1;
		}
		else
		{
			return val < T{0} ? -1 : 1;
		}
	}
}