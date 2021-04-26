export module cmoon.linear.sub_matrix;

import <cstddef>;

import cmoon.linear.matrix;
import cmoon.linear.static_matrix;

namespace cmoon::linear
{
	export
	template<class T, class Allocator>
	[[nodiscard]] matrix<T, Allocator> sub_matrix(const matrix<T, Allocator>& m, const std::size_t r, const std::size_t c)
	{
		const auto rows = m.rows();
		const auto columns = m.cols();

		matrix<T, Allocator> result {rows - 1, columns - 1};
		auto it {std::begin(result)};
		for (std::size_t i {0}; i < rows; ++i)
		{
			if (i == r)
			{
				continue;
			}

			for (std::size_t j {0}; j < columns; ++j)
			{
				if (j == c)
				{
					continue;
				}

				*it = m(i, j);
				++it;
			}
		}

		return result;
	}

	export
	template<class T, std::size_t Rows, std::size_t Columns>
	[[nodiscard]] constexpr static_matrix<T, Rows - 1, Columns - 1> sub_matrix(const static_matrix<T, Rows, Columns>& m, const std::size_t r, const std::size_t c) noexcept
	{
		static_matrix<T, Rows - 1, Columns - 1> result;
		auto it {std::begin(result)};
		for (std::size_t i {0}; i < Rows; ++i)
		{
			if (i == r)
			{
				continue;
			}

			for (std::size_t j {0}; j < Columns; ++j)
			{
				if (j == c)
				{
					continue;
				}

				*it = m(i, j);
				++it;
			}
		}

		return result;
	}
}