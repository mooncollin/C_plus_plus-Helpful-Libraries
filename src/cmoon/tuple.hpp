#pragma once

#include <tuple>
#include <functional>
#include <type_traits>

namespace cmoon
{
	namespace tuples
	{
		template<class Tuple, class Function>
		void for_each(Tuple&& t, Function&& func)
		{
			std::apply([f = std::forward<Function>(func)](auto& ...x) {
				(f(x), ...);
			}, std::forward<Tuple>(t));
		}

		template<class Tuple, class Function>
		bool all_of(Tuple&& t, Function&& func)
		{
			return std::apply([f = std::forward<Function>(func)](const auto& ...x) {
				return (f(x) && ...);
			}, std::forward<Tuple>(t));
		}

		template<class Tuple, class Function>
		bool any_of(Tuple&& t, Function&& func)
		{
			return std::apply([f = std::forward<Function>(func)](const auto& ...x) {
				return (f(x) || ...);
			}, std::forward<Tuple>(t));
		}

		template<class Tuple, class Function>
		bool none_of(Tuple&& t, Function&& func)
		{
			return std::apply([f = std::forward<Function>(func)](const auto& ...x) {
				return (!f(x) && ...);
			}, std::forward<Tuple>(t));
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