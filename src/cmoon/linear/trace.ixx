export module cmoon.linear.trace;

import <cstddef>;

import cmoon.linear.matrix;
import cmoon.linear.static_matrix;

namespace cmoon::linear
{
	export
	template<class T, class Allocator>
	[[nodiscard]] T trace(const matrix<T, Allocator>& m) noexcept
	{
		return main_diagonal(m).sum();
	}

	export
	template<class T, std::size_t Rows, std::size_t Cols>
	[[nodiscard]] constexpr T trace(const static_matrix<T, Rows, Cols>& m) noexcept
	{
		return main_diagonal(m).sum();
	}
}