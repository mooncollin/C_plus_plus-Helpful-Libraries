export module cmoon.simd.simd_cast;

import <concepts>;
import <cstdint>;
import <algorithm>;

import <immintrin.h>;

import cmoon.simd.simd;
import cmoon.simd.fixed_size;
import cmoon.simd.impl.deduce_physical;
import cmoon.simd.impl.operations;

namespace cmoon
{
	export
	template<class V, class T, class Abi>
		requires(std::same_as<V, simd<T, Abi>>)
	[[nodiscard]] simd<T, Abi> simd_cast(const simd<T, Abi>& v) noexcept
	{
		return v;
	}

	export
	template<class V, class T, class Abi>
		requires(!cmoon::is_simd_v<V>)
	[[nodiscard]] simd<V, simd_abi::fixed_size<simd<T, Abi>::size()>> simd_cast(const simd<T, Abi>& v) noexcept
	{
		using old_simd_type = simd<T, Abi>;
		using new_simd_type = simd<V, simd_abi::fixed_size<simd<T, Abi>::size()>>;

		using old_op = simd_abi::simd_operations<typename old_simd_type::value_type,
												 typename old_simd_type::physical_type,
												 old_simd_type::size()>;

		using new_op = simd_abi::simd_operations<typename new_simd_type::value_type,
												 typename new_simd_type::physical_type,
												 new_simd_type::size()>;

		if constexpr (can_simd_convert<new_op, typename old_simd_type::value_type, typename old_simd_type::physical_type>)
		{
			return new_op::template convert<typename old_simd_type::value_type>(
				static_cast<typename old_simd_type::physical_type>(v)
			);
		}
		else
		{
			typename old_simd_type::value_type old_buf[old_op::physical_size()];
			typename new_simd_type::value_type new_buf[new_op::physical_size()];

			old_op::store_all(old_buf, static_cast<typename old_simd_type::physical_type>(v));

			std::transform(old_buf,
						   old_buf + old_simd_type::size(),
						   new_buf,
						   [](const auto& x) { return static_cast<typename new_simd_type::value_type>(x); });


			return new_simd_type{new_op::load_all(new_buf)};
		}
	}

	export
	template<class V, class T, class Abi>
		requires(cmoon::is_simd_v<V> &&
				 !std::same_as<V, simd<T, Abi>> &&
				 V::size() == simd<T, Abi>::size())
	[[nodiscard]] auto simd_cast(const simd<T, Abi>& v) noexcept
	{
		return simd_cast<typename V::value_type>(v);
	}
}