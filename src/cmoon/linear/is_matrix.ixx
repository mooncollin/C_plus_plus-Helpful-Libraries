module;

#include "cmoon/linear/matrix_definitions.hpp"

export module cmoon.linear.is_matrix;

import <cstddef>;

namespace cmoon::linear
{
	export
	template<class T>
	struct is_matrix : std::false_type {};

	export
	template<class Rep, class Allocator>
	struct is_matrix<matrix<Rep, Allocator>> : std::true_type {};

	export
	template<class T>
	constexpr auto is_matrix_v = is_matrix<T>::value;

	export
	template<class T>
	concept regular_matrix_type = is_matrix_v<T>;

	export
	template<class T>
	struct is_fixed_matrix : std::false_type {};

	export
	template<class Rep, std::size_t Rows, std::size_t Cols>
	struct is_fixed_matrix<fixed_matrix<Rep, Rows, Cols>> : std::true_type {};

	export
	template<class T>
	constexpr auto is_fixed_matrix_v = is_fixed_matrix<T>::value;

	export
	template<class T>
	concept fixed_matrix_type = is_fixed_matrix_v<T>;

	export
	template<class T>
	concept matrix_type = regular_matrix_type<T> || fixed_matrix_type<T>;
}