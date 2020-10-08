#pragma once

#include <type_traits>
#include <execution>
#include <functional>

#include "collin/linear/matrix.hpp"
#include "collin/math.hpp"
#include "collin/iterator.hpp"

namespace collin
{
	namespace linear
	{
		template<class T>
		[[nodiscard]] matrix<T> transpose(const matrix<T>& m)
		{
			const auto rows = m.rows();
			const auto columns = m.cols();
			
			matrix<T> out{columns, rows};
			for (std::size_t i {0}; i < columns; ++i)
			{
				const auto row = out.get_row(i);
				const auto column = m.get_column(i);
				std::copy(std::begin(column), std::end(column), std::begin(row));
			}
			return out;
		}

		template<class T, std::size_t Rows, std::size_t Cols>
		[[nodiscard]] constexpr fixed_matrix<T, Cols, Rows> transpose(const fixed_matrix<T, Rows, Cols>& m) noexcept
		{
			fixed_matrix<T, Cols, Rows> out;
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

		template<class T>
		[[nodiscard]] matrix<T> sub_matrix(const matrix<T>& m, const std::size_t r, const std::size_t c)
		{
			const auto rows = m.rows();
			const auto columns = m.cols();

			matrix<T> result {rows - 1, columns - 1};
			auto it = std::begin(result);
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

		template<class T, std::size_t Rows, std::size_t Columns>
		[[nodiscard]] constexpr fixed_matrix<T, Rows - 1, Columns - 1> sub_matrix(const fixed_matrix<T, Rows, Columns>& m, const std::size_t r, const std::size_t c) noexcept
		{
			fixed_matrix<T, Rows - 1, Columns - 1> result;
			auto it = std::begin(result);
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

		template<class T>
		[[nodiscard]] T trace(const matrix<T>& m) noexcept
		{
			return main_diagonal(m).sum();
		}

		template<class T, std::size_t Rows, std::size_t Cols>
		[[nodiscard]] constexpr T trace(const fixed_matrix<T, Rows, Cols>& m) noexcept
		{
			return main_diagonal(m).sum();
		}

		template<class Rep>
		[[nodiscard]] Rep determinant(const matrix<Rep>& m)
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
			
			matrix<long double> elim {size, size};
			std::copy(std::begin(m), std::end(m), std::begin(elim));
			details::no_b_matrix temp;
			const auto operations = details::pivot_elimination(elim, temp);
			const auto p = main_diagonal(elim).product();
			if constexpr (std::is_integral_v<Rep>)
			{
				return static_cast<Rep>(std::round(p * operations));
			}
			else
			{
				return static_cast<Rep>(p * operations);
			}
		}

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
				details::no_b_matrix temp;
				const auto operations = details::pivot_elimination(elim, temp);
				const auto p = main_diagonal(elim).product();
				if constexpr (std::is_integral_v<Rep>)
				{
					return static_cast<Rep>(std::round(p * operations));
				}
				else
				{
					return static_cast<Rep>(p * operations);
				}
			}
		}

		template<class T>
		[[nodiscard]] T cofactor(const matrix<T>& m, const std::size_t p, const std::size_t q)
		{
			const auto sub = sub_matrix(m, p, q);
			const auto det = determinant(sub);
			return collin::math::is_even(p + q) ? det : -det;
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr T cofactor(const square_matrix<T, S>& m, const std::size_t p, const std::size_t q) noexcept
		{
			const auto sub = sub_matrix(m, p, q);
			const auto det = determinant(sub);
			return collin::math::is_even(p + q) ? det : -det;
		}

		template<class T>
		[[nodiscard]] matrix<T> adjugate(const matrix<T>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the adjugate of a square matrix"};
			}

			const auto size {m.rows()};

			matrix<T> out{size, size};

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

		template<class T>
		[[nodiscard]] matrix<T> inverse(const matrix<T>& m)
		{
			auto mutable_m = m;
			auto identity = identity_matrix<T>(m.rows());
			return gaussian_elimination(mutable_m, identity);
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr square_matrix<T, S> inverse(const square_matrix<T, S>& m)
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
				auto mutable_m = m;
				auto identity = identity_matrix<T, S>();
				return gaussian_elimination(mutable_m, identity);
			}
		}

