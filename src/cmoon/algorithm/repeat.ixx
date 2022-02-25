export module cmoon.algorithm.repeat;

import <concepts>;
import <cstdint>;
import <functional>;
import <type_traits>;

namespace cmoon
{
	export
	template<std::invocable<> F>
	constexpr void repeat(std::uintmax_t n, F&& f) noexcept(std::is_nothrow_invocable_v<F>)
	{
		while(n--) std::invoke(f);
	}
}