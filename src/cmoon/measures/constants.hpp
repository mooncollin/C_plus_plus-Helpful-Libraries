#pragma once

#include "cmoon/measures/measure.hpp"
#include "cmoon/measures/kinematic.hpp"
#include "cmoon/measures/distance.hpp"
#include "cmoon/measures/time.hpp"
#include "cmoon/measures/mass.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep = std::intmax_t>
		constexpr speed<Rep, meters> speed_of_light_vacuum {299'792'458};

		template<class Rep = std::intmax_t>
		constexpr basic_meters<Rep> light_year {9'460'730'472'580'800};
	}
}