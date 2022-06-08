export module cmoon.tuple.tuple_like;

import <type_traits>;
import <tuple>;
import <utility>;

namespace cmoon
{
	template<class... Types>
	struct tuple_like_t : public std::type_identity<std::tuple<Types...>> {};

	template<class Type1, class Type2>
	struct tuple_like_t<Type1, Type2> : public std::type_identity<std::pair<Type1, Type2>> {};

	export
	template<class... Types>
	using tuple_like = typename tuple_like_t<Types...>::type;

	export
	template<class... Types>
	constexpr tuple_like<Types...> make_tuple_like(Types&&... values) noexcept(std::is_nothrow_constructible_v<tuple_like<Types...>, Types...>)
	{
		return tuple_like<Types...>{std::forward<Types>(values)...};
	}
}