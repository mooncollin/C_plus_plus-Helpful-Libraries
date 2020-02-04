#ifndef COLLIN_UTILITY
#define COLLIN_UTILITY

#include <iterator>
#include <exception>

namespace collin
{
	template<class Container>
	constexpr auto& last_of(Container& container, typename Container::size_type n = 1)
	{
		return *(std::end(container) - n);
	}

	template<class Container>
	constexpr auto& first_of(Container& container, typename Container::size_type n = 1)
	{
		return *(std::begin(container) + n);
	}

	template<class Container, class... Args>
	constexpr auto construct_value(Args&&... args)
	{
		return typename Container::value_type {std::forward<Args...>(args)...};
	}

	template<class Container, class... Args>
	constexpr auto construct_value(const Container& _, Args&&... args)
	{
		return construct_value<Container>(args...);
	}
}

#endif