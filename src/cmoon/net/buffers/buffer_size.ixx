export module cmoon.net.buffers.buffer_size;

import <cstddef>;
import <numeric>;
import <functional>;

import cmoon.net.buffers.buffer_sequence_begin;
import cmoon.net.buffers.buffer_sequence_end;

namespace cmoon::net
{
	template<class ConstBufferSequence>
	inline constexpr std::size_t buffer_size(const ConstBufferSequence& buffers) noexcept
	{
		return std::transform_reduce(buffer_sequence_begin(buffers),
									 buffer_sequence_end(buffers),
									 std::size_t{0}, std::plus{}, [](const auto& buf) { return const_buffer{*buffer}.size(); });
	}
}