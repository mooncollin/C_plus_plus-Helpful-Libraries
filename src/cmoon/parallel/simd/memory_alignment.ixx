export module cmoon.simd.memory_alignment;

import <cstddef>;
import <type_traits>;

namespace cmoon
{
	export
	template<class T, class U = typename T::value_type>
	struct memory_alignment : std::integral_constant<std::size_t, alignof(U)> {};

	template<class T, class U = typename T::value_type>
	inline constexpr std::size_t memory_alignment_v = memory_alignment<T, U>::value;
}