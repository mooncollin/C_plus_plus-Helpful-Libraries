#pragma once

#include "cmoon/measures/measure.hpp"
#include "cmoon/measures/distance.hpp"
#include "cmoon/measures/mass.hpp"
#include "cmoon/measures/time.hpp"

namespace cmoon
{
	namespace measures
	{
		template<distance_type Distance>
		using area = convert_unit_dimension<Distance, 2>;

		template<distance_type Distance>
		using volume = convert_unit_dimension<Distance, 3>;

		template<class Rep, distance_type Distance, mass_type Mass, time_type Time = seconds>
		using newton = basic_derived_unit<Rep, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Time, -2>>;

		template<class Rep, distance_type Distance, mass_type Mass, time_type Time = seconds>
		using momentum = basic_derived_unit<Rep, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Time, -1>>;

		template<class Rep, distance_type Distance, mass_type Mass, time_type Time = seconds>
		using angular_momentum = basic_derived_unit<Rep, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Time, -1>>;

		template<class Rep, distance_type Distance, mass_type Mass, time_type Time = seconds>
		using torque = basic_derived_unit<Rep, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Time, -2>>;

		template<class Rep, distance_type Distance, mass_type Mass>
		using density = basic_derived_unit<Rep, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, -3>>;
	}
}