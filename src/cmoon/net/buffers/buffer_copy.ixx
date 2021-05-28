export module cmoon.net.buffers.buffer_copy;

import <cstddef>;
import <algorithm>;

import cmoon.net.buffers.buffer_sequence_begin;
import cmoon.net.buffers.buffer_sequence_end;

namespace cmoon::net
{
	export
	template<class MutableBufferSequence, class ConstBufferSequence>
	constexpr std::size_t buffer_copy(const MutableBufferSequence& dest, const ConstBufferSequence& source) noexcept
	{
		std::size_t amount_written {0};

		auto dest_i {buffer_sequence_begin(dest)};
		const auto dest_end {buffer_sequence_end(dest)};

		auto source_i {buffer_sequence_begin(source)};
		const auto source_end {buffer_sequence_end(source)};

		while (dest_i != dest_end &&
			   source_i != source_end)
		{
			const auto amount_to_write = std::min(dest_i->size(), source_i->size());
			std::copy_n(source_i->data(), amount_to_write, dest_i->data());
			amount_written += amount_to_write;
			*dest_i += amount_to_write;
			*source_i += amount_to_write;

			if (dest_i->size() == 0)
			{
				++dest_i;
			}

			if (source_i->size() == 0)
			{
				++source_i;
			}
		}

		return amount_written;
	}
	
	export
	template<class MutableBufferSequence, class ConstBufferSequence>
	constexpr std::size_t buffer_copy(const MutableBufferSequence& dest, const ConstBufferSequence& source, std::size_t max_size) noexcept
	{
		std::size_t amount_written {0};

		auto dest_i {buffer_sequence_begin(dest)};
		const auto dest_end {buffer_sequence_end(dest)};

		auto source_i {buffer_sequence_begin(source)};
		const auto source_end {buffer_sequence_end(source)};

		while (dest_i != dest_end &&
			   source_i != source_end)
		{
			const auto amount_to_write = std::min(max_size, std::min(dest_i->size(), source_i->size()));
			std::copy_n(source_i->data(), amount_to_write, dest_i->data());
			amount_written += amount_to_write;
			*dest_i += amount_to_write;
			*source_i += amount_to_write;
			max_size -= amount_to_write;

			if (dest_i->size() == 0)
			{
				++dest_i;
			}

			if (source_i->size() == 0)
			{
				++source_i;
			}
		}

		return amount_written;
	}
}