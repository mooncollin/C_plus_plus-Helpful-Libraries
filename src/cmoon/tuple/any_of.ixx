export module cmoon.tuple.any_of;

import <tuple>;

namespace cmoon::tuple
{
	export
	template<class Tuple, class Function>
	bool any_of(Tuple&& t, Function&& func)
	{
		return std::apply([f = std::forward<Function>(func)](const auto& ...x) {
			return (f(x) || ...);
		}, std::forward<Tuple>(t));
	}
}