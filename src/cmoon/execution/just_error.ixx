export module cmoon.execution.just_error;

import <utility>;
import <type_traits>;
import <concepts>;

import cmoon.concepts;

import cmoon.execution.connect;
import cmoon.execution.start;
import cmoon.execution.receiver;
import cmoon.execution.set_error;
import cmoon.execution.sender_base;

namespace cmoon::execution
{
	template<class T>
	struct just_error_sender : public sender_base
	{
		template<class R>
		struct op
		{
			public:
				constexpr op(T t, R&& r)
					: err_{std::move(t)}, r_{std::forward<R>(r)} {}

				constexpr friend void tag_invoke(start_t, op& o) noexcept
				{
					execution::set_error(std::move(o.r_), std::move(o.err_));
				}
			private:
				T err_;
				R r_;
		};

		public:
			constexpr just_error_sender(T&& t)
				: err_{std::forward<T>(t)} {}

			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Variant<>;

			template<template<class...> class Variant>
			using error_types = Variant<T>;

			static constexpr bool sends_done {false};

			template<receiver<T> R>
				requires(std::copy_constructible<T>)
			constexpr friend auto tag_invoke(connect_t, const just_error_sender& s, R&& r)
			{
				return op<R>{s.err_, std::forward<R>(r)};
			}

			template<receiver<T> R>
			constexpr friend auto tag_invoke(connect_t, just_error_sender&& s, R&& r)
			{
				return op<R>{std::move(s.err_), std::forward<R>(r)};
			}
		private:
			T err_;
	};

	export
	template<cmoon::moveable_value T>
	constexpr just_error_sender<std::decay_t<T>> just_error(T&& t) noexcept(std::is_nothrow_constructible_v<std::decay_t<T>, T>)
	{
		return {std::forward<T>(t)};
	}
}