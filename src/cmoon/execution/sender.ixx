export module cmoon.execution.sender;

import <type_traits>;
import <concepts>;
import <stdexcept>;

import cmoon.execution.submit;
import cmoon.execution.receiver;
import cmoon.execution.executor;
import cmoon.execution.as_invocable;
import cmoon.execution.void_receiver;
import cmoon.execution.sink_receiver;

namespace cmoon::execution
{
	template<class S, class R>
	concept sender_to_impl =
		requires(S && s, R && r)
	{
		execution::submit(std::forward<S>(s), std::forward<R>(r));
	};

	export
	struct sender_base {};

	export
	template<class S>
	concept sender = std::move_constructible<std::remove_cvref_t<S>> &&
						sender_to_impl<S, sink_receiver>;

	export
	template<class S, class R>
	concept sender_to = sender<S> &&
						receiver<R> &&
						sender_to_impl<S, R>;

	export
	template<class S>
	struct sender_traits;

	template<template<template<class...> class Tuple, template<class...> class Variant> class>
	struct has_value_types;

	template<template<class...> class Variant>
	struct has_error_types;

	template<class S>
	concept has_sender_types = 
		requires
	{
		typename has_value_types<S::template value_types>;
		typename has_error_types<S::template error_types>;
		typename std::bool_constant<S::sends_done>;
	};

	template<class S>
	struct sender_traits_base
	{
		using __unspecialized = void;
	};

	template<has_sender_types S>
	struct sender_traits_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = typename S::template value_types<Tuple, Variant>;

		template<template<class...> class Variant>
		using error_types = typename S::template error_types<Variant>;

		static constexpr bool sends_done = S::sends_done;
	};

	template<class S>
		requires(executor_of<S, as_invocable<void_receiver, S>>)
	struct sender_traits_base<S>
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Variant<Tuple<>>;

		template<template<class...> class Variant>
		using error_types = Variant<std::exception_ptr>;

		static constexpr bool sends_done = true;
	};

	template<std::derived_from<sender_base> S>
	struct sender_traits_base<S> {};

	export
	template<class S>
	struct sender_traits : sender_traits_base<S> {};

	export
	template<class S>
	concept typed_sender = sender<S> &&
							has_sender_types<sender_traits<S>>;
}