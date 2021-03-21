#include <cstddef>
#include <utility>
#include <cmath>

#include <cuda_runtime.h>

namespace cmoon
{
	namespace cuda
	{
		template<class T>
		__global__
		void plus_ranges_kernel(int n, const T* const input1, const T* const input2, T* out)
		{
			const auto index = blockIdx.x * blockDim.x + threadIdx.x;
			const auto stride = blockDim.x * gridDim.x;
			for (auto i = index; i < n; i += stride)
			{
				out[i] = input1[i] + input2[i];
			}
		}

		template<class T>
		void plus_ranges_impl(int n, const T* const input1, const T* const input2, T* out)
		{
			constexpr auto blockSize = 256;
			const auto numBlocks = (n + blockSize - 1) / blockSize;
			plus_ranges_kernel<<<numBlocks, blockSize>>>(n, input1, input2, out);
		}

		namespace details
		{
			void dummy_template_instantiator()
			{
				plus_ranges_impl<float>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<double>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<char>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<unsigned char>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<short>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<unsigned short>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<int>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<unsigned int>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<long>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<unsigned long>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<long long>(0, nullptr, nullptr, nullptr);
				plus_ranges_impl<unsigned long long>(0, nullptr, nullptr, nullptr);
			}
		}
	}
}