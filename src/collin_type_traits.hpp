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
}

#endif