		template<class T>
		[[nodiscard]] bool is_orthogonal(const matrix<T>& m)
		{
			return inverse(m) == transpose(m);
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr bool is_orthogonal(const square_matrix<T, S>& m)
		{
			return inverse(m) == transpose(m);
		}

		namespace details
		{
			class no_b_matrix {};

			template<matrix_type Matrix1, class Other>
			constexpr typename Matrix1::value_type pivot_elimination(Matrix1& a, Other& b)
			{
				constexpr auto b_exists = !std::is_same_v<Other, no_b_matrix>;

				using value_t = typename Matrix1::value_type;
				const auto n = a.rows();
				value_t operations {1};

				for (std::size_t j {0}; j < n; ++j)
				{
					auto pivot_row_i = j;

					for (std::size_t k {j + 1}; k < n; ++k)
					{
						if (collin::math::abs(a(k, j)) > collin::math::abs(a(pivot_row_i, j)))
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

					const auto pivot_row = a.get_row(j);
					const auto pivot = pivot_row[j];

					if (pivot == 0)
					{
						throw std::invalid_argument{"Pivot cannot be zero"};
					}

					for (std::size_t i {j + 1}; i < n; ++i)
					{
						const auto below_pivot_row = a.get_row(i);
						const auto mult = below_pivot_row[j] / pivot;
						const auto func = [mult](auto lhs, auto rhs) { return (lhs * -mult) + rhs; };
						std::transform(std::begin(pivot_row),
									   std::end(pivot_row),
									   std::begin(below_pivot_row),
									   std::begin(below_pivot_row),
									   func);

						if constexpr (b_exists)
						{
							const auto b_pivot_row = b.get_row(j);
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

			template<matrix_type Matrix1, class T, std::size_t Extent, std::size_t RowSize, class T2, std::size_t Extent2, std::size_t RowSize2>
			constexpr void back_substitution_impl(const Matrix1& a, const column<T, Extent, RowSize>& b, const column<T2, Extent2, RowSize2>& c)
			{
				auto b_it = b.rbegin();
				auto c_it = c.rbegin();
				const auto d = main_diagonal(a);
				auto d_it = d.rbegin();

				const auto a_rows = a.get_rows_slice();

				for (const auto current_row : collin::iterators::reverse(a_rows))
				{
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
						details::back_substitution_impl(a, b_column, c_column);
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
							details::back_substitution_impl(mat, b_column, c_column);
						};
						futures.push_back(std::async(std::launch::async, func, std::cref(a)));
					}
						
					for (auto& f : futures)
					{
						f.wait();
					}
				}
			}
		}

		template<class T, class T2>
		auto pivot_elimination(matrix<T>& a, matrix<T2>& b)
		{
			if (!is_square_matrix(a) || a.rows() != b.rows())
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}

			return details::pivot_elimination(a, b);
		}

		template<class T, class T2, std::size_t Rows, std::size_t Columns>
		auto pivot_elimination(matrix<T>& a, fixed_matrix<T2, Rows, Columns>& b)
		{
			if (!is_square_matrix(a) || a.rows() != b.rows())
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}

			return details::pivot_elimination(a, b);
		}

		template<class T, std::size_t Rows, class T2, std::size_t Columns>
		constexpr auto pivot_elimination(square_matrix<T, Rows>& a, fixed_matrix<T2, Rows, Columns>& b)
		{
			return details::pivot_elimination(a, b);
		}

		template<class T, std::size_t Rows, class T2>
		auto pivot_elimination(square_matrix<T, Rows>& a, matrix<T2>& b)
		{
			if (a.rows() != b.rows())
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}

			return details::pivot_elimination(a, b);
		}

		template<class T>
		auto pivot_elimination(matrix<T>& a)
		{
			if (!is_square_matrix(a))
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}

			details::no_b_matrix temp;
			return details::pivot_elimination(a, temp);
		}

