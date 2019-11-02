#ifndef COLLIN_TUPLE
#define COLLIN_TUPLE

#include <tuple>
#include <functional>

namespace collin
{
	template<typename... Ts, class Function>
	void for_each(const std::tuple<Ts...>& t, Function func)
	{
		std::apply([&](const auto& ...x) {
			(..., func(x));
		}, t);
	}
}

#endif