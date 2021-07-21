export module cmoon.math.trig;

import <cmath>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<cmoon::arithmetic T>
	[[nodiscard]] auto cot(T t) noexcept
	{
		return 1.0 / std::tan(t);
	}
}