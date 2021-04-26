export module cmoon.linear.inverse;

import <cstddef>;
import <type_traits>;
import <stdexcept>;
import <algorithm>;

import cmoon.linear.is_matrix;
import cmoon.linear.matrix;
import cmoon.linear.static_matrix;
import cmoon.linear.square_matrix;
import cmoon.linear.gaussian_elimination;
import cmoon.linear.determinant;

namespace cmoon::linear
{
	template<class T, class Allocator>
	[[nodiscard]] matrix<T, Allocator> inverse_impl(matrix<T, Allocator>& m)
	{
		auto identity = identity_matrix<T, Allocator>(m.rows());
		return gaussian_elimination(m, identity);
	}

	template<class T, std::size_t S>
	[[nodiscard]] square_matrix<T, S> inverse(square_matrix<T, S>& m)
	{
		if (std::is_constant_evaluated())
		{
			const auto det = determinant(m);
			if (det == 0)
			{
				throw std::invalid_argument{"cannot find the inverse of a singular matrix"};
			}

			const auto adj = adjugate(m);

			square_matrix<T, S> out;
			std::transform(std::begin(adj),
							std::end(adj),
							std::begin(out),
							[det](const auto& v) { return v / det; });

			return out;
		}
		else
		{
			auto identity = identity_matrix<T, S>();
			return gaussian_elimination(m, identity);
		}
	}
	
	export
	template<matrix_type M>
	[[nodiscard]] auto inverse(M m)
	{
		return inverse_impl(m);
	}

	export
	template<matrix_type M>
		requires(!std::is_lvalue_reference_v<M>)
	[[nodiscard]] auto inverse(M&& m)
	{
		return inverse_impl(m);
	}
}