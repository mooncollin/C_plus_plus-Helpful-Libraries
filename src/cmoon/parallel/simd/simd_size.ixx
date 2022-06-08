export module cmoon.simd.simd_size;

import <type_traits>;
import <cstddef>;
import <cstdint>;

import <immintrin.h>;

import cmoon.simd.scalar;
import cmoon.simd.fixed_size;

namespace cmoon::simd_abi
{
	export
	template<class T, class Abi>
	struct simd_size;

	export
	template<class T>
	struct simd_size<T, scalar> : std::integral_constant<std::size_t, 1> {};

	export
	template<class T, std::size_t N>
	struct simd_size<T, fixed_size<N>> : std::integral_constant<std::size_t, N> {};

	export
	template<>
	struct simd_size<float, __m128> : std::integral_constant<std::size_t, 4> {};

	export
	template<>
	struct simd_size<double, __m128d> : std::integral_constant<std::size_t, 2> {};

	export
	template<>
	struct simd_size<std::int8_t, __m128i> : std::integral_constant<std::size_t, 16> {};

	export
	template<>
	struct simd_size<std::int16_t, __m128i> : std::integral_constant<std::size_t, 8> {};

	export
	template<>
	struct simd_size<std::int32_t, __m128i> : std::integral_constant<std::size_t, 4> {};

	export
	template<>
	struct simd_size<std::int64_t, __m128i> : std::integral_constant<std::size_t, 2> {};

	export
	template<class T, class Abi>
	inline constexpr auto simd_size_v = simd_size<T, Abi>::value;
}