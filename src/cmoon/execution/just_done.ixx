export module cmoon.execution.just_done;

import <utility>;
import <type_traits>;
import <concepts>;

import cmoon.concepts;

import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.receiver;
import cmoon.execution.set_done;
import cmoon.execution.sender;

namespace cmoon::execution
{
	template<class R>
	struct op
	{
		public:
			constexpr op(R&& r)
				: r_{std::forward<R>(r)} {}

			constexpr friend void tag_invoke(start_t, op& o) noexcept
			{
				execution::set_done(std::move(o.r_));
			}
		private:
			R r_;
	};

	struct just_done_sender
	{
		template<template<class...> class Tuple, template<class...> class Variant>
		using value_types = Variant<>;

		template<template<class...> class Variant>
		using error_types = Variant<>;

		static constexpr bool sends_done {true};

		template<receiver R>
		constexpr friend auto tag_invoke(connect_t, const just_done_sender&, R&& r)
		{
			return op<R>{std::forward<R>(r)};
		}
	};

	static_assert(cmoon::execution::sender<just_done_sender>);

	export
	constexpr just_done_sender just_done() noexcept
	{
		return {};
	}
}