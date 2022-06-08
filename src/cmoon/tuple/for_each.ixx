export module cmoon.tuple.for_each;

import <functional>;
import <utility>;

namespace cmoon
{
	export
	template<class F, class Tuple>
	constexpr void tuple_for_each(F&& f, Tuple&& tuple)
	{
		std::apply([&f]<class... Ts>(Ts&&... elements) {
			(std::invoke(f, std::forward<Ts>(elements)), ...);
		}, std::forward<Tuple>(tuple));
	}
}