export module cmoon.simd.deduce;

import <cstddef>;
import <cstdint>;
import <type_traits>;

import <immintrin.h>;

import cmoon.simd.scalar;
import cmoon.simd.fixed_size;

namespace cmoon::simd_abi
{
	export
	template<class T, std::size_t N, class... Abis>
	struct deduce : std::type_identity<fixed_size<N>> {};

	export
	template<class T, class... Abis>
	struct deduce<T, 1, Abis...> : std::type_identity<scalar> {};

	export
	template<class... Abis>
	struct deduce<float, 4, Abis...> : std::type_identity<__m128> {};

	export
	template<class... Abis>
	struct deduce<double, 2, Abis...> : std::type_identity<__m128d> {};

	export
	template<class... Abis>
	struct deduce<std::int8_t, 16, Abis...> : std::type_identity<__m128i> {};

	export
	template<class... Abis>
	struct deduce<std::int16_t, 8, Abis...> : std::type_identity<__m128i> {};

	export
	template<class... Abis>
	struct deduce<std::int32_t, 4, Abis...> : std::type_identity<__m128i> {};

	export
	template<class... Abis>
	struct deduce<std::int64_t, 2, Abis...> : std::type_identity<__m128i> {};

	export
	template<class T, std::size_t N, class... Abis>
	using deduce_t = typename deduce<T, N, Abis...>::type;
}