export module cmoon.measures.ratio;

import <type_traits>;

import cmoon.measures.dimension_type;
import cmoon.measures.basic_unit;

namespace cmoon::measures
{
	export
	template<basic_unit_type Unit>
	struct is_numerator_unit : std::bool_constant<(Unit::dimension > static_cast<dimension_type>(0))> {};

	export
	template<basic_unit_type Unit>
	constexpr auto is_numerator_unit_v = is_numerator_unit<Unit>::value;

	export
	template<basic_unit_type Unit>
	struct is_denominator_unit : std::bool_constant<(Unit::dimension < static_cast<dimension_type>(0))> {};

	export
	template<basic_unit_type Unit>
	constexpr auto is_denominator_unit_v = is_denominator_unit<Unit>::value;

	export
	template<class T>
	concept numerator_unit = is_numerator_unit_v<T>;

	export
	template<class T>
	concept denominator_unit = is_denominator_unit_v<T>;
}