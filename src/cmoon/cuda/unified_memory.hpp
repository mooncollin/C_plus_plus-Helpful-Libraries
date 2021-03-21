#pragma once

#include <memory>
#include <utility>
#include <type_traits>
#include <cassert>

#include <cuda_runtime.h>

namespace cmoon
{
	namespace cuda
	{
		enum class memory_attachment : unsigned int
		{
			global = cudaMemAttachGlobal,
			host = cudaMemAttachHost
		};

		template<class T>
		struct unified_memory_allocator
		{
			using value_type = T;
			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			using propagate_on_container_move_assignment = std::true_type;

			constexpr unified_memory_allocator() noexcept = default;
			constexpr unified_memory_allocator(memory_attachment a) noexcept
				: flags{a} {}

			template<class U>
			constexpr unified_memory_allocator(const unified_memory_allocator<U>& other) noexcept
				: flags{other.flags} {}
			
			[[nodiscard]] T* allocate(size_type n)
			{
				void* storage;

				const auto result = cudaMallocManaged(std::addressof(storage), sizeof(T) * n, static_cast<std::underlying_type_t<memory_attachment>>(flags));
				assert(("bug in unified_memory_allocator::allocate", result != cudaErrorInvalidValue));

				switch (result)
				{
					case cudaErrorMemoryAllocation:
						throw std::bad_alloc{};
					case cudaErrorNotSupported:
						throw std::system_error{cudaErrorNotSupported, std::generic_category()};
				}

				return reinterpret_cast<T*>(storage);
			}

			void deallocate(T* p, size_type) noexcept
			{
				cudaFree(p);
			}

			memory_attachment flags {memory_attachment::global};
		};

		template<class T1, class T2>
		[[nodiscard]] constexpr bool operator==(const unified_memory_allocator<T1>& lhs, const unified_memory_allocator<T2>& rhs) noexcept
		{
			return lhs.flags == rhs.flags;
		}

		template<class T1, class T2>
		[[nodiscard]] constexpr bool operator!=(const unified_memory_allocator<T1>& lhs, const unified_memory_allocator<T2>& rhs) noexcept
		{
			return !(lhs == rhs);
		}
	}
}