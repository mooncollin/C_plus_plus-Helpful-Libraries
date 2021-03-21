#pragma once

#include <cstddef>
#include <iterator>
#include <utility>
#include <numeric>
#include <algorithm>
#include <functional>
#include <cassert>

#include "cmoon/cuda/details/add.hpp"
#include "cmoon/cuda/details/matrix_multiply.hpp"
#include "cmoon/cuda/unified_memory.hpp"
#include "cmoon/linear/matrix.hpp"

namespace cmoon
{
	namespace cuda
	{
		template<class T>
		constexpr inline void plus_ranges(std::size_t n, const T* const input1, const T* const input2, T* out)
		{
			if (std::is_constant_evaluated())
			{
				for (std::size_t i {0}; i < n; ++i)
				{
					out[i] = input1[i] + input2[i];
				}
			}
			else
			{
				assert(std::in_range<int>(n));
				plus_ranges_impl(static_cast<int>(n), input1, input2, out);
			}
		}

		template<class T>
		constexpr void matrix_multiply(std::size_t m, std::size_t n, std::size_t k, const T* const input1, const T* const input2, T* out) noexcept
		{
			if (std::is_constant_evaluated())
			{
				for (std::size_t row {0}; row < m; ++row)
				{
					for (std::size_t col {0}; col < k; ++col)
					{
						for (std::size_t i {0}; i < n; ++i)
						{
							out[row * n + col] += input1[row * n + i] * input2[i * k + col];
						}
					}
				}
			}
			else
			{
				assert(std::in_range<int>(m));
				assert(std::in_range<int>(n));
				assert(std::in_range<int>(k));
				matrix_multiply_impl(static_cast<int>(m), static_cast<int>(n), static_cast<int>(k), input1, input2, out);
			}
		}

		template<class T>
		[[nodiscard]] linear::matrix<T, unified_memory_allocator<T>> matrix_multiply(const linear::matrix<T, unified_memory_allocator<T>>& lhs, const linear::matrix<T, unified_memory_allocator<T>>& rhs) noexcept
		{
			linear::matrix<T, unified_memory_allocator<T>> out {lhs.rows(), rhs.cols()};
			matrix_multiply(lhs.rows(), lhs.cols(), rhs.cols(), lhs.data(), rhs.data(), out.data());
			cudaDeviceSynchronize();
			return out;
		}
	}
}