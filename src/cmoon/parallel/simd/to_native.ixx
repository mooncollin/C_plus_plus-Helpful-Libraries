export module cmoon.simd.to_native;

import <cstddef>;
import <algorithm>;

import cmoon.simd.simd;
import cmoon.simd.impl.operations;

namespace cmoon
{
	template<class T, std::size_t N>
	native_simd<T> to_native(const fixed_size_simd<T, N>& v) noexcept
	{
		if constexpr (N == fixed_size_simd<T, N>::size())
		{
			return native_simd<T>{v};
		}

		using old_simd_type = fixed_size_simd<T, N>;
		using new_simd_type = native_simd<T>;

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


	template<class T, std::size_t N>
	native_simd_mask<T> to_native(const fixed_size_simd_mask<T, N>& v) noexcept
	{
		if constexpr (N == fixed_size_simd_mask<T, N>::size())
		{
			return native_simd_mask<T>{v};
		}

		using old_simd_type = fixed_size_simd_mask<T, N>;
		using new_simd_type = native_simd_mask<T>;

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
}