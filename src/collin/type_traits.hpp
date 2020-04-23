#pragma once

#include <type_traits>
#include <cstddef>
#include <functional>

namespace collin
{
	namespace type_traits
	{
		template<typename T, typename = void>
		constexpr bool is_type_complete = false;

		template<typename T>
		constexpr bool is_type_complete<T, std::void_t<decltype(sizeof(T))>> = true;

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
	}
}