#pragma once

#include "measure.hpp"
#include "distance.hpp"
#include "time.hpp"

namespace collin
{
	namespace measures
	{
		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using speed = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<seconds, -1>>;

		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using acceleration = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<seconds, -2>>;

		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using jolt = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<seconds, -3>>;

		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using jounce = basic_derived_unit<basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<seconds, -4>>;

		template<class Distance, typename = std::enable_if_t<
/* requires */	is_distance_v<Distance>
		>>
		using volumetric_flow = basic_derived_unit<basic_dimension_unit_t<Distance, 3>, basic_dimension_unit_t<seconds, -1>>;

		using angular_velocity = basic_dimension_unit_t<seconds, -1>;
		using angular_acceleration = basic_dimension_unit_t<seconds, -2>;
		using frequency_drift = basic_dimension_unit_t<seconds, -2>;
	}
}