export module cmoon.type_traits.is_future;

import <type_traits>;

namespace cmoon
{
	export
	template<class T>
	struct is_future : std::false_type {};

	export
	template<class T>
	struct is_future<std::future<T>> : std::true_type {};

	export
	template<class T>
	constexpr auto is_future_v = is_future<T>::value;
}