export module cmoon.net.buffers.buffer_sequence_end;

namespace cmoon::net
{
	export
	template<class C>
	inline constexpr auto buffer_sequence_end(C& c) noexcept
	{
		return c.end();
	}

	export
	template<class C>
	inline constexpr auto buffer_sequence_end(const C& c) noexcept
	{
		return c.end();
	}
}