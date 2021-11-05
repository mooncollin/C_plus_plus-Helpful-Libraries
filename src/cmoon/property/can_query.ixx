export module cmoon.property.can_query;

import <type_traits>;

import cmoon.property.query;

namespace cmoon
{
	export
	template<class T, class P>
	struct can_query : std::false_type {};

	export
	template<class T, class P>
		requires(requires(const T t, P p) { cmoon::query(t, p); })
	struct can_query<T, P> : std::true_type {};

	export
	template<class T, class P>
	inline constexpr bool can_query_v = can_query<T, P>::value;
}