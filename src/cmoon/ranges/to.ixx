export module cmoon.ranges.to;

import <ranges>;
import <concepts>;
import <iterator>;
import <tuple>;

namespace cmoon::ranges
{
	template<class T, class R>
	concept compatible_range = std::ranges::input_range<R> &&
							   std::constructible_from<T, std::ranges::range_reference_t<R>> &&
							   !std::convertible_to<std::ranges::range_reference_t<R>, T>;

	export
	struct from_range_t
	{
		explicit from_range_t() = default;
	};

	export
	inline constexpr from_range_t from_range {};

	template<class C>
	concept can_insert = std::ranges::range<C> &&
		requires(C c)
	{
		std::inserter(c, std::ranges::end(c));
	};

	template<class C, class R>
	concept reservable_container = std::ranges::sized_range<R> &&
		requires(C& c, R&& r)
	{
		{ c.capacity() } -> std::same_as<std::ranges::range_size_t<R>>;
		c.reserve(c.capacity());
	};

	export
	template<class C, std::ranges::input_range R, class... Args>
		requires(!std::ranges::view<C>)
	constexpr C to(R&& r, Args&&... args)
	{
		if constexpr (std::constructible_from<C, R, Args...>)
		{
			return C(std::forward<R>(r), std::forward<Args>(args)...);
		}
		else if constexpr (std::constructible_from<C, from_range_t, R, Args...>)
		{
			return C(from_range, std::forward<R>(r), std::forward<Args>(args)...);
		}
		else if constexpr (std::constructible_from<C, Args...> &&
						   std::indirectly_copyable<std::ranges::iterator_t<R>, std::ranges::iterator_t<C>> &&
						   can_insert<C>)
		{
			C c(std::forward<Args>(args)...);
			if constexpr (reservable_container<C, R>)
			{
				c.reserve(std::ranges::size(r));
			}

			std::ranges::copy(r, std::inserter(c, std::ranges::end(c)));
			return c;
		}
		else if constexpr (std::ranges::input_range<std::ranges::range_value_t<C>> &&
						   std::ranges::input_range<std::ranges::range_value_t<R>> &&
						   !std::ranges::view<std::ranges::range_value_t<C>> &&
						   std::indirectly_copyable<std::ranges::iterator_t<std::ranges::range_reference_t<R>>,
						   std::ranges::iterator_t<std::ranges::range_value_t<C>>> &&
						   can_insert<C>)
		{
			C c(std::forward<Args>(args)...);
			if constexpr (reservable_container<C, R>)
			{
				c.reserve(std::ranges::size(r));
			}

			auto v = r | std::ranges::views::transform([](auto&& elem) {
				return to<range_value_t<C>>(elem);
			});

			std::ranges::copy(v, std::inserter(c, std::ranges::end(c)));
			return c;
		}
	}

	template<template<class...> class C, std::ranges::input_range R, class... Args>
	struct deduce_type;

	template<template<class...> class C, std::ranges::input_range R, class... Args>
		requires(requires(C c, R r, Args... args) {
			C(r, args...);
		})
	struct deduce_type<C, R, Args...> : std::type_identity_t<decltype(C(std::declval<R>(), std::declval<Args>()...))> {};

	template<template<class...> class C, std::ranges::input_range R, class... Args>
		requires(requires(C c, R r, Args... args) {
			C(from_range, r, args...);
		})
	struct deduce_type<C, R, Args...> : std::type_identity_t<decltype(C(from_range, std::declval<R>(), std::declval<Args>()...))> {};

	template<template<class...> class C, std::ranges::input_range R, class... Args>
		requires(requires(C c, std::ranges::iterator_t<R> iter, Args... args) {
			C(iter, iter, args...);
		})
	struct deduce_type<C, R, Args...> : std::type_identity_t<decltype(C(std::declval<std::ranges::iterator_t<R>>(), std::declval<std::ranges::iterator_t<R>>(), std::declval<Args>()...))> {};

	template<template<class...> class C, std::ranges::input_range R, class... Args>
	using deduce_type_t = typename deduce_type<C, R, Args...>::type;

	export
	template<template<class...> class C, std::ranges::input_range R, class... Args>
	constexpr deduce_type_t<C, R, Args...> to(R&& r, Args&&... args)
	{
		return to<deduce_type_t<C, R, Args...>>(std::forward<R>(R), std::forward<Args>(args)...);
	}
}