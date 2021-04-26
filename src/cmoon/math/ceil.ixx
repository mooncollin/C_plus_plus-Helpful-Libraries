export module cmoon.math.ceil;

import <concepts>;

namespace cmoon
{
	export
	template<std::integral Rep = int, std::floating_point Float>
	[[nodiscard]] constexpr Rep ceil(const Float& f) noexcept
	{
		const auto inum = static_cast<Rep>(f);
			
		if(f == static_cast<Float>(inum))
		{
			return inum;
		}

		return inum + (f > 0 ? 1 : 0);
	}
}