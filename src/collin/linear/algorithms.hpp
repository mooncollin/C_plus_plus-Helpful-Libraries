#pragma once

#include <type_traits>

#include "collin/linear/matrix.hpp"
#include "collin/math.hpp"

namespace collin
{
	namespace linear
	{
		template<class T>
		[[nodiscard]] matrix<T> transpose(const matrix<T>& m)
		{
			matrix<T> out{m.cols(), m.rows()};

			for (std::size_t i {0}; i < m.rows(); ++i)
			{
				for (std::size_t j {0}; j < m.cols(); ++j)
				{
					out(j, i) = m(i, j);
				}
			}

			return out;
		}

		template<class T, std::size_t Rows, std::size_t Cols>
		[[nodiscard]] constexpr fixed_matrix<T, Cols, Rows> transpose(const fixed_matrix<T, Rows, Cols>& m) noexcept
		{
			fixed_matrix<T, Cols, Rows> out;

			for (std::size_t i {0}; i < Rows; ++i)
			{
				for (std::size_t j {0}; j < Cols; ++j)
				{
					out(j, i) = m(i, j);
				}
			}

			return out;
		}

		template<class T>
		[[nodiscard]] matrix<T> cofactor(const matrix<T>& m, std::size_t p, std::size_t q)
		{
			if(!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the cofactor of a square matrix"};
			}

			const auto size = m.rows();

			matrix<T> out{size - 1, size - 1};
			std::size_t i {0};
			std::size_t j {0};

			for(std::size_t row {0}; row < size; ++row)
			{
				for(std::size_t col {0}; col < size; ++col)
				{
					if(row != p && col != q)
					{
						out(i, j++) = m(row, col);

						if(j == size - 1)
						{
							j = 0;
							++i;
						}
					}
				}
			}

			return out;
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr square_matrix<T, S - 1> cofactor(const square_matrix<T, S>& m, std::size_t p, std::size_t q) noexcept
		{
			square_matrix<T, S - 1> out;
			std::size_t i {0};
			std::size_t j {0};

			for(std::size_t row{0}; row < S; ++row)
			{
				for(std::size_t col{0}; col < S; ++col)
				{
					if(row != p && col != q)
					{
						out(i, j++) = m(row, col);

						if(j == S - 1)
						{
							j = 0;
							++i;
						}
					}
				}
			}

			return out;
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

			int sign {1};
			Rep result {0};

			for(std::size_t f {0}; f < size; ++f)
			{
				result += sign * m(0, f) * determinant(cofactor(m, 0, f));
				sign = -sign;
			}

			return result;
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
			else
			{
				int sign {1};
				Rep result {0};

				for(std::size_t f{0}; f < S; ++f)
				{
					result += sign * m(0, f) * determinant(cofactor(m, 0, f));
					sign = -sign;
				}

				return result;
			}
		}

		template<class T>
		[[nodiscard]] matrix<T> pivot_matrix(const matrix<T>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the pivot matrix of a square matrix"};
			}

			const auto size = m.rows();
			auto pivot = identity_matrix<T>(size);

			for (std::size_t j {0}; j < size; ++j)
			{
				std::size_t max_valued_row {0};
				for (std::size_t i {j}; i < size; ++i)
				{
					if (m(i, j) > m(max_valued_row, j))
					{
						max_valued_row = i;
					}
				}

				if (j != max_valued_row)
				{
					std::swap(pivot.get_row(j), pivot.get_row(max_valued_row));
				}
			}

			return pivot;
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr square_matrix<T, S> pivot_matrix(const square_matrix<T, S>& m) noexcept
		{
			auto pivot = identity_matrix<T, S>();

			for (std::size_t j {0}; j < S; ++j)
			{
				std::size_t max_valued_row {0};
				for (std::size_t i {j}; i < S; ++i)
				{
					if (m(i, j) > m(max_valued_row, j))
					{
						max_valued_row = i;
					}
				}

				if (j != max_valued_row)
				{
					std::swap(pivot.get_row(j), pivot.get_row(max_valued_row));
				}
			}

			return pivot;
		}

		template<class T>
		[[nodiscard]] std::tuple<matrix<T>, matrix<T>, matrix<T>>
			lu_decomposition(const matrix<T>& a) noexcept
		{
			auto p = pivot_matrix(a);
			matrix<T> l{a.rows(), a.cols()};
			matrix<T> u{a.rows(), a.cols()};
			auto pa = p * a;

			for(std::size_t j {0}; j < a.rows(); ++j)
			{
				l(j, j) = T{1};

				for(std::size_t i {0}; i < j + 1; ++i)
				{
					T sum {0};
					for(std::size_t k {0}; k < i; ++k)
					{
						sum += l(i, k) * u(k, j);
					}

					u(i, j) = pa(i, j) - sum;
				}

				for(std::size_t i {j}; i < a.rows(); ++i)
				{
					T sum {0};
					for(std::size_t k {0}; k < j; ++k)
					{
						sum += l(i, k) * u(k, j);
					}

					l(i, j) = (pa(i, j) - sum) / u(j, j);
				}
			}

			return {p, l, u};
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr std::tuple<square_matrix<T, S>, square_matrix<T, S>, square_matrix<T, S>>
			lu_decomposition(const square_matrix<T, S>& a) noexcept
		{
			square_matrix<T, S> l;
			square_matrix<T, S> u;
			auto p = pivot_matrix(a);
			auto pa = p * a;

			for(std::size_t j {0}; j < S; ++j)
			{
				l(j, j) = T{1};

				for(std::size_t i {0}; i < j + 1; ++i)
				{
					T sum{};
					for(std::size_t k {0}; k < i; ++k)
					{
						sum += l(i, k) * u(k, j);
					}

					u(i, j) = pa(i, j) - sum;
				}

				for(std::size_t i {j}; i < S; ++i)
				{
					T sum{};
					for(std::size_t k {0}; k < j; ++k)
					{
						sum += l(i, k) * u(k, j);
					}

					l(i, j) = (pa(i, j) - sum) / u(j, j);
				}
			}

			return {p, l, u};
		}

		template<class T>
		[[nodiscard]] matrix<T> adjugate(const matrix<T>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument{"can only find the adjoint of a square matrix"};
			}

			const auto size {m.rows()};

			matrix<T> out{size, size};

			if(size == 1)
			{
				out(0, 0) = 1;
				return out;
			}
			else if(size == 2)
			{
				out(0, 0) = m(1, 1);
				out(1, 1) = m(0, 0);

				out(0, 1) = -m(0, 1);
				out(1, 0) = -m(1, 0);

				return out;
			}

			for(std::size_t i{0}; i < size; ++i)
			{
				for(std::size_t j{0}; j < size; ++j)
				{
					const auto sign = collin::math::is_even(i + j) ? 1 : -1;
					
					out(j, i) = sign * determinant(cofactor(m, i, j));
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
				for(std::size_t i{0}; i < S; ++i)
				{
					for(std::size_t j{0}; j < S; ++j)
					{
						const auto sign = collin::math::is_even(i + j) ? 1 : -1;
					
						out(j, i) = sign * determinant(cofactor(m, i, j));
					}
				}
			}

			return out;
		}

		template<class T>
		[[nodiscard]] matrix<T> inverse(const matrix<T>& m)
		{
			const auto det = determinant(m);
			if(det == 0)
			{
				throw std::invalid_argument{"cannot find the inverse of a singular matrix"};
			}

			const auto adj = adjugate(m);
			const auto size = m.rows();

			matrix<T> out{size, size};

			for(std::size_t i{0}; i < size; ++i)
			{
				for(std::size_t j{0}; j < size; ++j)
				{
					out(i, j) = adj(i, j) / det;
				}
			}

			return out;
		}

		template<class T, std::size_t S>
		[[nodiscard]] constexpr square_matrix<T, S> inverse(const square_matrix<T, S>& m)
		{
			const auto det = determinant(m);
			if(det == 0)
			{
				throw std::invalid_argument{"cannot find the inverse of a singular matrix"};
			}

			const auto adj = adjugate(m);

			square_matrix<T, S> out;

			for(std::size_t i{0}; i < S; ++i)
			{
				for(std::size_t j{0}; j < S; ++j)
				{
					out(i, j) = adj(i, j) / det;
				}
			}

			return out;
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
	}
}