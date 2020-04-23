#pragma once

#include <utility>
#include <algorithm>
#include <functional>
#include "iterator.hpp"

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
	}
}