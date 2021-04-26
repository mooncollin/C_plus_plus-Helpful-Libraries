#pragma once

#include <cstddef>

namespace cmoon::linear
{
	template<class Rep, class Allocator>
	class matrix;

	template<class Rep, std::size_t Rows, std::size_t Cols>
	class static_matrix;
}