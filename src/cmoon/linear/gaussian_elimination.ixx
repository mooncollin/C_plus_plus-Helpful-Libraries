export module cmoon.linear.gaussian_elimination;

import <algorithm>;
import <future>;

import cmoon.linear.is_matrix;
import cmoon.linear.matrix;
import cmoon.linear.static_matrix;
import cmoon.linear.square_matrix;
import cmoon.linear.pivot_elimination;
import cmoon.linear.back_substitution;

namespace cmoon::linear
{
	template<matrix_type Matrix1, matrix_type Matrix2, matrix_type Matrix3>
	void gaussian_elimination_impl(Matrix1& a, Matrix2& b, Matrix3& c)
	{
		const auto columns = std::min(a.cols(), b.cols());
		pivot_elimination(a, b);

		if (columns < 70)
		{
			for (std::size_t i {0}; i < columns; ++i)
			{
				const auto b_column = b.get_column(i);
				const auto c_column = c.get_column(i);
				back_substitution(a, b_column, c_column);
			}
		}
		else
		{
			std::vector<std::future<void>> futures;
			for (std::size_t i {0}; i < columns; ++i)
			{
				const auto b_column = b.get_column(i);
				const auto c_column = c.get_column(i);
				const auto func = [b_column, c_column](const Matrix1& mat) {
					back_substitution(mat, b_column, c_column);
				};
				futures.push_back(std::async(std::launch::async, func, std::cref(a)));
			}
						
			for (auto& f : futures)
			{
				f.wait();
			}
		}
	}

	export
	template<class T, class T2, class Allocator>
	matrix<T, Allocator> gaussian_elimination(matrix<T, Allocator>& a, matrix<T2, Allocator>& b)
	{
		const auto columns = std::min(a.cols(), b.cols());
		matrix<T, Allocator> result{a.rows(), columns};
		gaussian_elimination_impl(a, b, result);

		return result;
	}

	export
	template<class T, class Allocator, class T2, std::size_t Rows, std::size_t Columns>
	matrix<T, Allocator> gaussian_elimination(matrix<T, Allocator>& a, static_matrix<T2, Rows, Columns>& b)
	{
		const auto columns = std::min(a.cols(), b.cols());
		matrix<T, Allocator> result{a.rows(), columns};
		gaussian_elimination_impl(a, b, result);

		return result;
	}

	export
	template<class T, class Allocator, class T2, std::size_t Rows>
	matrix<T, Allocator> gaussian_elimination(square_matrix<T, Rows>& a, matrix<T2, Allocator>& b)
	{
		const auto columns = std::min(a.cols(), b.cols());
		matrix<T, Allocator> result{a.rows(), columns};
		gaussian_elimination_impl(a, b, result);

		return result;
	}

	export
	template<class T, std::size_t Rows, class T2, std::size_t Columns>
	static_matrix<T, Rows, Columns> gaussian_elimination(square_matrix<T, Rows>& a, static_matrix<T2, Rows, Columns>& b)
	{
		constexpr auto columns = std::min(Rows, Columns);
		static_matrix<T, Rows, columns> result;
		gaussian_elimination_impl(a, b, result);

		return result;
	}
}