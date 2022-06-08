export module cmoon.simd.is_abi_tag;

import <type_traits>;
import <cstddef>;

import <immintrin.h>;

import cmoon.simd.native;
import cmoon.simd.compatible;
import cmoon.simd.fixed_size;
import cmoon.simd.scalar;

namespace cmoon
{
	export
	template<class T>
	struct is_abi_tag : std::false_type {};

	export
	template<std::size_t N>
	struct is_abi_tag<cmoon::simd_abi::fixed_size<N>> : std::true_type {};

	export
	template<>
	struct is_abi_tag<cmoon::simd_abi::scalar> : std::true_type {};

	export
	template<>
	struct is_abi_tag<__m128> : std::true_type {};

	export
	template<>
	struct is_abi_tag<__m128i> : std::true_type {};

	export
	template<>
	struct is_abi_tag<__m128d> : std::true_type {};

	export
	template<class T>
	inline constexpr bool is_abi_tag_v = is_abi_tag<T>::value;
}