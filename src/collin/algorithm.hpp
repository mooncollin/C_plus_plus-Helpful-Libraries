#pragma once

#include <utility>
#include <algorithm>
#include <functional>

#include "collin/iterator.hpp"
#include "collin/concepts.hpp"

namespace collin
{
	namespace algorithms
	{
		template<class InputIterator, class T>
		T& apply_range(InputIterator first, InputIterator last, T& val)
		{
			std::for_each(first, last, [&val](const auto& f) {
				f(val);
			});

			return val;
		}

		template<class Container, class Predicate>
		typename Container::iterator remove_until(Container& c, Predicate p)
		{
			auto first_false = std::find_if_not(std::begin(c), std::end(c), p);
			if(first_false != std::end(c))
			{
				first_false += 1;
			}
			return c.erase(std::begin(c), first_false);
		}

		template<class BidirIt>
		constexpr void reverse(BidirIt first, BidirIt last)
		{
			while ((first != last) && (first != --last)) {
				std::iter_swap(first++, last);
			}
		}

		template<class BidirIt>
		constexpr bool next_permutation(BidirIt first, BidirIt last)
		{
			if (first == last) return false;
			BidirIt i = last;
			if (first == --i) return false;

			while (true) {
				BidirIt i1, i2;

				i1 = i;
				if (*--i < *i1) {
					i2 = last;
					while (!(*i < *--i2))
						;
					std::iter_swap(i, i2);
					collin::algorithms::reverse(i1, last);
					return true;
				}
				if (i == first) {
					collin::algorithms::reverse(first, last);
					return false;
				}
			}
		}

		template<class InputIterator1, class InputIterator2>
		constexpr int sequence_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
		{
			for (; (first1 != last1) && (first2 != last2); ++first1, ++first2)
			{
				if (*first1 < *first2) return -1;
				if (*first2 < *first1) return 1;
			}

			return ((first1 == last1) && (first2 != last2)) ? -1 :
				   ((first1 != last1) && (first2 == last2)) ?  1 : 0;
		}

		template<class InputIterator1, class InputIterator2>
		constexpr int sequence_compare(InputIterator1 first1, InputIterator2 first2, std::size_t amount)
		{
			for (; amount-- > 0; ++first1, ++first2)
			{
				if (*first1 < *first2) return -1;
				if (*first2 < *first1) return 1;
			}

			return 0;
		}
	}
}