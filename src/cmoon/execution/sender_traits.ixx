export module cmoon.execution.sender_traits;

import <concepts>;
import <exception>;
import <type_traits>;

import cmoon.execution.impl.has_sender_types;
import cmoon.execution.impl.is_awaitable;
import cmoon.execution.sender_base;

namespace cmoon::execution
{
	template<class S>
	struct sender_traits_base
	{
		using unspecialized = void;
	};

	template<has_sender_types S>
	struct sender_traits_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = typename S::template value_types<Tuple, Variant>;

		template<template<class...> class Variant>
		using error_types = typename S::template error_types<Variant>;

		static constexpr bool sends_done {S::sends_done};
	};

	template<class S>
		requires(!has_sender_types<S> &&
				 std::derived_from<S, sender_base>)
	struct sender_traits_base<S> {};

	template<class S>
		requires(!has_sender_types<S> &&
				 !std::derived_from<S, sender_base> &&
				 is_awaitable<S> &&
				 std::is_void_v<std::remove_cv_t<await_result_type<S>>>)
	struct sender_traits_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Variant<Tuple<>>;

		template<template<class...> class Variant>
		using error_types = Variant<std::exception_ptr>;

		static constexpr bool sends_done {false};
	};

	template<class S>
		requires(!has_sender_types<S> &&
				 !std::derived_from<S, sender_base> &&
				 is_awaitable<S> &&
				 !std::is_void_v<std::remove_cv_t<await_result_type<S>>>)
	struct sender_traits_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Variant<Tuple<await_result_type<S>>>;

		template<template<class...> class Variant>
		using error_types = Variant<std::exception_ptr>;

		static constexpr bool sends_done {false};
	};

	export
	template<class S>
	struct sender_traits : public sender_traits_base<S> {};
}