export module cmoon.net.buffers.dynamic_string_buffer;

import <cstddef>;
import <string>;
import <algorithm>;

import cmoon.net.buffers.const_buffer;
import cmoon.net.buffers.mutable_buffer;
import cmoon.net.buffers.buffer;

namespace cmoon::net
{
	export
	template<class CharT, class Traits, class Allocator>
		requires(sizeof(CharT) == 1)
	class dynamic_string_buffer
	{
		public:
			using const_buffers_type = const_buffer;
			using mutable_buffers_type = mutable_buffer;

			explicit dynamic_string_buffer(std::basic_string<CharT, Traits, Allocator>& str) noexcept
				: str_{str}, max_size_{str.max_size()} {}

			dynamic_string_buffer(std::basic_string<CharT, Traits, Allocator>& str, std::size_t maximum_size) noexcept
				: str_{str}, max_size_{maximum_size} {}

			[[nodiscard]] std::size_t size() const noexcept
			{
				return std::min(str_.size(), max_size_);
			}

			[[nodiscard]] std::size_t max_size() const noexcept
			{
				return max_size_;
			}

			[[nodiscard]] std::size_t capacity() const noexcept
			{
				return std::min(str_.capacity(), max_size_);
			}

			[[nodiscard]] const_buffers_type data(std::size_t pos, std::size_t n) const noexcept
			{
				return buffer(buffer(str_, max_size_) + pos, n);
			}

			[[nodiscard]] mutable_buffers_type data(std::size_t pos, std::size_t n) noexcept
			{
				return buffer(buffer(str_, max_size_) + pos, n);
			}

			void grow(std::size_t n)
			{
				if (size() > max_size() || max_size() - size() < n)
				{
					throw std::length_error {};
				}

				str_.resize(size() + n);
			}

			void shrink(std::size_t n)
			{
				str_.resize(n > size() ? 0 : size() - n);
			}

			void consume(std::size_t n)
			{
				str_.erase(0, std::min(n, size()));
			}
		private:
			std::basic_string<CharT, Traits, Allocator>& str_;
			const std::size_t max_size_;
	};

	template<class CharT, class Traits, class Allocator>
	[[nodiscard]] dynamic_string_buffer<T, Allocator> dynamic_buffer(std::basic_string<CharT, Traits, Allocator>& str) noexcept
	{
		return dynamic_string_buffer<CharT, Traits, Allocator>{str};
	}

	template<class CharT, class Traits, class Allocator>
	[[nodiscard]] dynamic_string_buffer<T, Allocator> dynamic_buffer(std::basic_string<CharT, Traits, Allocator>& str, std::size_t n) noexcept
	{
		return dynamic_string_buffer<CharT, Traits, Allocator>{str, n};
	}
}