export module cmoon.algorithm.min;

import <type_traits>;
import <functional>;
import <concepts>;

namespace cmoon
{
	export
	template<typename T0, typename T1, typename... Ts>
	[[nodiscard]] constexpr std::add_const_t<std::common_reference_t<T0, T1, Ts...>> min(const T0& v0, const T1& v1, const Ts&... rest)
	{
		if (v0 < v1)
		{
			if constexpr (sizeof...(Ts) == 0)
			{
				return v0;
			}
			else
			{
				return min(v0, rest...);
			}
		}

		if constexpr (sizeof...(Ts) == 0)
		{
			return v1;
		}
		else
		{
			return min(v1, rest...);
		}
	}

	export
	template<class Compare, typename T0, typename T1, typename... Ts>
		requires(std::invocable<Compare, T0, T1>)
	[[nodiscard]] constexpr std::add_const_t<std::common_reference_t<T0, T1, Ts...>> min(Compare comp, const T0& v0, const T1& v1, const Ts&... rest)
	{
		if (std::invoke(comp, v0, v1))
		{
			if constexpr (sizeof...(Ts) == 0)
			{
				return v0;
			}
			else
			{
				return min(v0, rest...);
			}
		}

		if constexpr (sizeof...(Ts) == 0)
		{
			return v1;
		}
		else
		{
			return min(v1, rest...);
		}
	}
}