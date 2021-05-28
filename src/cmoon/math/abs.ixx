export module cmoon.math.abs;

import cmoon.concepts;

namespace cmoon
{
	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr T abs(const T& value) noexcept
	{
		if constexpr (cmoon::unsigned_arithmetic<T>)
		{
			return value;
		}
		else
		{
			return value < T{0} ? -value : value;
		}
	}
}