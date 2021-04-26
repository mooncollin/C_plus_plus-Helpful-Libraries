export module cmoon.measures.distance.constants;

import <cstdint>;

import cmoon.measures.distance;

namespace cmoon::measures
{
	export
	template<class Rep = std::intmax_t>
	constexpr basic_meters<Rep> light_year {9'460'730'472'580'800};
}