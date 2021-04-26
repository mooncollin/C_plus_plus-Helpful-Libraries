export module cmoon.tuple.filter;

import <tuple>;
import <utility>;
import <type_traits>;

namespace cmoon::tuple
{
	export
	template<template<class> class Predicate, typename Tuple>
	struct filter;

	export
	template<template<class> class Predicate, typename... Ts>
	struct filter<Predicate, std::tuple<Ts...>>
	{
		private:
			template<class E>
			using filter_impl = std::conditional_t<Predicate<E>::value, std::tuple<E>, std::tuple<>>;
		public:
			using type = decltype(std::tuple_cat(std::declval<filter_impl<Ts>>()...));
	};

	export
	template<template<class> class Predicate, typename... Ts>
	using filter_t = typename filter<Predicate, Ts...>::type;
}