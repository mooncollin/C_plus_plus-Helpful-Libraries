export module cmoon.measures.coherent.constants;

import <cstdint>;

import cmoon.measures.coherent;
import cmoon.measures.distance;

namespace cmoon::measures
{
	export
	template<class Rep = std::intmax_t>
	constexpr speed<Rep, meters> speed_of_light_vacuum {299'792'458};
}