export module cmoon.math.floor;

import <concepts>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<std::integral Rep = int, cmoon::arithmetic T>
	[[nodiscard]] constexpr Rep floor(const T& t) noexcept
	{
		if constexpr (std::integral<T>)
		{
			return t;
		}
		else
		{
			const auto val_int = static_cast<Rep>(t);

			if(t >= 0 || t == static_cast<T>(val_int))
			{
				return val_int;
			}

			return val_int - 1;
		}
	}
}