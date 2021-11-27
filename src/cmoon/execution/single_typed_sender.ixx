export module cmoon.execution.single_typed_sender;

import <type_traits>;

import cmoon.execution.typed_sender;

namespace cmoon::execution
{
	template<class Variant>
	struct single_sender_value_type_impl;

	template<template<class...> class Variant, template<class...> class Tuple, class T>
	struct single_sender_value_type_impl<Variant<Tuple<T>>> : std::type_identity<T> {};

	template<template<class...> class Variant, template<class...> class Tuple>
	struct single_sender_value_type_impl<Variant<Tuple<>>> : std::type_identity<void> {};

	export
	template<class S>
		requires(requires
		{
			typename single_sender_value_type_impl<value_types_of_t<S>>::type;
		})
	using single_sender_value_type = typename single_sender_value_type_impl<value_types_of_t<S>>::type;

	export
	template<class S>
	concept single_typed_sender = typed_sender<S> &&
		requires
	{
		typename single_sender_value_type<S>;
	};
}