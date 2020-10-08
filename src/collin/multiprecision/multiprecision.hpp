#pragma once

#include <cstdint>
#include <type_traits>

#include "collin/math.hpp"
#include "collin/platform.hpp"

namespace collin
{
	namespace multiprecision
	{
		enum class signed_type
		{
			signed_magnitude,
			unsigned_magnitude
		};

		enum class checked_type
		{
			checked,
			unchecked
		};

		using double_limb_type = std::uintmax_t;
		using signed_double_limb_type = std::make_signed_t<double_limb_type>;

		using limb_type = collin::platform::least_bits_unsigned_int<collin::platform::bits_in_type<double_limb_type> / 2>;
		using signed_limb_type = std::make_signed_t<limb_type>;

		constexpr limb_type digits_per_block_10 = std::numeric_limits<limb_type>::digits10;
		constexpr limb_type max_block_10 = collin::math::power_of_10<limb_type>(digits_per_block_10);

		constexpr auto bits_per_limb = collin::platform::bits_in_type<limb_type>;
		constexpr auto max_limb_value = std::numeric_limits<limb_type>::max();
		constexpr auto sign_bit_mask = static_cast<limb_type>(1u) << (bits_per_limb - 1);

		[[nodiscard]] constexpr limb_type block_multiplier(limb_type count)
		{
			return collin::math::power_of_10<limb_type>(count);
		}
	}
}