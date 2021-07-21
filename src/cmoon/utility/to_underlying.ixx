export module cmoon.utility.to_underlying;

import <type_traits>;

namespace cmoon
{
	export
	template<class Enum>
	[[nodiscard]] constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
	{
		return static_cast<std::underlying_type_t<Enum>>(e);
	}
}