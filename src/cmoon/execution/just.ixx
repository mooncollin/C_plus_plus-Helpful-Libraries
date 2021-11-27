export module cmoon.execution.just;

import <tuple>;
import <type_traits>;
import <functional>;
import <concepts>;
import <exception>;
import <utility>;

import cmoon.concepts;
import cmoon.utility;
import cmoon.functional;

import cmoon.execution.receiver;
import cmoon.execution.sender_base;
import cmoon.execution.set_error;
import cmoon.execution.set_value;
import cmoon.execution.start;
import cmoon.execution.connect;

namespace cmoon::execution
{
	template<typename... Ts>
	class just_sender
	{
		template<receiver R>
		struct op
		{
			public:
				constexpr op(R&& r, std::tuple<Ts...> ts)
					: r_{std::forward<R>(r)}, ts_{std::move(ts)} {}

				constexpr friend void tag_invoke(start_t, op& s) noexcept
				{
					try
					{
						std::apply([&s](Ts&&... ts) {
							execution::set_value(std::move(s.r_), std::forward<Ts>(ts)...);
						}, std::move(s.ts_));
					}
					catch (...)
					{
						execution::set_error(std::move(s.r_), std::current_exception());
					}
				}
			private:
				R r_;
				std::tuple<Ts...> ts_;
		};

		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<Tuple<Ts...>>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr>;

			static constexpr bool sends_done {false};

			explicit constexpr just_sender(Ts&&... ts)
				: ts_{std::forward<Ts>(ts)...} {}

			template<receiver_of<Ts...> R>
				requires((std::copy_constructible<Ts> && ...))
			constexpr friend auto tag_invoke(connect_t, const just_sender& s, R&& r)
			{
				return op<R>{std::forward<R>(r), s.ts_};
			}

			template<receiver_of<Ts...> R>
			constexpr friend auto tag_invoke(connect_t, just_sender&& s, R&& r)
			{
				return op<R>{std::forward<R>(r), std::move(s.ts_)};
			}
		private:
			std::tuple<Ts...> ts_;
	};

	export
	template<cmoon::moveable_value... Ts>
	just_sender<std::decay_t<Ts>...> just(Ts&&... ts) noexcept((std::is_nothrow_constructible_v<std::decay_t<Ts>, Ts> && ...))
	{
		return just_sender<std::decay_t<Ts>...>{cmoon::decay_copy(std::forward<Ts>(ts))...};
	}
}