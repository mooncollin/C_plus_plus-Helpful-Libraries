#pragma once

#include "collin/measures/measure.hpp"
#include "collin/measures/kinematic.hpp"
#include "collin/measures/distance.hpp"

namespace collin
{
	namespace measures
	{
		template<class Rep = std::intmax_t>
		constexpr speed<basic_meters<Rep>> speed_of_light_vacuum {299'792'458};

		
	}
}