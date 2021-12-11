export module cmoon.executors.inline_scheduler;

import <exception>;
import <type_traits>;
import <utility>;
import <concepts>;

import cmoon.execution;

namespace cmoon::executors
{
	template<class R>
	struct inline_op
	{
		[[no_unique_address]] R r;

		constexpr friend void tag_invoke(cmoon::execution::start_t, inline_op& o) noexcept
		{
			try
			{
				cmoon::execution::set_value(std::move(o.r));
			}
			catch (...)
			{
				cmoon::execution::set_error(std::move(o.r), std::current_exception());
			}
		}
	};

	struct inline_sender
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Variant<Tuple<>>;

		template<template<class...> class Variant>
		using error_types = Variant<std::exception_ptr>;

		static constexpr bool sends_done {false};

		template<cmoon::execution::receiver_of R>
		constexpr friend inline_op<std::decay_t<R>> tag_invoke(cmoon::execution::connect_t, inline_sender, R&& r) noexcept(std::is_nothrow_constructible_v<std::decay_t<R>, R>)
		{
			return {std::forward<R>(r)};
		}
	};

	export
	struct inline_scheduler
	{
		constexpr friend inline_sender tag_invoke(cmoon::execution::schedule_t, const inline_scheduler&) noexcept
		{
			return {};
		}

		[[nodiscard]] constexpr friend bool operator==(const inline_scheduler&, const inline_scheduler&) noexcept = default;
	};
}