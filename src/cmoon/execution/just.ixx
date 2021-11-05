export module cmoon.execution.just;

import <tuple>;
import <type_traits>;
import <functional>;
import <concepts>;
import <exception>;
import <utility>;

import cmoon.execution.receiver;
import cmoon.execution.sender_base;
import cmoon.execution.set_error;
import cmoon.execution.set_value;
import cmoon.execution.operation_state;

namespace cmoon::execution
{
	template<typename T>
	concept moveable_value = std::move_constructible<std::remove_cvref_t<T>> &&
							 std::constructible_from<std::remove_cvref_t<T>, T>;

	template<receiver R, typename... Ts>
	struct op
	{
		public:
			constexpr op(R&& r, std::tuple<Ts...> ts)
				: r_{std::forward<R>(r)}, ts_{std::move(ts)} {}

			constexpr void start() && noexcept try
			{
				std::apply([this](Ts&&... ts) mutable {
					execution::set_value(std::move(r_), std::forward<Ts>(ts)...);
				}, std::move(ts_));
			}
			catch (...)
			{
				execution::set_error(std::move(r_), std::current_exception());
			}
		private:
			R r_;
			std::tuple<Ts...> ts_;
	};

	template<typename... Ts>
	class just_sender
	{
		template<class T>
		using augment_type = std::conditional_t<std::is_lvalue_reference_v<T>,
												std::reference_wrapper<std::remove_reference_t<T>>,
												std::remove_cvref_t<T>>;

		public:
			template<template<class...> class Tuple, template<class...> class Variant>
			using value_types = Tuple<augment_type<Ts>...>;

			template<template<class...> class Variant>
			using error_types = Variant<std::exception_ptr>;

			static constexpr bool sends_done {false};

			explicit constexpr just_sender(Ts&&... ts)
				: ts_{std::forward<Ts>(ts)...} {}

			template<receiver_of<Ts...> R>
				requires((std::copy_constructible<Ts> && ...))
			constexpr operation_state auto connect(R&& r) const &
			{
				return op<std::remove_cvref_t<R>, augment_type<Ts>...>{std::forward<R>(r), ts_};
			}

			template<receiver_of<Ts...> R>
			constexpr operation_state auto connect(R&& r) &&
			{
				return op<R, augment_type<Ts>...>{std::forward<R>(r), std::move(ts_)};
			}
		private:
			std::tuple<augment_type<Ts>...> ts_;
	};

	template<typename... Ts>
	just_sender(Ts&&...) -> just_sender<Ts...>;

	export
	template<moveable_value... Ts>
	auto just(Ts&&... ts)
	{
		return just_sender{std::forward<Ts>(ts)...};
	}
}