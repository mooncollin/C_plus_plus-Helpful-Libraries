export module cmoon.net.buffers.buffer_sequence_begin;

namespace cmoon::net
{
	export
	template<class C>
	inline constexpr auto buffer_sequence_begin(C& c) noexcept
	{
		return c.begin();
	}

	export
	template<class C>
	inline constexpr auto buffer_sequence_begin(const C& c) noexcept
	{
		return c.begin();
	}
}