#ifndef COLLIN_TYPE_TRAITS
#define COLLIN_TYPE_TRAITS

#include <type_traits>
#include <cstddef>
#include <functional>

namespace collin
{
	template<class T, class S, class = void>
	struct is_stream_readable : std::false_type
	{
	};

	template<class T, class S>
	struct is_stream_readable<S, T, std::void_t<decltype(std::declval<S&>() << std::declval<const T&>())>> : std::true_type
	{
	};

	template<class T, class S>
	constexpr bool is_stream_readable_v = is_stream_readable<S, T>::value;

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

#endif