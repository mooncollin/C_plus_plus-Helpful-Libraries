export module cmoon.algorithm.max;

import <type_traits>;
import <functional>;
import <concepts>;

namespace cmoon
{
	export
	template<typename T0, typename T1, typename... Ts>
	[[nodiscard]] constexpr std::add_const_t<std::common_reference_t<T0, T1, Ts...>> max(const T0& v0, const T1& v1, const Ts&... rest)
	{
		if (v1 < v0)
		{
			if constexpr (sizeof...(Ts) == 0)
			{
				return v0;
			}
			else
			{
				return max(v0, rest...);
			}
		}

		if constexpr (sizeof...(Ts) == 0)
		{
			return v1;
		}
		else
		{
			return max(v1, rest...);
		}
	}

	export
	template<class Compare, typename T0, typename T1, typename... Ts>
		requires(std::invocable<Compare, T0, T1>)
	[[nodiscard]] constexpr std::add_const_t<std::common_reference_t<T0, T1, Ts...>> max(Compare comp, const T0& v0, const T1& v1, const Ts&... rest)
	{
		if (std::invoke(comp, v1, v0))
		{
			if constexpr (sizeof...(Ts) == 0)
			{
				return v1;
			}
			else
			{
				return max(comp, v1, rest...);
			}
		}

		if constexpr (sizeof...(Ts) == 0)
		{
			return v0;
		}
		else
		{
			return max(comp, v0, rest...);
		}
	}
}