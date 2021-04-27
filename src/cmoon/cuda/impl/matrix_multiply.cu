#include <cstddef>
#include <utility>
#include <cmath>

#include <cuda_runtime.h>

namespace cmoon::cuda
{
	template<class T>
	__global__
	void matrix_multiply_kernel(int m, int n, int k, const T* const input1, const T* const input2, T* out) noexcept
	{
		const auto row {blockIdx.y * blockDim.y + threadIdx.y};
		const auto column {blockIdx.x * blockDim.x + threadIdx.x};

		if (row < m && column < k)
		{
			for (int i {0}; i < n; ++i)
			{
				out[row * k + column] += input1[row * n + i] * input2[i * k + column];
			}
		}
	}

	template<class T>
	__global__
	void square_matrix_multiply_kernel(int n, const T* const input1, const T* const input2, T* out) noexcept
	{
		constexpr unsigned int block_size {16};
		__shared__ int tile_a[block_size][block_size];
		__shared__ int tile_b[block_size][block_size];

		const auto row {blockIdx.y * block_size + threadIdx.y};
		const auto column {blockIdx.x * block_size + threadIdx.x};
		int tmp {0};

		for (int sub {0}; sub < gridDim.x; ++sub)
		{
			auto idx {row * n + sub * block_size + threadIdx.x};
			if (idx >= n * n)
			{
				tile_a[threadIdx.y][threadIdx.x] = 0;
			}
			else
			{
				tile_a[threadIdx.y][threadIdx.x] = input1[idx];
			}

			idx = (sub * block_size + threadIdx.y) * n + column;
			if (idx >= n * n)
			{
				tile_b[threadIdx.y][threadIdx.x] = 0;
			}
			else
			{
				tile_b[threadIdx.y][threadIdx.x] = input2[idx];
			}

			__syncthreads();

			for (int k {0}; k < block_size; ++k)
			{
				tmp += tile_a[threadIdx.y][k] * tile_b[k][threadIdx.x];
			}

			__syncthreads();
		}

		if (row < n && column < n)
		{
			out[row * n + column] = tmp;
		}
	}

	template<class T>
	void matrix_multiply_impl(int m, int n, int k, const T* const input1, const T* const input2, T* out) noexcept
	{
		constexpr unsigned int block_size {16};
		const auto grid_rows {(m + block_size - 1) / block_size};
		const auto grid_cols {(k + block_size - 1) / block_size};

		const ::dim3 dim_grid {grid_cols, grid_rows};
		const ::dim3 dim_block {block_size, block_size};

		if (m == n && n == k)
		{
			square_matrix_multiply_kernel<<<dim_grid, dim_block>>>(n, input1, input2, out);
		}
		else
		{
			matrix_multiply_kernel<<<dim_grid, dim_block>>>(m, n, k, input1, input2, out);
		}
	}

	namespace details
	{
		void dummy_template_instantiator()
		{
			matrix_multiply_impl<float>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<double>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<char>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<unsigned char>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<short>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<unsigned short>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<int>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<unsigned int>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<long>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<unsigned long>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<long long>(0, 0, 0, nullptr, nullptr, nullptr);
			matrix_multiply_impl<unsigned long long>(0, 0, 0, nullptr, nullptr, nullptr);
		}
	}
}