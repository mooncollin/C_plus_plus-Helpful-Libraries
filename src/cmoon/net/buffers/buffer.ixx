export module cmoon.net.buffers.buffer;

import <cstddef>;
import <memory>;
import <array>;
import <vector>;
import <string>;
import <string_view>;
import <span>;

import cmoon.net.buffers.mutable_buffer;
import cmoon.net.buffers.const_buffer;

namespace cmoon::net
{
	export
	[[nodiscard]] constexpr mutable_buffer buffer(void* p, std::size_t n) noexcept
	{
		return mutable_buffer{p, n};
	}

	export
	[[nodiscard]] constexpr const_buffer buffer(const void* p, std::size_t n) noexcept
	{
		return const_buffer{p, n};
	}

	export
	[[nodiscard]] constexpr mutable_buffer buffer(const mutable_buffer& b) noexcept
	{
		return b;
	}

	export
	[[nodiscard]] constexpr mutable_buffer buffer(const mutable_buffer& b, std::size_t n) noexcept
	{
		return mutable_buffer{b.data(), std::min(b.size(), n)};
	}

	export
	[[nodiscard]] constexpr const_buffer buffer(const const_buffer& b) noexcept
	{
		return b;
	}

	export
	[[nodiscard]] constexpr const_buffer buffer(const const_buffer& b, std::size_t n) noexcept
	{
		return const_buffer{b.data(), std::min(b.size(), n)};
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr mutable_buffer buffer(T(&data)[N]) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr const_buffer buffer(const T(&data)[N]) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr mutable_buffer buffer(std::array<T, N>& data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr const_buffer buffer(const std::array<T, N>& data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] mutable_buffer buffer(std::vector<T, Allocator>& data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] const_buffer buffer(const std::vector<T, Allocator>& data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class CharT, class Traits, class Allocator>
	[[nodiscard]] mutable_buffer buffer(std::basic_string<CharT, Traits, Allocator>& data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class CharT, class Traits, class Allocator>
	[[nodiscard]] const_buffer buffer(const std::basic_string<CharT, Traits, Allocator>& data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class CharT, class Traits>
	[[nodiscard]] constexpr const_buffer buffer(std::basic_string_view<CharT, Traits> data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, std::size_t Extent>
	[[nodiscard]] constexpr mutable_buffer buffer(std::span<T, Extent> data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, std::size_t Extent>
	[[nodiscard]] constexpr const_buffer buffer(std::span<const T, Extent> data) noexcept
	{
		const auto begin = std::begin(data);
		const auto end = std::end(data);
		return buffer(begin != end ? std::addressof(*begin) : nullptr,
					  (end - begin) * sizeof(*begin));
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr mutable_buffer buffer(T(&data)[N], std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr const_buffer buffer(const T(&data)[N], std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr mutable_buffer buffer(std::array<T, N>& data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, std::size_t N>
	[[nodiscard]] constexpr const_buffer buffer(const std::array<T, N>& data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] mutable_buffer buffer(std::vector<T, Allocator>& data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, class Allocator>
	[[nodiscard]] const_buffer buffer(const std::vector<T, Allocator>& data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class CharT, class Traits, class Allocator>
	[[nodiscard]] mutable_buffer buffer(std::basic_string<CharT, Traits, Allocator>& data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class CharT, class Traits, class Allocator>
	[[nodiscard]] const_buffer buffer(const std::basic_string<CharT, Traits, Allocator>& data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class CharT, class Traits>
	[[nodiscard]] constexpr const_buffer buffer(std::basic_string_view<CharT, Traits> data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, std::size_t Extent>
	[[nodiscard]] constexpr mutable_buffer buffer(std::span<T, Extent> data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}

	export
	template<class T, std::size_t Extent>
	[[nodiscard]] constexpr const_buffer buffer(std::span<const T, Extent> data, std::size_t n) noexcept
	{
		return buffer(buffer(data), n);
	}
}