		template<class T, std::size_t N>
		constexpr auto pivot_elimination(square_matrix<T, N>& a)
		{
			details::no_b_matrix temp;
			return details::pivot_elimination(a, temp);
		}

		template<class T, class T2, std::size_t Extent, std::size_t RowSize>
		[[nodiscard]] auto back_substitution(const matrix<T>& a, const column<T2, Extent, RowSize>& b)
		{
			if (b.size() != a.rows())
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}

			matrix<T> c {b.size(), 1};

			return details::back_substitution_impl(a, b, c.get_column(0));
		}

		template<class T, std::size_t N, class T2, std::size_t Extent, std::size_t RowSize>
		[[nodiscard]] constexpr auto back_substitution(const square_matrix<T, N>& a, const column<T2, Extent, RowSize>& b)
		{
			if constexpr (Extent != dynamic_extent)
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
			fixed_matrix<T, N, 1> c;
			return details::back_substitution_impl(a, b, c.get_column(0));
		}

		template<class T, class T2, class T3, std::size_t Extent1, std::size_t RowSize1, std::size_t Extent2, std::size_t RowSize2>
		[[nodiscard]] void back_substitution(const matrix<T>& a, const column<T2, Extent1, RowSize1>& b, const column<T3, Extent2, RowSize2>& c)
		{
			if (b.size() != a.rows() || b.size() != c.size())
			{
				throw std::invalid_argument{"Invalid dimensions"};
			}

			details::back_substitution_impl(a, b, c);
		}

		template<class T, std::size_t N, class T2, class T3, std::size_t Extent1, std::size_t RowSize1, std::size_t Extent2, std::size_t RowSize2>
		[[nodiscard]] constexpr void back_substitution(const square_matrix<T, N>& a, const column<T2, Extent1, RowSize1>& b, const column<T3, Extent2, RowSize2>& c)
		{
			if constexpr (Extent1 != dynamic_extent &&
						  Extent2 != dynamic_extent)
			{
				static_assert(Extent1 == N);
				static_assert(Extent2 == N);
			}
			else if constexpr (Extent1 != dynamic_extent)
			{
				static_assert(Extent1 == N);
				if (c.size() != N)
				{
					throw std::invalid_argument{ "Invalid dimensions" };
				}
			}
			else if constexpr (Extent2 != dynamic_extent)
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

			return details::back_substitution_impl(a, b, c);
		}

		template<class T, class T2>
		matrix<T> gaussian_elimination(matrix<T>& a, matrix<T2>& b)
		{
			const auto columns = std::min(a.cols(), b.cols());
			matrix<T> result{a.rows(), columns};
			details::gaussian_elimination_impl(a, b, result);

			return result;
		}

		template<class T, class T2, std::size_t Rows, std::size_t Columns>
		matrix<T> gaussian_elimination(matrix<T>& a, fixed_matrix<T2, Rows, Columns>& b)
		{
			const auto columns = std::min(a.cols(), b.cols());
			matrix<T> result{a.rows(), columns};
			details::gaussian_elimination_impl(a, b, result);

			return result;
		}

		template<class T, class T2, std::size_t Rows>
		matrix<T> gaussian_elimination(square_matrix<T, Rows>& a, matrix<T2>& b)
		{
			const auto columns = std::min(a.cols(), b.cols());
			matrix<T> result{a.rows(), columns};
			details::gaussian_elimination_impl(a, b, result);

			return result;
		}

		template<class T, std::size_t Rows, class T2, std::size_t Columns>
		fixed_matrix<T, Rows, Columns> gaussian_elimination(square_matrix<T, Rows>& a, fixed_matrix<T2, Rows, Columns>& b)
		{
			constexpr auto columns = std::min(Rows, Columns);
			fixed_matrix<T, Rows, columns> result;
			details::gaussian_elimination_impl(a, b, result);

			return result;
		}
	}
}