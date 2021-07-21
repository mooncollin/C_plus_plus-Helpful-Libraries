module;

#include "cmoon/linear/matrix_definitions.hpp"

export module cmoon.linear.square_matrix;

import <cstddef>;

namespace cmoon::linear
{
	export
	template<class T, std::size_t S>
	using square_matrix = static_matrix<T, S, S>;
}