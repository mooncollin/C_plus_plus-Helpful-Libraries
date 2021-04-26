export module cmoon.tuple.default_tuple;

import <tuple>;

namespace cmoon::tuple
{
	export
	template<class... Ts>
	constexpr auto default_tuple()
	{
		return std::make_tuple((Ts{})...);
	}
}