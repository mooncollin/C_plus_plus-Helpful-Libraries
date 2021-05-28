export module cmoon.type_traits.is_any_of;

import <type_traits>;

namespace cmoon
{
	export
	template<typename T, typename... Ts>
	constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;
}