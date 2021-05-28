export module cmoon.net.buffers.write;

import <cstddef>;
import <system_error>;

import cmoon.net.buffers.buffer_sequence_begin;
import cmoon.net.buffers.buffer_sequence_end;
import cmoon.net.buffers.transfer_all;
import cmoon.net.buffers.buffer_traits;

namespace cmoon::net
{
	export
	template<class SyncWriteStream, class ConstBufferSequence, class CompletionCondition>
	std::size_t write(SyncWriteStream& stream, const ConstBufferSequence& buffers, CompletionCondition completion_condition, std::error_code& ec)
	{
		std::size_t total_bytes_written {0};
		ec.clear();
		auto i {buffer_sequence_begin(buffers)};
		const auto end {buffer_sequence_end(buffers)};

		while (i != end)
		{
			const auto bytes_written = stream.write_some(buffer(*i, completion_condition(ec, i->size())), ec);
			if (bytes_written == 0)
			{
				break;
			}

			*i += bytes_written;
			total_bytes_written += bytes_written;

			if (i->size() == 0)
			{
				++i;
			}
		}

		return total_bytes_written;
	}

	export
	template<class SyncWriteStream, class ConstBufferSequence>
	std::size_t write(SyncWriteStream& stream, const ConstBufferSequence& buffers)
	{
		std::error_code ec;
		return write(stream, buffers, std::transfer_all{}, ec);
	}

	export
	template<class SyncWriteStream, class ConstBufferSequence>
	std::size_t write(SyncWriteStream& stream, const ConstBufferSequence& buffers, std::error_code& ec)
	{
		return write(stream, buffers, std::transfer_all{}, ec);
	}

	export
	template<class SyncWriteStream, class ConstBufferSequence, class CompletionCondition>
	std::size_t write(SyncWriteStream& stream, const ConstBufferSequence& buffers, CompletionCondition completion_condition)
	{
		std::error_code ec;
		return write(stream, buffers, completion_condition, ec);
	}

	export
	template<class SyncWriteStream, class DynamicBuffer, class CompletionCondition>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t write(SyncWriteStream& stream, DynamicBuffer b, CompletionCondition completion_condition, std::error_code& ec)
	{
		std::size_t total_bytes_written {0};
		ec.clear();

		while (b.size() > 0)
		{
			const auto bytes_written = stream.write_some(b.data(0, completion_condition(b.size(), ec)));
			if (bytes_written == 0)
			{
				break;
			}
			
			total_bytes_written += bytes_written;

			b.consume(bytes_written);
		}

		return total_bytes_written;
	}

	export
	template<class SyncWriteStream, class DynamicBuffer>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t write(SyncWriteStream& stream, DynamicBuffer b)
	{
		std::error_code ec;
		return write(stream, b, transfer_all{}, ec);
	}

	export
	template<class SyncWriteStream, class DynamicBuffer, class CompletionCondition>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t write(SyncWriteStream& stream, DynamicBuffer b, CompletionCondition completion_condition)
	{
		std::error_code ec;
		return write(stream, b, completion_condition, ec);
	}

	export
	template<class SyncWriteStream, class DynamicBuffer>
		requires(is_dynamic_buffer_v<DynamicBuffer>)
	std::size_t write(SyncWriteStream& stream, DynamicBuffer b, std::error_code& ec)
	{
		return write(stream, b, transfer_all{}, ec);
	}
}