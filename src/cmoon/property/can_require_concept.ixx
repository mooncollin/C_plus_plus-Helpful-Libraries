export module cmoon.property.can_require_concept;

import <type_traits>;

import cmoon.property.require_concept;

namespace cmoon
{
	export
	template<class T, class P>
	struct can_require_concept : std::false_type {};

	export
	template<class T, class P>
		requires(requires { cmoon::require_concept(std::declval<T>(), std::declval<P>()); })
	struct can_require_concept<T, P> : std::true_type {};

	export
	template<class T, class P>
	inline constexpr bool can_require_concept_v = can_require_concept<T, P>::value;
}