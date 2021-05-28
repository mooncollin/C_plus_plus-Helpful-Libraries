export module cmoon.algorithm.min;

import <type_traits>;
import <utility>;

namespace cmoon
{
	export
	template<typename T>
	T min(T&& t)
	{
		return std::forward<T>(t);
	}

	export
	template<typename T0, typename T1, typename... Ts>
	typename std::common_type_t<T0, T1, Ts...> min(T0&& v0, T1&& v1, Ts&&... rest)
	{
		if (v1 < v0)
		{
			return min(std::forward<T1>(v1), std::forward<Ts>(rest)...);
		}

		return min(std::forward<T0>(v0), std::forward<Ts>(rest)...);
	}
}