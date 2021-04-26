export module cmoon.linear.pivot_elimination;

import <cstddef>;
import <stdexcept>;
import <algorithm>;

import cmoon.linear.is_matrix;
import cmoon.linear.matrix;
import cmoon.linear.static_matrix;
import cmoon.linear.square_matrix;

namespace cmoon::linear
{
	struct no_b_matrix {};

	template<matrix_type Matrix1, class Other>
	constexpr typename Matrix1::value_type pivot_elimination_impl(Matrix1& a, Other& b)
	{
		constexpr auto b_exists {!std::is_same_v<Other, no_b_matrix>};

		using value_t = typename Matrix1::value_type;
		const auto n {a.rows()};
		value_t operations {1};

		for (std::size_t j {0}; j < n; ++j)
		{
			auto pivot_row_i = j;

			for (std::size_t k {j + 1}; k < n; ++k)
			{
				if (cmoon::abs(a(k, j)) > cmoon::abs(a(pivot_row_i, j)))
				{
					pivot_row_i = k;
				}
			}

			if (pivot_row_i != j)
			{
				std::swap(a.get_row(j), a.get_row(pivot_row_i));
				if constexpr (b_exists)
				{
					std::swap(b.get_row(j), b.get_row(pivot_row_i));
				}
				operations = -operations;
			}

			const auto pivot_row {a.get_row(j)};
			const auto pivot {pivot_row[j]};

			if (pivot == 0)
			{
				throw std::invalid_argument{"Pivot cannot be zero"};
			}

			for (std::size_t i {j + 1}; i < n; ++i)
			{
				const auto below_pivot_row {a.get_row(i)};
				const auto mult {below_pivot_row[j] / pivot};
				const auto func {[mult](auto lhs, auto rhs) { return (lhs * -mult) + rhs; }};
				std::transform(std::begin(pivot_row),
								std::end(pivot_row),
								std::begin(below_pivot_row),
								std::begin(below_pivot_row),
								func);

				if constexpr (b_exists)
				{
					const auto b_pivot_row {b.get_row(j)};
					const auto b_below_pivot_row = b.get_row(i);
					std::transform(std::begin(b_pivot_row),
									std::end(b_pivot_row),
									std::begin(b_below_pivot_row),
									std::begin(b_below_pivot_row),
									func);
				}
			}
		}

		return operations;
	}

	export
	template<class T, class T2, class Allocator>
	auto pivot_elimination(matrix<T, Allocator>& a, matrix<T2, Allocator>& b)
	{
		if (!is_square_matrix(a) || a.rows() != b.rows())
		{
			throw std::invalid_argument{"Invalid dimensions"};
		}

		return pivot_elimination_impl(a, b);
	}

	export
	template<class T, class T2, std::size_t Rows, std::size_t Columns>
	auto pivot_elimination(matrix<T>& a, static_matrix<T2, Rows, Columns>& b)
	{
		if (!is_square_matrix(a) || a.rows() != b.rows())
		{
			throw std::invalid_argument{"Invalid dimensions"};
		}

		return pivot_elimination_impl(a, b);
	}

	export
	template<class T, std::size_t Rows, class T2, std::size_t Columns>
	constexpr auto pivot_elimination(square_matrix<T, Rows>& a, static_matrix<T2, Rows, Columns>& b)
	{
		return pivot_elimination_impl(a, b);
	}

	export
	template<class T, std::size_t Rows, class T2, class Allocator>
	auto pivot_elimination(square_matrix<T, Rows>& a, matrix<T2, Allocator>& b)
	{
		if (a.rows() != b.rows())
		{
			throw std::invalid_argument{"Invalid dimensions"};
		}

		return pivot_elimination_impl(a, b);
	}

	export
	template<class T, class Allocator>
	auto pivot_elimination(matrix<T, Allocator>& a)
	{
		if (!is_square_matrix(a))
		{
			throw std::invalid_argument{"Invalid dimensions"};
		}

		no_b_matrix temp;
		return pivot_elimination_impl(a, temp);
	}

	export
	template<class T, std::size_t N>
	constexpr auto pivot_elimination(square_matrix<T, N>& a)
	{
		no_b_matrix temp;
		return pivot_elimination_impl(a, temp);
	}
}