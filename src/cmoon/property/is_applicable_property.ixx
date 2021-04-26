export module cmoon.property.is_applicable_property;

import <type_traits>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<class T, class P>
	struct is_applicable_property : std::false_type {};

	export
	template<cmoon::complete_type T, cmoon::complete_type P>
		requires(requires { P::template is_applicable_property_v<T>; } && P::template is_applicable_property_v<T>)
	struct is_applicable_property<T, P> : std::true_type {};

	export
	template<class T, class P>
	inline constexpr bool is_applicable_property_v = is_applicable_property<T, P>::value;
}