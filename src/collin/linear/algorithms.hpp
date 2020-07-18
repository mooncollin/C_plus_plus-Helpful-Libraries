#pragma once

#include "collin/linear/matrix.hpp"

namespace collin
{
	namespace linear
	{
		template<class Matrix, typename = std::enable_if_t<
/* requires */	is_either_matrix_v<Matrix>
		>>
		constexpr bool is_square_matrix(const Matrix& m) noexcept
		{
			if constexpr (is_fixed_matrix_v<Matrix>)
			{
				return Matrix::num_rows == Matrix::num_columns; // Truely compile time
			}
			else
			{
				return m.rows() == m.cols();
			}
		}

		namespace details
		{
			template<class Matrix, class PermutationContainer, typename = std::enable_if_t<
	/* requires */ is_either_matrix_v<Matrix>
			>>
			constexpr typename Matrix::value_type determinant_helper(const Matrix& m, PermutationContainer& permutations)
			{
				const auto size_of_permutations = std::size(permutations);
				const auto number_of_permutations = collin::math::factorial(size_of_permutations);

				typename Matrix::value_type running_total {};
				bool add {false};
				short operator_counter {-1};

				for (std::uintmax_t p = 0; p < number_of_permutations; p++)
				{
					typename Matrix::value_type product {1};
					for (std::size_t i = 0; i < size_of_permutations; i++)
					{
						product *= m(i, permutations[i]);
					}

					if (operator_counter == -1)
					{
						running_total = running_total + product;
						operator_counter = 0;
					}
					else
					{
						if (add)
						{
							running_total = running_total + product;
						}
						else
						{
							running_total = running_total - product;
						}

						operator_counter++;
						if (operator_counter == 2)
						{
							add = !add;
							operator_counter = 0;
						}
					}

					collin::algorithms::next_permutation(std::begin(permutations), std::end(permutations));
				}

				return running_total;
			}
		}

		template<class Rep>
		Rep determinant(const matrix<Rep>& m)
		{
			if (!is_square_matrix(m))
			{
				throw std::invalid_argument {"can only find the determinant of a square matrix"};
			}

			if (m.rows() == 1)
			{
				return m(0, 0);
			}
			else if (m.rows() == 2)
			{
				return m(0, 0) * m(1, 1) -
					m(0, 1) * m(1, 0);
			}

			std::vector<std::size_t> permutations(m.rows());
			std::iota(std::begin(permutations), std::end(permutations), 0);

			return details::determinant_helper(m, permutations);
		}

		template<class Rep, std::size_t S>
		constexpr Rep determinant(const square_matrix<Rep, S>& m)
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

			auto permutations = collin::index_array<S>();
			return details::determinant_helper(m, permutations);
		}
	}
}