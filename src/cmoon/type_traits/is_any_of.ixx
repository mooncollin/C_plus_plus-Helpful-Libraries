export module cmoon.type_traits.is_any_of;

import <type_traits>;

namespace cmoon
{
	export
	template<typename T, typename... Ts>
	struct is_any_of : public std::disjunction<std::is_same<T, Ts>...> {};

	export
	template<typename T, typename... Ts>
	constexpr bool is_any_of_v = is_any_of<T, Ts...>::value;

	export
	template<typename T, typename... Ts>
	struct is_all_of : public std::conjunction<std::is_same<T, Ts>...> {};

	export
	template<typename T, typename... Ts>
	constexpr bool is_all_of_v = is_all_of<T, Ts...>::value;
}