export module cmoon.math.ceil;

import <concepts>;
import <type_traits>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<std::integral Rep = int, cmoon::arithmetic T>
	[[nodiscard]] constexpr Rep ceil(const T& t) noexcept
	{
		if constexpr (std::integral<T>)
		{
			return t;
		}
		else
		{
			const auto inum = static_cast<Rep>(t);
			
			if(t == static_cast<T>(inum))
			{
				return inum;
			}

			return inum + (t > 0 ? 1 : 0);
		}
	}
}