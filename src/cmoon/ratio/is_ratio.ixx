export module cmoon.ratio.is_ratio;

import <cstdint>;
import <ratio>;
import <type_traits>;

namespace cmoon
{
	export
	template<class T>
	struct is_ratio : std::false_type {};

	export
	template<std::intmax_t N, std::intmax_t D>
	struct is_ratio<std::ratio<N, D>> : std::true_type {};

	export
	template<class T>
	constexpr bool is_ratio_v = is_ratio<T>::value;

	export
	template<class T>
	concept ratio_type = is_ratio_v<T>;
}