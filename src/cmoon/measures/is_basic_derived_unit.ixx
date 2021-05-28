export module cmoon.measures.is_basic_derived_unit;

import <utility>;
import <type_traits>;

import cmoon.ratio;
import cmoon.type_traits;

import cmoon.measures.is_basic_unit;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio, basic_unit_type... Units>
		requires(cmoon::is_unique_v<typename Units::unit_values...> &&
					sizeof...(Units) >= 1
				)
	class basic_derived_unit;

	// For type checking only
	template<class Rep, cmoon::ratio_type Ratio, basic_unit_type... Units>
	std::true_type is_basic_derived_unit_base_impl(const basic_derived_unit<Rep, Ratio, Units...>&);

	std::false_type is_basic_derived_unit_base_impl(...);

	template<class U>
	constexpr auto is_based_in_basic_derived_unit = decltype(is_basic_derived_unit_base_impl(std::declval<U>()))::value;

	export
	template<class T>
	struct is_basic_derived_unit : std::bool_constant<is_based_in_basic_derived_unit<T>> {};

	export
	template<class T>
	constexpr bool is_basic_derived_unit_v = is_basic_derived_unit<T>::value;

	export
	template<class T>
	concept basic_derived_unit_type = is_basic_derived_unit_v<T>;
}