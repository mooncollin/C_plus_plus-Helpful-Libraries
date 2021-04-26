export module cmoon.measures.is_basic_unit;

import <type_traits>;
import <utility>;

import cmoon.ratio;

import cmoon.measures.dimension_type;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension = 1>
	class basic_unit;

	// For type checking only
	template<class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
	std::true_type is_basic_unit_base_impl(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>&);

	std::false_type is_basic_unit_base_impl(...);

	template<class U>
	constexpr auto is_based_in_basic_unit = decltype(is_basic_unit_base_impl(std::declval<U>()))::value;

	export
	template<class T>
	struct is_basic_unit : std::bool_constant<is_based_in_basic_unit<T>> {};

	export
	template<class T>
	constexpr bool is_basic_unit_v = is_basic_unit<T>::value;

	export
	template<class T>
	concept basic_unit_type = is_basic_unit_v<T>;
}