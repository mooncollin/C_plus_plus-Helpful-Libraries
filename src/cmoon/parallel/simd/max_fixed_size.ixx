export module cmoon.simd.max_fixed_size;

import <cstddef>;
import <cstdint>;

import <immintrin.h>;

import cmoon.simd.simd_size;

namespace cmoon::simd_abi
{
	template<class T>
	struct max_fixed_size_impl;

	template<>
	struct max_fixed_size_impl<float> : simd_size<float, __m128> {};

	template<>
	struct max_fixed_size_impl<double> : simd_size<double, __m128d> {};

	template<>
	struct max_fixed_size_impl<std::int8_t> : simd_size<std::int8_t, __m128i> {};

	template<>
	struct max_fixed_size_impl<std::int16_t> : simd_size<std::int16_t, __m128i> {};

	template<>
	struct max_fixed_size_impl<std::int32_t> : simd_size<std::int32_t, __m128i> {};

	template<>
	struct max_fixed_size_impl<std::int64_t> : simd_size<std::int64_t, __m128i> {};

	export
	template<class T>
	inline constexpr auto max_fixed_size = max_fixed_size_impl<T>::value;
}