export module cmoon.execution.schedule;

import <utility>;
import <concepts>;

import cmoon.meta;

import cmoon.execution.connect;
import cmoon.execution.sender;
import cmoon.execution.executor;

namespace cmoon::execution
{
	namespace schedule_cpo
	{
		template<class E>
		struct as_sender
		{
			public:
				template<template<class...> class Tuple, template<class...> class Varient>
				using value_types = Varient<Tuple<>>;

				template<template<class...> class Varient>
				using error_types = Varient<std::exception_ptr>;

				static constexpr bool sends_done = true;

				explicit as_sender(E e) noexcept
					: ex_{std::move(e)} {}

				template<receiver_of R>
				connect_result_t<E, R> connect(R&& r)
				{
					return execution::connect(std::move(ex_), std::forward<R>(r));
				}

				template<receiver_of R>
				connect_result_t<const E&, R> connect(R&& r) const
				{
					return execution::connect(ex_, std::forward<R>(r));
				}
			private:
				E ex_;
		};

		void schedule();

		template<class S>
		concept has_adl = sender<S> &&
			requires(S&& s)
		{
			schedule(std::forward<S>(s));
		};
			
		template<class S>
		concept can_member_call = sender<S> &&
			requires(S&& s)
		{
			std::forward<S>(s).schedule();
		};

		template<class S>
		concept can_as_sender = executor<S> &&
			requires(S&& s)
		{
			as_sender<std::remove_cvref_t<S>>{std::forward<S>(s)};
		};

		struct cpo
		{
			private:
				enum class state { none, member_call, non_member_call, as_sender_call };

				template<class S>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (can_member_call<S>)
					{
						return {state::member_call, noexcept(std::declval<S>().schedule())};
					}
					else if constexpr (has_adl<S>)
					{
						return {state::non_member_call, noexcept(schedule(std::declval<S>()))};
					}
					else if constexpr (can_as_sender<S>)
					{
						return {state::as_sender_call, noexcept(as_sender<std::remove_cvref_t<S>>{std::declval<S>()})};
					}
					else
					{
						return {state::none};
					}
				}

				template<class S>
				static constexpr auto choice = choose<S>();
			public:
				template<class S>
					requires(choice<S>.strategy != state::none)
				constexpr decltype(auto) operator()(S&& s) const noexcept(choice<S>.no_throw)
				{
					if constexpr (choice<S>.strategy == state::member_call)
					{
						return std::forward<S>(s).schedule();
					}
					else if constexpr (choice<S>.strategy == state::non_member_call)
					{
						return schedule(std::forward<S>(s));
					}
					else if constexpr (choice<S>.strategy == state::as_sender_call)
					{
						return as_sender<std::remove_cvref_t<S>>{std::forward<S>(s)};
					}
					else
					{
						static_assert(false, "should be unreachable");
					}
				}
		};
	}

	namespace cpos
	{
		export
		inline constexpr schedule_cpo::cpo schedule {};
	}

	using namespace cpos;

	export
	template<class S>
	concept scheduler = std::copy_constructible<std::remove_cvref_t<S>> &&
						std::equality_comparable<std::remove_cvref_t<S>> &&
		requires(S&& s)
	{
		execution::schedule(std::forward<S>(s));
	};
}