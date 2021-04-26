export module cmoon.property.can_require;

import <type_traits>;

import cmoon.property.require;

namespace cmoon
{
	export
	template<class T, class... Ps>
	struct can_require : std::false_type {};

	export
	template<class T, class... Ps>
		requires(requires { cmoon::require(std::declval<const T>(), std::declval<Ps>()...); })
	struct can_require<T, Ps...> : std::true_type {};

	export
	template<class T, class... Ps>
	inline constexpr bool can_require_v = can_require<T, Ps...>::value;
}