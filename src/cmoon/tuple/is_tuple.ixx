export module cmoon.tuple.is_tuple;

import <tuple>;
import <type_traits>;

namespace cmoon::tuple
{
	template<class Tuple>
	struct is_tuple : std::false_type {};

	template<class... Types>
	struct is_tuple<std::tuple<Types...>> : std::true_type {};

	template<class Tuple>
	constexpr auto is_tuple_v = is_tuple<Tuple>::value;

	template<class Tuple>
	concept tuple_type = is_tuple_v<Tuple>;
}