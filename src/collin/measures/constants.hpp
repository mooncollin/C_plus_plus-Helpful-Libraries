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
		constexpr speed<basic_meters<Rep>> speed_of_light_vacuum {299'792'458};

		template<class Rep = std::intmax_t>
		constexpr basic_meters<Rep> light_year {9'460'730'472'580'800};

		constexpr basic_derived_unit<std::uintmax_t, basic_dimension_unit_t<meters, 3>, basic_dimension_unit_t<kilograms, -1>, basic_dimension_unit_t<seconds, -2>> gravitational_constant {collin::math::basic_rational<std::uintmax_t>{667430, 10000000000000000}};
	}
}