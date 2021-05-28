export module cmoon.multiprecision.properties;

import <cstdint>;
import <type_traits>;
import <limits>;

import cmoon.math;
import cmoon.platform;

namespace cmoon::multiprecision
{
	export
	enum class signed_type
	{
		signed_magnitude,
		unsigned_magnitude
	};

	export
	enum class checked_type
	{
		checked,
		unchecked
	};

	export
	using double_limb_type = std::uintmax_t;

	export
	using signed_double_limb_type = std::make_signed_t<double_limb_type>;

	export
	using limb_type = cmoon::least_bits_unsigned_int<cmoon::bits_in_type<double_limb_type> / 2>;

	export
	using signed_limb_type = std::make_signed_t<limb_type>;

	export
	inline constexpr limb_type digits_per_block_10 {std::numeric_limits<limb_type>::digits10};

	export
	inline constexpr limb_type max_block_10 {cmoon::power_of_10<limb_type>(digits_per_block_10)};

	export
	inline constexpr auto bits_per_limb {cmoon::bits_in_type<limb_type>};

	export
	inline constexpr auto max_limb_value {std::numeric_limits<limb_type>::max()};

	export
	inline constexpr auto sign_bit_mask {static_cast<limb_type>(1u) << (bits_per_limb - 1)};

	export
	[[nodiscard]] constexpr limb_type block_multiplier(limb_type count) noexcept
	{
		return cmoon::power_of_10<limb_type>(count);
	}
}