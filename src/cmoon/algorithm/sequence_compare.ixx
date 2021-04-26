export module cmoon.algorithm.sequence_compare;

import <iterator>;
import <compare>;

namespace cmoon
{
	export
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