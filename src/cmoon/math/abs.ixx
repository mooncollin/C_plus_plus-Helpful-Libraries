export module cmoon.math.abs;

import <type_traits>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr T abs(const T& value) noexcept
	{
		if constexpr (std::is_unsigned_v<T>)
		{
			return value;
		}
		else
		{
			return value < T{0} ? -value : value;
		}
	}
}