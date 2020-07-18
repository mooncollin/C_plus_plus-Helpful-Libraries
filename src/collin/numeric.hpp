#pragma once

namespace collin
{
	template<class ForwardIt, class T>
	constexpr void iota(ForwardIt first, ForwardIt last, T value)
	{
		while (first != last)
		{
			*first++ = value;
			++value;
		}
	}
}