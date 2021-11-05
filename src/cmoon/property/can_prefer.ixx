export module cmoon.property.can_prefer;

import <type_traits>;

import cmoon.property.prefer;

namespace cmoon
{
	export
	template<class T, class... Ps>
	struct can_prefer : std::false_type {};

	export
	template<class T, class... Ps>
		requires(requires(const T t, const Ps&... ps) { cmoon::prefer(t, ps...); })
	struct can_prefer<T, Ps...> : std::true_type {};

	export
	template<class T, class... Ps>
	inline constexpr bool can_prefer_v = can_prefer<T, Ps...>::value;
}