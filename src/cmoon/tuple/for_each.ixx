export module cmoon.tuple.for_each;

import <tuple>;

namespace cmoon::tuple
{
	export
	template<class Tuple, class Function>
	void for_each(Tuple&& t, Function&& func)
	{
		std::apply([f = std::forward<Function>(func)](auto& ...x) {
			(f(x), ...);
		}, std::forward<Tuple>(t));
	}
}