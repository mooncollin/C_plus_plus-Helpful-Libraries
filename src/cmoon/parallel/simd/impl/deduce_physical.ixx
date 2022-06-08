export module cmoon.simd.impl.deduce_physical;

import <cstddef>;
import <cstdint>;
import <type_traits>;

import <immintrin.h>;

import cmoon.simd.simd_size;

namespace cmoon::simd_abi
{
	export
	template<class T, std::size_t N>
	struct deduce_physical;

	export
	template<std::size_t N>
		requires(N <= simd_size_v<float, __m128>)
	struct deduce_physical<float, N> : std::type_identity<__m128> {};

	export
	template<std::size_t N>
		requires(N <= simd_size_v<double, __m128d>)
	struct deduce_physical<double, N> : std::type_identity<__m128d> {};

	export
	template<std::size_t N>
		requires(N <= simd_size_v<std::int8_t, __m128i>)
	struct deduce_physical<std::int8_t, N> : std::type_identity<__m128i> {};

	export
	template<std::size_t N>
		requires(N <= simd_size_v<std::int16_t, __m128i>)
	struct deduce_physical<std::int16_t, N> : std::type_identity<__m128i> {};

	export
	template<std::size_t N>
		requires(N <= simd_size_v<std::int32_t, __m128i>)
	struct deduce_physical<std::int32_t, N> : std::type_identity<__m128i> {};

	export
	template<std::size_t N>
		requires(N <= simd_size_v<std::int64_t, __m128i>)
	struct deduce_physical<std::int64_t, N> : std::type_identity<__m128i> {};

	export
	template<class T, std::size_t N>
	using deduce_physical_t = typename deduce_physical<T, N>::type;

	export
	template<class T, std::size_t N>
	concept has_physical_type = 
		requires
	{
		typename deduce_physical<T, N>::type;
	};
}