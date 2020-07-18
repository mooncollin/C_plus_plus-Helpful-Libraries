#pragma once

#include <tuple>
#include <functional>

namespace collin
{
	namespace tuples
	{
		template<typename... Ts, class Function>
		void for_each(const std::tuple<Ts...>& t, Function func)
		{
			std::apply([&](const auto& ...x) {
				(..., func(x));
			}, t);
		}

		template<typename... Ts, class Function>
		void for_each(std::tuple<Ts...>& t, Function func)
		{
			std::apply([&](auto& ...x) {
				(..., func(x));
				}, t);
		}

		template<class... Ts>
		constexpr auto default_tuple()
		{
			return std::make_tuple((Ts{})...);
		}
	}
}