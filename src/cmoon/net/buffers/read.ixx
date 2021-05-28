export module cmoon.net.buffers.read;

import <cstddef>;
import <system_error>;

import cmoon.net.buffers.buffer_sequence_begin;
import cmoon.net.buffers.buffer_sequence_end;
import cmoon.net.buffers.transfer_all;
import cmoon.net.buffers.buffer_traits;

namespace cmoon::net
{
	export
	template<class SyncReadStream, class MutableBufferSequence, class CompletionCondition>
		requires(is_mutable_buffer_sequence_v<MutableBufferSequence>)
	std::size_t read(SyncReadStream& stream, const MutableBufferSequence& buffers, CompletionCondition completion_condition, std::error_code& ec)
	{
		std::size_t total_bytes_read {0};
		ec.clear();
		auto i {buffer_sequence_begin(buffers)};
		const auto end {buffer_sequence_end(buffers)};

		while (i != end)
		{
			const auto bytes_read = stream.read_some(buffer(*i, completion_condition(ec, i->size())), ec);
			if (bytes_read == 0)
			{
				break;
			}

			*i += bytes_read;
			total_bytes_read += bytes_read;
			
			if (i->size() == 0)
			{
				++i;
			}
		}

		return total_bytes_read;
	}

	export
	template<class SyncReadStream, class MutableBufferSequence, class CompletionCondition>
		requires(is_mutable_buffer_sequence_v<MutableBufferSequence>)
	std::size_t read(SyncReadStream& stream, const MutableBufferSequence& buffers, CompletionCondition completion_condition)
	{
		std::error_code ec;
		return read(stream, buffers, completion_condition, ec);
	}

	export
	template<class SyncReadStream, class MutableBufferSequence>
		requires(is_mutable_buffer_sequence)_<MutableBufferSequence>)
	std::size_t read(SyncReadStream& stream, const MutableBufferSequence& buffers)
	{
		std::error_code ec;
		return read(stream, buffers, transfer_all{}, ec);
	}

	export
	template<class SyncReadStream, class MutableBufferSequence>
		requires(is_mutable_buffer_sequence_v<MutableBufferSequence>)
	std::size_t read(SyncReadStream& stream, const MutableBufferSequence& buffers, std::error_code& ec)
	{
		return read(stream, buffers, transfer_all{}, ec);
	}

	export
	template<class SyncReadStream, class DynamicBuffer, class CompletionCondition>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t read(SyncReadStream& stream, DynamicBuffer b, CompletionCondition completion_condition, std::error_code& ec)
	{
		std::size_t total_bytes_read {0};
		ec.clear();

		while (true)
		{
			const auto orig_size = b.size();
			const auto N = completion_condition(ec, b.capacity() - orig_size);
			if (N == 0)
			{
				break;
			}

			b.grow(N);
			auto x = b.data(orig_size, N);
			const auto bytes_read = stream.read_some(x, ec);

			if (bytes_read == 0)
			{
				break;
			}

			total_bytes_read += bytes_read;
		}

		return total_bytes_read;
	}

	export
	template<class SyncReadStream, class DynamicBuffer>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t read(SyncReadStream& stream, DynamicBuffer b)
	{
		std::error_code ec;
		return read(stream, b, transfer_all{}, ec);
	}

	export
	template<class SyncReadStream, class DynamicBuffer>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t read(SyncReadStream& stream, DynamicBuffer b, std::error_code& ec)
	{
		return read(stream, b, transfer_all{}, ec);
	}

	export
	template<class SyncReadStream, class DynamicBuffer, class CompletionCondition>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t read(SyncReadStream& stream, DynamicBuffer b, CompletionCondition completion_condition)
	{
		std::error_code ec;
		return read(stream, b, completion_condition, ec);
	}
}