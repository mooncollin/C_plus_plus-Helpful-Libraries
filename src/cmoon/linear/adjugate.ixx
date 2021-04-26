export module cmoon.linear.adjugate;

import <stdexcept>;
import <cstddef>;

import cmoon.linear.matrix;
import cmoon.linear.static_matrix;
import cmoon.linear.square_matrix;
import cmoon.linear.cofactor;

namespace cmoon::linear
{
	export
	template<class T, class Allocator>
	[[nodiscard]] matrix<T, Allocator> adjugate(const matrix<T, Allocator>& m)
	{
		if (!is_square_matrix(m))
		{
			throw std::invalid_argument{"can only find the adjugate of a square matrix"};
		}

		const auto size {m.rows()};

		matrix<T, Allocator> out{size, size};

		if (size == 1)
		{
			out(0, 0) = 1;
		}
		else if (size == 2)
		{
			out(0, 0) = m(1, 1);
			out(1, 1) = m(0, 0);

			out(0, 1) = -m(0, 1);
			out(1, 0) = -m(1, 0);
		}
		else
		{
			for(std::size_t i {0}; i < size; ++i)
			{
				for(std::size_t j {0}; j < size; ++j)
				{
					out(j, i) = cofactor(m, i, j);
				}
			}
		}

			

		return out;
	}

	export
	template<class T, std::size_t S>
	[[nodiscard]] constexpr square_matrix<T, S> adjugate(const square_matrix<T, S>& m) noexcept
	{
		square_matrix<T, S> out;

		if constexpr(S == 1)
		{
			out(0, 0) = 1;
		}
		else if constexpr(S == 2)
		{
			out(0, 0) = m(1, 1);
			out(1, 1) = m(0, 0);

			out(0, 1) = -m(0, 1);
			out(1, 0) = -m(1, 0);
		}
		else
		{
			for(std::size_t i {0}; i < S; ++i)
			{
				for(std::size_t j {0}; j < S; ++j)
				{
					out(j, i) = cofactor(m, i, j);
				}
			}
		}

		return out;
	}
}