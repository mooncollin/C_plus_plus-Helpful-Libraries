export module cmoon.measures.derived.base;

import cmoon.measures;
import cmoon.measures.time;
import cmoon.measures.electric_current;
import cmoon.measures.mass;
import cmoon.measures.distance;
import cmoon.measures.temperature;
import cmoon.measures.amount;
import cmoon.measures.luminous;

namespace cmoon::measures
{
	export
	template<class Rep, class Ratio, class Time = seconds>
	using frequency = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Time, -1>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Time = seconds>
	using force = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 1>, convert_unit_dimension<Time, -2>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Time = seconds>
	using pressure = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, -1>, convert_unit_dimension<Time, -2>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Time = seconds>
	using work = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -2>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Time = seconds>
	using power = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -3>>;

	export
	template<class Rep, class Ratio, class Current, class Time = seconds>
	using electric_charge = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Time, 1>, convert_unit_dimension<Current, 1>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Current, class Time = seconds>
	using voltage = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -3>, convert_unit_dimension<Current, -1>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Current, class Time = seconds>
	using electrical_capacitance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, -1>, convert_unit_dimension<Distance, -2>, convert_unit_dimension<Time, 4>, convert_unit_dimension<Current, 2>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Current, class Time = seconds>
	using electrical_resistance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -3>, convert_unit_dimension<Current, -2>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Current, class Time = seconds>
	using electrical_conductance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, -1>, convert_unit_dimension<Distance, -2>, convert_unit_dimension<Time, 3>, convert_unit_dimension<Current, 2>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Current, class Time = seconds>
	using magnetic_flux = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -1>, convert_unit_dimension<Current, -1>>;

	export
	template<class Rep, class Ratio, class Mass, class Current, class Time = seconds>
	using magnetic_induction = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Time, -2>, convert_unit_dimension<Current, -1>>;

	export
	template<class Rep, class Ratio, class Mass, class Distance, class Current, class Time = seconds>
	using electrical_inductance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Mass, 1>, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -2>, convert_unit_dimension<Current, -2>>;

	export
	template<class Rep, class Ratio, luminous_intensity_type Luminous, class Distance>
	using illuminance = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Luminous, 1>, convert_unit_dimension<Distance, -2>>;

	export
	template<class Rep, class Ratio, class Distance, class Time = seconds>
	using absorbed_dose_radiation = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Distance, 2>, convert_unit_dimension<Time, -2>>;

	export
	template<class Rep, class Ratio, class Amount, class Time = seconds>
	using catalytic_activity = basic_derived_unit<Rep, Ratio, convert_unit_dimension<Time, -1>, convert_unit_dimension<Amount, 1>>;
}