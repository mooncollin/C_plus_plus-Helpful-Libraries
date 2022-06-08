export module cmoon.simd.to_fixed_size;

import cmoon.simd.simd;
import cmoon.simd.simd_size;

namespace cmoon
{
	export
	template<class T, class Abi>
	fixed_size_simd<T, simd_abi::simd_size_v<T, Abi>> to_fixed_size(const simd<T, Abi>& x) noexcept
	{
		return fixed_size_simd<T, simd_abi::simd_size_v<T, Abi>>{static_cast<typename simd<T, Abi>::physical_type>(x)};
	}

	template<class T, class Abi>
	fixed_size_simd_mask<T, simd_abi::simd_size_v<T, Abi>> to_fixed_size(const simd_mask<T, Abi>& x) noexcept
	{
		return fixed_size_simd_mask<T, simd_abi::simd_size_v<T, Abi>>{static_cast<typename simd_mask<T, Abi>::physical_type>(x)};
	}
}