export module cmoon.net.buffers.const_buffer;

import <cstddef>;
import <algorithm>;
import <memory>;

namespace cmoon::net
{
	export
	class const_buffer
	{
		public:
			constexpr const_buffer() noexcept
				: data_{nullptr}, size_{0} {}

			constexpr const_buffer(const void* p, std::size_t n) noexcept
				: data_{p}, size_{n} {}

			constexpr const_buffer(const mutable_buffer& b) noexcept
				: data_{b.data()}, size_{b.size()} {}

			constexpr const void* data() const noexcept
			{
				return data_;
			}

			constexpr std::size_t size() const noexcept
			{
				return size_;
			}

			constexpr const_buffer& operator+=(std::size_t n) noexcept
			{
				const auto min = std::min(n, size_);
				data_ = static_cast<const char*>(data_) + min;
				size_ -= min;

				return *this;
			}
		private:
			const void* data_;
			std::size_t size_;
	};

	export
	constexpr const const_buffer* buffer_sequence_begin(const const_buffer& b) noexcept
	{
		return std::addressof(b);
	}

	export
	constexpr const const_buffer* buffer_sequence_end(const const_buffer& b) noexcept
	{
		return std::addressof(b) + 1;
	}

	export
	constexpr const_buffer operator+(const const_buffer& b, std::size_t n) noexcept
	{
		const auto min = std::min(n, b.size());
		return const_buffer{static_cast<char*>(b.data()) + min, b.size() - min};
	}

	export
	constexpr const_buffer operator+(std::size_t n, const const_buffer& b) noexcept
	{
		return b + n;
	}
}