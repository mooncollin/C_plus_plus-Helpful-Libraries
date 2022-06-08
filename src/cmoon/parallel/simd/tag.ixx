export module cmoon.simd.tag;

import <cstddef>;
import <type_traits>;

namespace cmoon
{
	export
	struct element_aligned_tag {};
	
	export
	inline constexpr element_aligned_tag element_aligned {};

	export
	struct vector_aligned_tag {};

	export
	inline constexpr vector_aligned_tag vector_aligned {};

	export
	template<std::size_t N>
	struct overaligned_tag {};

	export
	template<std::size_t N>
	inline constexpr overaligned_tag<N> overaligned {};

	export
	template<class T>
	struct is_simd_flag_type : std::false_type {};

	export
	template<>
	struct is_simd_flag_type<element_aligned_tag> : std::true_type {};

	export
	template<>
	struct is_simd_flag_type<vector_aligned_tag> : std::true_type {};

	export
	template<std::size_t N>
	struct is_simd_flag_type<overaligned_tag<N>> : std::true_type {};

	export
	template<class T>
	inline constexpr auto is_simd_flag_type_v = is_simd_flag_type<T>::value;

	export
	template<class T>
	concept simd_flag_type = is_simd_flag_type_v<T>;
}