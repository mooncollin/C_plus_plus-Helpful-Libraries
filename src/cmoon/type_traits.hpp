#pragma once

#include <type_traits>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>
#include <optional>
#include <concepts>

namespace cmoon
{
	template<typename T>
	struct function_traits;

	template<typename R, typename... Args>
	struct function_traits<std::function<R(Args...)>>
	{
		static constexpr auto nargs = sizeof...(Args);
		using result_type = R;

		template<std::size_t i>
		struct arg
		{
			using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
		};
	};

	template<typename T, typename... Ts>
	constexpr bool is_any_of_v = std::disjunction_v<std::is_same<T, Ts>...>;

	template<typename...>
	struct is_unique;

	template<>
	struct is_unique<> : std::true_type {};

	template<typename F, typename... T>
	struct is_unique<F, T...> : std::bool_constant<is_unique<T...>::value && !is_any_of_v<F, T...>> {};

	template<typename... T>
	constexpr auto is_unique_v = is_unique<T...>::value;

	template<bool B, class T, class F, T t, F f>
	struct conditional_value;

	template<class T, class F, T t, F f>
	struct conditional_value<true, T, F, t, f>
	{
		static constexpr auto value = t;
	};

	template<class T, class F, T t, F f>
	struct conditional_value<false, T, F, t, f>
	{
		static constexpr auto value = f;
	};

	template<class T, T t, T t2>
	struct conditional_value<true, T, T, t, t2>
	{
		static constexpr auto value = t;
	};

	template<class T, T t, T t2>
	struct conditional_value<false, T, T, t, t2>
	{
		static constexpr auto value = t2;
	};

	template<bool B, class T, class F, T t, F f>
	constexpr auto conditional_value_v = conditional_value<B, T, F, t, f>::value;

	template<bool B, class T, T t, T t2>
	constexpr auto conditional_value2_v = conditional_value<B, T, T, t, t2>::value;

	template<typename T>
	struct is_optional : std::false_type {};

	template<typename T>
	struct is_optional<std::optional<T>> : std::true_type {};

	template<typename T>
	struct is_optional<std::optional<const T>> : std::true_type {};

	template<typename T>
	struct is_string_literal : std::false_type {};

	template<std::size_t N>
	struct is_string_literal<const char(&)[N]> : std::true_type {};

	template<std::size_t N>
	struct is_string_literal<const wchar_t(&)[N]> : std::true_type {};

	template<std::size_t N>
	struct is_string_literal<const char8_t(&)[N]> : std::true_type {};

	template<std::size_t N>
	struct is_string_literal<const char16_t(&)[N]> : std::true_type {};

	template<std::size_t N>
	struct is_string_literal<const char32_t(&)[N]> : std::true_type {};

	template<class T>
	constexpr auto is_string_literal_v = is_string_literal<T>::value;

	template<typename T>
	struct is_char_literal : std::false_type {};

	template<>
	struct is_char_literal<char> : std::true_type {};

	template<>
	struct is_char_literal<wchar_t> : std::true_type {};

	template<>
	struct is_char_literal<char8_t> : std::true_type {};

	template<>
	struct is_char_literal<char16_t> : std::true_type {};

	template<>
	struct is_char_literal<char32_t> : std::true_type {};

	template<class T>
	constexpr auto is_char_literal_v = is_char_literal<T>::value;

	struct nonesuch
	{
		~nonesuch() = delete;
		nonesuch(const nonesuch&) = delete;
		nonesuch& operator=(const nonesuch&) = delete;
	};

	namespace details
	{
		template<class Default, class AlwaysVoid, template<class...> class Op, class... Args>
		struct detector
		{
			using value_t = std::false_type;
			using type = Default;
		};

		template<class Default, template<class...> class Op, class... Args>
		struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
		{
			using value_t = std::true_type;
			using type = Op<Args...>;
		};
	}

	template<template<class...> class Op, class... Args>
	using is_detected = typename details::detector<nonesuch, void, Op, Args...>::value_t;

	template<template<class...> class Op, class... Args>
	using detected_t = typename details::detector<nonesuch, void, Op, Args...>::type;

	template<class Default, template<class...> class Op, class... Args>
	using detected_or = details::detector<Default, void, Op, Args...>;

	template<template<class...> class Op, class... Args>
	constexpr inline auto is_detected_v = is_detected<Op, Args...>::value;

	template<class Default, template<class...> class Op, class... Args>
	using detected_or_t = typename detected_or<Default, Op, Args...>::type;

	template<class Expected, template<class...> class Op, class... Args>
	using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

	template<class Expected, template<class...> class Op, class... Args>
	constexpr inline auto is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

	template<class To, template<class...> class Op, class... Args>
	using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

	template<class To, template<class...> class Op, class... Args>
	constexpr inline auto is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;
}