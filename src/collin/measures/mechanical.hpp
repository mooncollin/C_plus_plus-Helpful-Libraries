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

		template<distance_type Distance, mass_type Mass, time_type Time = seconds>
		using newton = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Mass::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Time, -2>>;

		template<distance_type Distance, mass_type Mass, time_type Time = seconds>
		using momentum = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Mass::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 1>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Time, -1>>;

		template<distance_type Distance, mass_type Mass, time_type Time = seconds>
		using angular_momentum = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Mass::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 2>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Time, -1>>;

		template<distance_type Distance, mass_type Mass, time_type Time = seconds>
		using torque = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Mass::rep, typename Time::rep>, basic_dimension_unit_t<Distance, 2>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Time, -2>>;

		template<distance_type Distance, mass_type Mass>
		using density = basic_derived_unit<std::common_type_t<typename Distance::rep, typename Mass::rep>, basic_dimension_unit_t<Mass, 1>, basic_dimension_unit_t<Distance, -3>>;
	}
}