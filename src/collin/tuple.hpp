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

		template<template<class> class Predicate, typename Tuple>
		struct filter;

		template<template<class> class Predicate, typename... Ts>
		struct filter<Predicate, std::tuple<Ts...>>
		{
			private:
				template<class E>
				using filter_impl = std::conditional_t<Predicate<E>::value, std::tuple<E>, std::tuple<>>;
			public:
				using type = decltype(std::tuple_cat(std::declval<filter_impl<Ts>>()...));
		};

		template<template<class> class Predicate, typename... Ts>
		using filter_t = typename filter<Predicate, Ts...>::type;
	}
}