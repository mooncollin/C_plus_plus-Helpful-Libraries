export module cmoon.linear.back_substitution;

import <cstddef>;
import <numeric>;
import <stdexcept>;
import <span>;

import cmoon.linear.is_matrix;
import cmoon.linear.matrix;
import cmoon.linear.static_matrix;

namespace cmoon::linear
{
	template<matrix_type Matrix1, class T, std::size_t Extent, std::size_t RowSize, class T2, std::size_t Extent2, std::size_t RowSize2>
	constexpr void back_substitution_impl(const Matrix1& a, const column<T, Extent, RowSize>& b, const column<T2, Extent2, RowSize2>& c)
	{
		auto b_it = b.rbegin();
		auto c_it = c.rbegin();
		const auto d = main_diagonal(a);
		auto d_it = d.rbegin();

		const auto a_rows = a.get_rows_slice();

		for (auto row_it = std::rbegin(a_rows); row_it != std::rend(a_rows); ++row_it)
		{
			const auto current_row = *row_it;
			const auto s = *b_it - std::transform_reduce(std::begin(current_row),
															std::end(current_row),
															std::begin(c), T2{0});

			++b_it;
			if (*d_it)
			{
				*c_it = s / *d_it;
				++c_it;
			}
			else
			{
				throw std::runtime_error{"Divide by zero"};
			}

			++d_it;
		}
	}

	export
	template<class T, class Allocator, class T2, std::size_t Extent, std::size_t RowSize>
	[[nodiscard]] void back_substitution(const matrix<T, Allocator>& a, const column<T2, Extent, RowSize>& b)
	{
		if (b.size() != a.rows())
		{
			throw std::invalid_argument{"Invalid dimensions"};
		}

		matrix<T> c {b.size(), 1};

		back_substitution_impl(a, b, c.get_column(0));
	}

	export
	template<class T, std::size_t N, class T2, std::size_t Extent, std::size_t RowSize>
	[[nodiscard]] constexpr void back_substitution(const square_matrix<T, N>& a, const column<T2, Extent, RowSize>& b)
	{
		if constexpr (Extent != std::dynamic_extent)
		{
			static_assert(N == Extent);
		}
		else
		{
			if (b.size() != N)
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}
		}
		static_matrix<T, N, 1> c;
		
		back_substitution_impl(a, b, c.get_column(0));
	}

	export
	template<class T, class Allocator, class T2, class T3, std::size_t Extent1, std::size_t RowSize1, std::size_t Extent2, std::size_t RowSize2>
	[[nodiscard]] void back_substitution(const matrix<T, Allocator>& a, const column<T2, Extent1, RowSize1>& b, const column<T3, Extent2, RowSize2>& c)
	{
		if (b.size() != a.rows() || b.size() != c.size())
		{
			throw std::invalid_argument{"Invalid dimensions"};
		}

		back_substitution_impl(a, b, c);
	}

	export
	template<class T, std::size_t N, class T2, class T3, std::size_t Extent1, std::size_t RowSize1, std::size_t Extent2, std::size_t RowSize2>
	[[nodiscard]] constexpr void back_substitution(const square_matrix<T, N>& a, const column<T2, Extent1, RowSize1>& b, const column<T3, Extent2, RowSize2>& c)
	{
		if constexpr (Extent1 != std::dynamic_extent &&
						Extent2 != std::dynamic_extent)
		{
			static_assert(Extent1 == N);
			static_assert(Extent2 == N);
		}
		else if constexpr (Extent1 != std::dynamic_extent)
		{
			static_assert(Extent1 == N);
			if (c.size() != N)
			{
				throw std::invalid_argument{ "Invalid dimensions" };
			}
		}
		else if constexpr (Extent2 != std::dynamic_extent)
		{
			static_assert(Extent2 == N);
			if (b.size() != N)
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}
		}
		else
		{
			if (c.size() != N || b.size() != N)
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}
		}

		back_substitution_impl(a, b, c);
	}
}