export module cmoon.type_traits.is_unique;
import <type_traits>;

import cmoon.type_traits.is_any_of;

namespace cmoon
{
	export
	template<typename...>
	struct is_unique;

	export
	template<>
	struct is_unique<> : std::true_type {};

	export
	template<typename F, typename... T>
	struct is_unique<F, T...> : std::bool_constant<is_unique<T...>::value && !is_any_of_v<F, T...>> {};

	export
	template<typename... T>
	constexpr bool is_unique_v = is_unique<T...>::value;
}