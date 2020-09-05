#pragma once

#include "measure.hpp"
#include "distance.hpp"
#include "time.hpp"

namespace collin
{
	namespace measures
	{
		template<distance_type Distance, time_type Time = seconds>
		using speed = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Time, -1>>;

		template<distance_type Distance, time_type Time = seconds>
		using acceleration = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Time, -2>>;

		template<distance_type Distance, time_type Time = seconds>
		using jolt = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Time, -3>>;

		template<distance_type Distance, time_type Time = seconds>
		using jounce = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Time, -4>>;

		template<distance_type Distance, time_type Time = seconds>
		using volumetric_flow = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 3>, basic_dimension_unit_t<Time, -1>>;

		using angular_velocity = basic_dimension_unit_t<seconds, -1>;
		using angular_acceleration = basic_dimension_unit_t<seconds, -2>;
		using frequency_drift = basic_dimension_unit_t<seconds, -2>;
	}
}