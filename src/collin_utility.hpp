#ifndef COLLIN_UTILITY
#define COLLIN_UTILITY

#include <iterator>
#include <exception>

namespace collin
{
	template<class T>
	const auto& last_of(const T& thing, typename T::size_type n = 1)
	{
		return *(std::cend(thing) - n);
	}

	template<class T>
	auto& last_of(T& thing, typename T::size_type n = 1)
	{
		return *(std::end(thing) - n);
	}

	template<class T>
	const auto& first_of(const T& thing, typename T::size_type n = 1)
	{
		return *(std::cbegin(thing) + n);
	}

	template<class T>
	auto& first_of(T& thing, typename T::size_type n = 1)
	{
		return *(std::begin(thing) + n);
	}
}

#endif