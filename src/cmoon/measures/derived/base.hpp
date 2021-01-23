#pragma once

#include "cmoon/ratio.hpp"
#include "cmoon/measures/measure.hpp"
#include "cmoon/measures/time.hpp"
#include "cmoon/measures/electric_current.hpp"
#include "cmoon/measures/mass.hpp"
#include "cmoon/measures/distance.hpp"
#include "cmoon/measures/temperature.hpp"
#include "cmoon/measures/amount.hpp"
#include "cmoon/measures/luminous.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep, cmoon::ratio_type Ratio, time_type Time = seconds>
		using frequency = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Time, -1>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, time_type Time = seconds>
		using force = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, time_type Time = seconds>
		using pressure = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, -1>, convert_unit_dimension<Time, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, time_type Time = seconds>
		using work = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, time_type Time = seconds>
		using power = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -3>>;

		template<class Rep, cmoon::ratio_type Ratio, electric_current_type Current, time_type Time = seconds>
		using electric_charge = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Time, 1>, convert_unit_dimension<Current, 1>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, electric_current_type Current, time_type Time = seconds>
		using voltage = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -3>, convert_unit_dimension<Current, -1>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, electric_current_type Current, time_type Time = seconds>
		using electrical_capacitance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, -1>, convert_unit_dimension<Distance, -2>, convert_unit_dimension<Time, 4>, convert_unit_dimension<Current, 2>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, electric_current_type Current, time_type Time = seconds>
		using electrical_resistance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -3>, convert_unit_dimension<Current, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, electric_current_type Current, time_type Time = seconds>
		using electrical_conductance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, -1>, convert_unit_dimension<Distance, -2>, convert_unit_dimension<Time, 3>, convert_unit_dimension<Current, 2>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, electric_current_type Current, time_type Time = seconds>
		using magnetic_flux = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -1>, convert_unit_dimension<Current, -1>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, electric_current_type Current, time_type Time = seconds>
		using magnetic_induction = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Time, -2>, convert_unit_dimension<Current, -1>>;

		template<class Rep, cmoon::ratio_type Ratio, mass_type Mass, distance_type Distance, electric_current_type Current, time_type Time = seconds>
		using electrical_inductance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -2>, convert_unit_dimension<Current, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, luminous_intensity_type Luminous, distance_type Distance>
		using illuminance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Luminous, 1>, convert_unit_dimension<Distance, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, distance_type Distance, time_type Time = seconds>
		using absorbed_dose_radiation = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -2>>;

		template<class Rep, cmoon::ratio_type Ratio, substance_amount_type Amount, time_type Time = seconds>
		using catalytic_activity = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Time, -1>, convert_unit_dimension<Amount, 1>>;
	}
}