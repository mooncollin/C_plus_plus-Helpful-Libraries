#pragma once

#include "measure.hpp"
#include "distance.hpp"
#include "mass.hpp"
#include "time.hpp"

namespace collin
{
	namespace measures
	{
		template<distance_type Distance>
		using area = basic_dimension_unit_t<Distance, 2>;

		template<distance_type Distance>
		using volume = basic_dimension_unit_t<Distance, 3>;

		template<distance_type Distance, mass_type Mass>
		using newton = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -2>>;

		template<distance_type Distance, mass_type Mass>
		using momentum = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -1>>;

		template<distance_type Distance, mass_type Mass>
		using angular_momentum = basic_derived_unit<basic_dimension_unit_t<Distance, 2>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -1>>;

		template<distance_type Distance, mass_type Mass>
		using torque = basic_derived_unit<basic_dimension_unit_t<Distance, 2>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -2>>;

		template<distance_type Distance, mass_type Mass>
		using density = basic_derived_unit<basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Distance, -3>>;
	}
}