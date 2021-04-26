#pragma once

#include <memory>
#include <cstddef>

namespace cmoon
{
	template<class T, class Allocator = std::allocator<T>>
	class multidimensional_array;

	template<class T, std::size_t Dimensions, class Allocator = std::allocator<T>>
	class fixed_multidimensional_array;

	template<class T, std::size_t... Dimensions>
		requires(sizeof...(Dimensions) > 0)
	class static_multidimensional_array;
}