#pragma once

#include "collin/measures/measure.hpp"
#include "collin/measures/kinematic.hpp"
#include "collin/measures/distance.hpp"
#include "collin/measures/time.hpp"
#include "collin/measures/mass.hpp"

namespace collin
{
	namespace measures
	{
		template<class Rep = std::intmax_t>
		constexpr speed<Rep, meters> speed_of_light_vacuum {299'792'458};

		template<class Rep = std::intmax_t>
		constexpr basic_meters<Rep> light_year {9'460'730'472'580'800};

		constexpr basic_derived_unit<long double, convert_unit_dimension<meters, 3>, convert_unit_dimension<kilograms, -1>, convert_unit_dimension<seconds, -2>> gravitational_constant {6.67430E-11};
	}
}