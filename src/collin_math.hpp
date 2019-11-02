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

	template<class T, typename Cast=std::intmax_t>
	constexpr bool is_odd(const T& val)
	{
		if constexpr(std::is_integral<T>::value)
		{
			return val & 1;
		}
		else if constexpr(std::is_floating_point<T>::value)
		{
			return static_cast<Cast>(val) & 1;
		}
	}

	template<class T, typename Cast=std::intmax_t>
	constexpr bool is_even(const T& val)
	{
		return !(is_odd<Cast>(val));
	}
}

#endif