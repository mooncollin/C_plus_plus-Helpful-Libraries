#pragma once

#include "measure.hpp"
#include "distance.hpp"
#include "mass.hpp"
#include "time.hpp"

namespace collin
{
	namespace measures
	{
		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using area = basic_dimension_unit_t<Distance, 2>;

		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using volume = basic_dimension_unit_t<Distance, 3>;

		template<class Distance, class Mass, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance> &&
				is_mass_v<Mass>
		>>
		using newton = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -2>>;

		template<class Distance, class Mass, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance> &&
				is_mass_v<Mass>
		>>
		using momentum = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -1>>;

		template<class Distance, class Mass, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance> &&
				is_mass_v<Mass>
		>>
		using angular_momentum = basic_derived_unit<basic_dimension_unit_t<Distance, 2>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -1>>;

		template<class Distance, class Mass, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance> &&
				is_mass_v<Mass>
		>>
		using torque = basic_derived_unit<basic_dimension_unit_t<Distance, 2>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<seconds, -2>>;

		template<class Distance, class Mass, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance> &&
				is_mass_v<Mass>
		>>
		using density = basic_derived_unit<basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Distance, -3>>;
	}
}