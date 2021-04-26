export module cmoon.measures.coherent;

import cmoon.measures;
import cmoon.measures.time;
import cmoon.measures.distance;
import cmoon.measures.electric_current;
import cmoon.measures.amount;
import cmoon.measures.luminous;

namespace cmoon::measures
{
	export
	template<time_type Time>
	using angular_velocity = convert_unit_dimension<Time, -1>;

	export
	template<time_type Time>
	using angular_acceleration = convert_unit_dimension<Time, -2>;

	export
	template<time_type Time>
	using frequency_drift = convert_unit_dimension<Time, -2>;

	export
	template<distance_type Distance>
	using area = convert_unit_dimension<Distance, 2>;

	export
	template<distance_type Distance>
	using volume = convert_unit_dimension<Distance, 3>;

	export
	template<distance_type Distance>
	using wavenumber = convert_unit_dimension<Distance, -1>;

	export
	template<class Rep, distance_type Distance, time_type Time = seconds>
	using speed = basic_derived_unit<Rep, std::ratio<1>, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -1>>;

	export
	template<class Rep, distance_type Distance, time_type Time = seconds>
	using acceleration = basic_derived_unit<Rep, std::ratio<1>, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -2>>;

	export
	template<class Rep, distance_type Distance, time_type Time = seconds>
	using jolt = basic_derived_unit<Rep, std::ratio<1>, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -3>>;

	export
	template<class Rep, distance_type Distance, time_type Time = seconds>
	using jounce = basic_derived_unit<Rep, std::ratio<1>, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -4>>;

	export
	template<class Rep, distance_type Distance, time_type Time = seconds>
	using volumetric_flow = basic_derived_unit<Rep, std::ratio<1>, convert_unit_dimension<Distance, 3>, convert_unit_dimension<Time, -1>>;

	export
	template<class Rep, distance_type Distance, mass_type Mass>
	using density = basic_derived_unit<Rep, std::ratio<1>, convert_unit_dimension<Distance, -3>, convert_unit_dimension<Mass, 1>>;

	export
	template<class Rep, mass_type Mass, distance_type Distance>
	using surface_density = basic_derived_unit<Rep, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, -2>>;

	export
	template<class Rep, mass_type Mass, distance_type Distance>
	using specific_volume = basic_derived_unit<Rep, convert_unit_dimension<Mass, -1>, convert_unit_dimension<Distance, 3>>;

	export
	template<class Rep, distance_type Distance, electric_current_type Current>
	using current_density = basic_derived_unit<Rep, convert_unit_dimension<Distance, -2>, convert_unit_dimension<Current, 1>>;

	export
	template<class Rep, distance_type Distance, electric_current_type Current>
	using magnetic_field_strength = basic_derived_unit<Rep, convert_unit_dimension<Distance, -1>, convert_unit_dimension<Current, 1>>;

	export
	template<class Rep, distance_type Distance, substance_amount_type Amount>
	using concentration = basic_derived_unit<Rep, convert_unit_dimension<Distance, -3>, convert_unit_dimension<Amount, 1>>;

	export
	template<class Rep, mass_type Mass, distance_type Distance>
	using mass_concentration = basic_derived_unit<Rep, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, -3>>;

	export
	template<class Rep, luminous_intensity_type Lum, distance_type Distance>
	using luminance = basic_derived_unit<Rep, convert_unit_dimension<Lum, 1>, convert_unit_dimension<Distance, 2>>;
}