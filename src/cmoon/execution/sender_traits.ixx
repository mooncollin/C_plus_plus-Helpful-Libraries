export module cmoon.execution.sender_traits;

import <concepts>;
import <exception>;


import cmoon.execution.impl.has_sender_types;
import cmoon.execution.impl.as_invocable;
import cmoon.execution.impl.executor_of_impl;
import cmoon.execution.sender_base;

namespace cmoon::execution
{
	struct void_receiver
	{
		void set_value() noexcept;
		void set_error(std::exception_ptr) noexcept;
		void set_done() noexcept;
	};

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
				 executor_of_impl<S, as_invocable<void_receiver, S>>)
	struct sender_traits_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Variant<Tuple<>>;

		template<template<class...> class Variant>
		using error_types = Variant<std::exception_ptr>;

		static constexpr bool sends_done {true};
	};

	template<class S>
		requires(!has_sender_types<S> &&
				 !executor_of_impl<S, as_invocable<void_receiver, S>> &&
				 std::derived_from<S, sender_base>)
	struct sender_traits_base<S> {};

	export
	template<class S>
	struct sender_traits : public sender_traits_base<S> {};
}