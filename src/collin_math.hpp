#ifndef COLLIN_MATH
#define COLLIN_MATH

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace collin
{
	constexpr auto factorial(std::size_t n)
	{
		std::uintmax_t result = 1;
		for(decltype(n) i = 2; i <= n; i++)
		{
			result *= i;
		}

		return result;
	}

	template<class T, class = std::enable_if_t<std::is_integral_v<T>>>
	constexpr bool is_odd(const T& val)
	{
		return val & 1;
	}

	template<class T>
	constexpr bool is_even(const T& val)
	{
		return !is_odd(val);
	}
}

#endif