export module cmoon.math.trig;

import <cmath>;

import cmoon.concepts;

namespace cmoon
{
	export
	constexpr auto cot {
		[]<cmoon::arithmetic T>(const T t) {
			return 1.0 / std::tan(t);
		}
	};
}