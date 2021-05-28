export module cmoon.net.buffers.dynamic_vector_buffer;

import <cstddef>;
import <vector>;
import <type_traits>;
import <stdexcept>;
import <algorithm>;

import cmoon.net.buffers.const_buffer;
import cmoon.net.buffers.mutable_buffer;
import cmoon.net.buffers.buffer;

namespace cmoon::net
{
	export
	template<class T, class Allocator>
		requires(sizeof(T) == 1 && (std::is_trivially_copyable_v<T> || std::is_standard_layout_v<T>))
	class dynamic_vector_buffer
	{
		public:
			using const_buffers_type = const_buffer;
			using mutable_buffers_type = mutable_buffer;

			explicit dynamic_vector_buffer(std::vector<T, Allocator>& vec) noexcept
				: vec_{vec}, size_{vec.size()}, max_size_{vec.max_size()} {}

			dynamic_vector_buffer(std::vector<T, Allocator>& vec, std::size_t maximum_size) noexcept
				: vec_{vec}, size_{vec.size()}, max_size_{maximum_size} {}

			[[nodiscard]] std::size_t size() const noexcept
			{
				return std::min(vec_.size(), max_size_);
			}

			[[nodiscard]] std::size_t max_size() const noexcept
			{
				return max_size_;
			}

			[[nodiscard]] std::size_t capacity() const noexcept
			{
				return std::min(vec_.capacity(), max_size_);
			}

			[[nodiscard]] const_buffers_type data(std::size_t pos, std::size_t n) const noexcept
			{
				return buffer(buffer(vec_, max_size_) + pos, n);
			}

			[[nodiscard]] mutable_buffers_type data(std::size_t pos, std::size_t n) noexcept
			{
				return buffer(buffer(vec_, max_size_) + pos, n);
			}

			void grow(std::size_t n)
			{
				if (size() > max_size() || max_size() - size() < n)
				{
					throw std::length_error{};
				}

				vec_.resize(size() + n);
			}

			void shrink(std::size_t n)
			{
				vec_.resize(n > size() ? 0 : size() - n);
			}

			void consume(std::size_t n)
			{
				vec_.erase(vec_.begin(), vec_.begin() + std::min(n, size()));
			}
		private:
			std::vector<T, Allocator>& vec_;
			const std::size_t max_size_;
	};

	template<class T, class Allocator>
	[[nodiscard]] dynamic_vector_buffer<T, Allocator> dynamic_buffer(std::vector<T, Allocator>& vec) noexcept
	{
		return dynamic_vector_buffer<T, Allocator>{vec};
	}

	template<class T, class Allocator>
	[[nodiscard]] dynamic_vector_buffer<T, Allocator> dynamic_buffer(std::vector<T, Allocator>& vec, std::size_t n) noexcept
	{
		return dynamic_vector_buffer<T, Allocator>{vec, n};
	}
}