export module cmoon.simd.native;

import cmoon.simd.compatible;

namespace cmoon::simd_abi
{
	export
	template<class T>
	using native = compatible<T>;
}