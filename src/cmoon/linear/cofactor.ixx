export module cmoon.linear.cofactor;

import <cstddef>;

import cmoon.math;

import cmoon.linear.matrix;
import cmoon.linear.fixed_matrix;
import cmoon.linear.square_matrix;
import cmoon.linear.sub_matrix;
import cmoon.linear.determinant;

namespace cmoon::linear
{
	export
	template<class T, class Allocator>
	[[nodiscard]] T cofactor(const matrix<T, Allocator>& m, const std::size_t p, const std::size_t q)
	{
		const auto sub = sub_matrix(m, p, q);
		const auto det = determinant(sub);
		return cmoon::is_even(p + q) ? det : -det;
	}

	export
	template<class T, std::size_t S>
	[[nodiscard]] constexpr T cofactor(const square_matrix<T, S>& m, const std::size_t p, const std::size_t q) noexcept
	{
		const auto sub = sub_matrix(m, p, q);
		const auto det = determinant(sub);
		return cmoon::is_even(p + q) ? det : -det;
	}
}