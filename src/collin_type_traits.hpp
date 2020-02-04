#ifndef COLLIN_TYPE_TRAITS
#define COLLIN_TYPE_TRAITS

#include <type_traits>

namespace collin
{
	template<class S, class T, class = void>
	struct is_streamable : std::false_type
	{
	};

	template<class S, class T>
	struct is_streamable<S, T, std::void_t<decltype(std::declval<S&>() << std::declval<const T&>())>> : std::true_type
	{
	};

	template<class S, class T>
	constexpr bool is_streamable_v = is_streamable<S, T>::value;

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
}

#endif