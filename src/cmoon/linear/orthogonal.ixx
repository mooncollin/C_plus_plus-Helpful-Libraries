export module cmoon.linear.orthogonal;

import <cstddef>;

import cmoon.linear.is_matrix;
import cmoon.linear.matrix;
import cmoon.linear.static_matrix;
import cmoon.linear.square_matrix;
import cmoon.linear.transpose;
import cmoon.linear.inverse;

namespace cmoon::linear
{
	export
	template<matrix_type M>
	[[nodiscard]] bool is_orthogonal(const M& m)
	{
		return inverse(m) == transpose(m);
	}
}