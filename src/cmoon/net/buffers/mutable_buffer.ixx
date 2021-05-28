export module cmoon.net.buffers.mutable_buffer;

import <cstddef>;
import <algorithm>;
import <memory>;

namespace cmoon::net
{
	export
	class mutable_buffer
	{
		public:
			constexpr mutable_buffer() noexcept
				: data_{nullptr}, size_{0} {}

			constexpr mutable_buffer(void* p, std::size_t n) noexcept
				: data_{p}, size_{n} {}

			constexpr void* data() const noexcept
			{
				return data_;
			}

			constexpr std::size_t size() const noexcept
			{
				return size_;
			}

			constexpr mutable_buffer& operator+=(std::size_t n) noexcept
			{
				const auto min = std::min(n, size_);
				data_ = static_cast<char*>(data_) + min;
				size_ -= min;

				return *this;
			}
		private:
			void* data_;
			std::size_t size_;
	};

	export
	constexpr const mutable_buffer* buffer_sequence_begin(const mutable_buffer& b) noexcept
	{
		return std::addressof(b);
	}

	export
	constexpr const mutable_buffer* buffer_sequence_end(const mutable_buffer& b) noexcept
	{
		return std::addressof(b) + 1;
	}

	export
	constexpr mutable_buffer operator+(const mutable_buffer& b, std::size_t n) noexcept
	{
		const auto min = std::min(n, b.size());
		return mutable_buffer{static_cast<char*>(b.data()) + min, b.size() - min};
	}

	export
	constexpr mutable_buffer operator+(std::size_t n, const mutable_buffer& b) noexcept
	{
		return b + n;
	}
}