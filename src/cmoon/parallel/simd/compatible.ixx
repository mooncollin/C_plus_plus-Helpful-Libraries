export module cmoon.simd.compatible;

import <type_traits>;
import <cstdint>;

import <immintrin.h>;

namespace cmoon::simd_abi
{
	template<class T>
	struct compatible_impl;

	template<>
	struct compatible_impl<std::int8_t> : std::type_identity<__m128i> {};

	template<>
	struct compatible_impl<std::int16_t> : std::type_identity<__m128i> {};

	template<>
	struct compatible_impl<std::int32_t> : std::type_identity<__m128i> {};

	template<>
	struct compatible_impl<std::int64_t> : std::type_identity<__m128i> {};

	template<>
	struct compatible_impl<float> : std::type_identity<__m128> {};

	template<>
	struct compatible_impl<double> : std::type_identity<__m128d> {};
	
	export
	template<class T>
	using compatible = typename compatible_impl<T>::type;
}