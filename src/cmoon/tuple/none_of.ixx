export module cmoon.tuple.none_of;

import <tuple>;

namespace cmoon::tuple
{
	export
	template<class Tuple, class Function>
	bool none_of(Tuple&& t, Function&& func)
	{
		return std::apply([f = std::forward<Function>(func)](const auto& ...x) {
			return (!f(x) && ...);
		}, std::forward<Tuple>(t));
	}
}