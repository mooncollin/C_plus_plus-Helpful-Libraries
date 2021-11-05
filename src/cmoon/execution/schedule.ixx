export module cmoon.execution.schedule;

import <utility>;
import <exception>;
import <type_traits>;

import cmoon.meta;

import cmoon.execution.receiver;
import cmoon.execution.sender;
import cmoon.execution.executor;
import cmoon.execution.connect;

namespace cmoon::execution
{
	template<class E>
	struct as_sender
	{
		private:
			E ex_;
		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<>>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr>;

			static constexpr bool sends_done {true};

			explicit as_sender(E&& e) noexcept
				: ex_{std::forward<E>(e)} {}

			template<class R>
				requires(receiver_of<R>)
			connect_result_t<const E&, R> connect(R&& r) const&
			{
				return execution::connect(ex_, std::forward<R>(r));
			}
	};

	namespace schedule_ns
	{
		void schedule();

		class schedule_t
		{
			private:
				enum class state { none, member_fn, default_fn, as_sender_s };

				template<class S>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(S&& s) {
						{ s.schedule() } -> sender;
					})
					{
						return {state::member_fn, noexcept(std::declval<S>().schedule())};
					}
					else if constexpr (requires(S&& s) {
						{ schedule(std::forward<S>(s)) } -> sender;
					})
					{
						return {state::default_fn, noexcept(schedule(std::declval<S>()))};
					}
					else if constexpr (executor<S>)
					{
						return {state::as_sender_s, std::is_nothrow_constructible_v<as_sender<S>, S>};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class S>
					requires(choose<S>().strategy != state::none)
				constexpr sender auto operator()(S&& s) const noexcept(choose<S>().no_throw)
				{
					constexpr auto choice {choose<S>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						return s.schedule();
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						return schedule(std::forward<S>(s));
					}
					else if constexpr (choice.strategy == state::as_sender_s)
					{
						return as_sender<S>{std::forward<S>(s)};
					}
				}
		};
	}

	export
	inline constexpr schedule_ns::schedule_t schedule{};
}