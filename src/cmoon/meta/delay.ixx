export module cmoon.meta.delay;

import <type_traits>;

namespace cmoon::meta
{
	export
	template<template<typename...> class T, typename... Args>
	struct delay : std::type_identity<T<Args...>> {};

	export
	template<template<typename...> class T, typename... Args>
	using delay_t = typename delay<T, Args...>::type;
}