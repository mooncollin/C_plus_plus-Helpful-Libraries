#pragma once

#include "cmoon/measures/measure.hpp"
#include "cmoon/measures/distance.hpp"
#include "cmoon/measures/time.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep, distance_type Distance, time_type Time = seconds>
		using speed = basic_derived_unit<Rep, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -1>>;

		template<class Rep, distance_type Distance, time_type Time = seconds>
		using acceleration = basic_derived_unit<Rep, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -2>>;

		template<class Rep, distance_type Distance, time_type Time = seconds>
		using jolt = basic_derived_unit<Rep, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -3>>;

		template<class Rep, distance_type Distance, time_type Time = seconds>
		using jounce = basic_derived_unit<Rep, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -4>>;

		template<class Rep, distance_type Distance, time_type Time = seconds>
		using volumetric_flow = basic_derived_unit<Rep, convert_unit_dimension<Distance, 3>, convert_unit_dimension<Time, -1>>;

		template<time_type Time>
		using angular_velocity = convert_unit_dimension<Time, -1>;

		template<time_type Time>
		using angular_acceleration = convert_unit_dimension<Time, -2>;

		template<time_type Time>
		using frequency_drift = convert_unit_dimension<Time, -2>;
	}
}