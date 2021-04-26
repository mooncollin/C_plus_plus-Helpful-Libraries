export module cmoon.linear.transpose;

import <cstddef>;
import <type_traits>;
import <algorithm>;

import cmoon.linear.matrix;
import cmoon.linear.static_matrix;

namespace cmoon::linear
{
	export
	template<class T, class Allocator>
	[[nodiscard]] matrix<T, Allocator> transpose(const matrix<T, Allocator>& m)
	{
		const auto rows = m.rows();
		const auto columns = m.cols();
			
		matrix<T, Allocator> out{columns, rows};
		for (std::size_t i {0}; i < columns; ++i)
		{
			const auto row = out.get_row(i);
			const auto column = m.get_column(i);
			std::copy(std::begin(column), std::end(column), std::begin(row));
		}
		return out;
	}

	export
	template<class T, std::size_t Rows, std::size_t Cols>
	[[nodiscard]] constexpr static_matrix<T, Cols, Rows> transpose(const static_matrix<T, Rows, Cols>& m) noexcept
	{
		static_matrix<T, Cols, Rows> out;
		if (std::is_constant_evaluated())
		{
			for (std::size_t i {0}; i < Rows; ++i)
			{
				for (std::size_t j {0}; j < Cols; ++j)
				{
					out(j, i) = m(i, j);
				}
			}
		}
		else
		{
			for (std::size_t i {0}; i < Rows; ++i)
			{
				const auto row = out.get_row(i);
				const auto column = m.get_column(i);
				std::copy(std::begin(column), std::end(column), std::begin(row));
			}
		}

		return out;
	}
}