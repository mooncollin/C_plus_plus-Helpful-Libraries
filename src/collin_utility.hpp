#ifndef COLLIN_UTILITY
#define COLLIN_UTILITY

#include <iterator>
#include <exception>

namespace collin
{
	template<class T>
	constexpr auto& last_of(T& thing, typename T::size_type n = 1)
	{
		return *(std::end(thing) - n);
	}

	template<class T>
	constexpr auto& first_of(T& thing, typename T::size_type n = 1)
	{
		return *(std::begin(thing) + n);
	}
}

#endif