export module cmoon.math.floor;

import <concepts>;

namespace cmoon
{
	export
	template<std::integral Rep = int, std::floating_point Float>
	[[nodiscard]] constexpr Rep floor(const Float& f) noexcept
	{
		const auto val_int = static_cast<Rep>(f);

		if(f >= 0 || f == static_cast<Float>(val_int))
		{
			return val_int;
		}

		return val_int - 1;
	}
}