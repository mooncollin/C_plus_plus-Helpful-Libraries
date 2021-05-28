export module cmoon.meta.utility;

import <cstddef>;

import cmoon.meta.type_list;

namespace cmoon::meta
{
	export
	template<std::size_t Index, typename... Ts>
	using get_type = typename type_list<Ts...>::template get_type<Index>;
}