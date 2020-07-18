#pragma once

#include <type_traits>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>

namespace collin
{
	namespace type_traits
	{
		template<class T, typename = void>
		struct is_type_complete : std::false_type {};

		template<class T>
		struct is_type_complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

		template<class T>
		constexpr bool is_type_complete_v = is_type_complete<T>::value;

		template<class T, class S, class = void>
		struct is_stream_writable : std::false_type
		{
		};

		template<class T, class S>
		struct is_stream_writable<S, T, std::void_t<decltype(std::declval<S&>() << std::declval<const T&>())>> : std::true_type
		{
		};

		template<class T, class S>
		constexpr bool is_stream_writable_v = is_stream_writable<S, T>::value;

		template<class T, class = void>
		struct has_size : std::false_type
		{
		};

		template<class T>
		struct has_size<T, std::void_t<decltype(std::size(std::declval<T>()))>> : std::true_type
		{
		};

		template<class T>
		constexpr bool has_size_v = has_size<T>::value;

		template<class T, typename = void>
		struct is_iterator : std::false_type {};

		template<class T>
		struct is_iterator<T, std::void_t<decltype(*std::declval<T>())>> : std::true_type {};

		template<class T>
		constexpr bool is_iterator_v = is_iterator<T>::value;

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
		struct contains : std::disjunction<std::is_same<T, Ts>...> {};

		template<typename T, typename... Ts>
		constexpr auto contains_v = contains<T, Ts...>::value;

		template<typename...>
		struct is_unique;

		template<>
		struct is_unique<> : std::true_type {};

		template<typename F, typename... T>
		struct is_unique<F, T...> : std::bool_constant<is_unique<T...>::value && !contains_v<F, T...>> {};

		template<typename... T>
		constexpr auto is_unique_v = is_unique<T...>::value;
	}
}