export module cmoon.tuple.tuple_transform;

import <tuple>;
import <utility>;
import <functional>;

import cmoon.tuple.tuple_like;

namespace cmoon
{
	export
	template<class Fn, class Tuple>
	constexpr auto tuple_transform(Fn&& fn, Tuple&& t)
	{
		return std::apply(
			[&fn]<class... Ts>(Ts&&... elements) {
				return tuple_like<std::invoke_result_t<Fn&, Ts>...>{std::invoke(fn, std::forward<Ts>(elements))...};
			},
		std::forward<Tuple>(t));
	}
}