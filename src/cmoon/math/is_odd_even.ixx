export module cmoon.math.is_odd_even;

import <concepts>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<std::integral T>
	[[nodiscard]] constexpr bool is_odd(const T& val) noexcept
	{
		return val & 1;
	}

	export
	template<std::floating_point F>
	[[nodiscard]] constexpr bool is_odd(const F& val) noexcept
	{
		return is_odd(static_cast<std::intmax_t>(val));
	}

	export
	template<cmoon::arithmetic T>
	[[nodiscard]] constexpr bool is_even(const T& val) noexcept
	{
		return !is_odd(val);
	}
}