export module cmoon.type_traits.is_specialization;

import <type_traits>;

namespace cmoon
{
	export
	template<class Type, template<typename...> class Template>
	struct is_specialization : std::false_type {};

	export
	template<template<typename...> class Template, class... Types>
	struct is_specialization<Template<Types...>, Template> : std::true_type {};

	export
	template<class Type, template<typename...> class Template>
	inline constexpr bool is_specialization_v = is_specialization<Type, Template>::value;
}