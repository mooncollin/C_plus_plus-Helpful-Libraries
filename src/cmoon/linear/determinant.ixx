export module cmoon.linear.determinant;

import <stdexcept>;
import <algorithm>;
import <memory>;
import <concepts>;

import cmoon.linear.matrix;
import cmoon.linear.static_matrix;
import cmoon.linear.pivot_elimination;

namespace cmoon::linear
{
	export
	template<class T, class Allocator>
	[[nodiscard]] T determinant(const matrix<T, Allocator>& m)
	{
		if (!is_square_matrix(m))
		{
			throw std::invalid_argument {"can only find the determinant of a square matrix"};
		}

		const auto size = m.rows();

		if (size == 1)
		{
			return m(0, 0);
		}
		else if (size == 2)
		{
			return m(0, 0) * m(1, 1) -
				m(0, 1) * m(1, 0);
		}
		else if (size == 3)
		{
			const auto a = m(0, 0);
			const auto b = m(0, 1);
			const auto c = m(0, 2);

			const auto d = m(1, 0);
			const auto e = m(1, 1);
			const auto f = m(1, 2);

			const auto g = m(2, 0);
			const auto h = m(2, 1);
			const auto i = m(2, 2);

			return (a*e*i + b*f*g + c*d*h) -
					(a*f*h) -
					(b*d*i) -
					(c*e*g);
		}
			
		matrix<long double, typename std::allocator_traits<Allocator>::template rebind_alloc<long double>> elim {size, size};
		std::copy(std::begin(m), std::end(m), std::begin(elim));
		const auto operations = pivot_elimination(elim);
		const auto p = main_diagonal(elim).product();
		if constexpr (std::integral<T>)
		{
			return static_cast<T>(std::round(p * operations));
		}
		else
		{
			return static_cast<T>(p * operations);
		}
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr Rep determinant(const square_matrix<Rep, S>& m) noexcept
	{
		if constexpr (S == 1)
		{
			return m(0, 0);
		}
		else if constexpr (S == 2)
		{
			return m(0, 0) * m(1, 1) -
				m(0, 1) * m(1, 0);
		}
		else if constexpr (S == 3)
		{
			const auto a = m(0, 0);
			const auto b = m(0, 1);
			const auto c = m(0, 2);

			const auto d = m(1, 0);
			const auto e = m(1, 1);
			const auto f = m(1, 2);

			const auto g = m(2, 0);
			const auto h = m(2, 1);
			const auto i = m(2, 2);

			return (a*e*i + b*f*g + c*d*h) -
					(a*f*h) -
					(b*d*i) -
					(c*e*g);
		}
		else if (std::is_constant_evaluated())
		{
			int sign {1};
			Rep result {0};

			for(std::size_t f {0}; f < S; ++f)
			{
				result += sign * m(0, f) * cofactor(m, 0, f);
				sign = -sign;
			}

			return result;
		}
		else
		{
			square_matrix<long double, S> elim;
			std::copy(std::begin(m), std::end(m), std::begin(elim));
			const auto operations = pivot_elimination(elim);
			const auto p = main_diagonal(elim).product();
			if constexpr (std::integral<Rep>)
			{
				return static_cast<Rep>(std::round(p * operations));
			}
			else
			{
				return static_cast<Rep>(p * operations);
			}
		}
	}
}