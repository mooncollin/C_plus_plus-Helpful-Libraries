#pragma once

#include <cstdint>
#include <concepts>
#include <iterator>

namespace cmoon
{
	template<std::invocable<> F>
	constexpr void repeat(std::uintmax_t n, F&& f) noexcept(noexcept(f()))
	{
		while(n--) f();
	}

	template<std::input_iterator InputIterator1, std::input_iterator InputIterator2>
	constexpr std::strong_ordering sequence_compare(InputIterator1 first1, const InputIterator1 last1, InputIterator2 first2, const InputIterator2 last2)
	{
		while (first1 != last1 && first2 != last2)
		{
			const auto r = *first1 <=> *first2;
			if (r != 0)
			{
				return r;
			}

			++first1;
			++first2;
		}

		if (first1 == last1 && first2 != last2)
		{
			return std::strong_ordering::less;
		}

		if (first1 != last1 && first2 == last2)
		{
			return std::strong_ordering::greater;
		}

		return std::strong_ordering::equal;
	}
}