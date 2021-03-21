#pragma once

#include <type_traits>
#include <concepts>
#include <stdexcept>

#include "cmoon/execution/impl/sink_receiver.hpp"
#include "cmoon/execution/impl/submit.hpp"
#include "cmoon/execution/impl/receiver.hpp"
#include "cmoon/execution/impl/executor.hpp"
#include "cmoon/execution/impl/void_receiver.hpp"
#include "cmoon/execution/impl/as_invocable.hpp"

namespace cmoon
{
	namespace execution
	{
		namespace details
		{
			template<class S, class R>
			concept sender_to_impl =
				requires(S && s, R && r)
			{
				execution::submit(std::forward<S>(s), std::forward<R>(r));
			};
		}

		struct sender_base {};

		template<class S>
		concept sender = std::move_constructible<std::remove_cvref_t<S>> &&
						 details::sender_to_impl<S, sink_receiver>;

		template<class S, class R>
		concept sender_to = sender<S> &&
							receiver<R> &&
							details::sender_to_impl<S, R>;

		template<class S>
		struct sender_traits;

		namespace details
		{
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
				requires(executor_of<S, details::as_invocable<details::void_receiver, S>>)
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
		}

		template<class S>
		struct sender_traits : details::sender_traits_base<S> {};
		template<class S>
		concept typed_sender = sender<S> &&
							   details::has_sender_types<sender_traits<S>>;
	}
}