export module cmoon.algorithm.repeat;

import <concepts>;
import <cstdint>;
import <functional>;

namespace cmoon
{
	export
	template<std::invocable<> F>
	constexpr void repeat(std::uintmax_t n, F&& f) noexcept(noexcept(f()))
	{
		while(n--) std::invoke(f);
	}
}