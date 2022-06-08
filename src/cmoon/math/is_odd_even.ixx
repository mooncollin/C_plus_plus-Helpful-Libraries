export module cmoon.math.is_odd_even;

import <concepts>;
import <functional>;

import cmoon.concepts;

namespace cmoon
{
	export
	constexpr auto is_odd {
		[]<std::integral T>(const T val) noexcept {
			return static_cast<bool>(val & 1);
		}
	};

	export
	constexpr auto is_even {std::not_fn(is_odd)};
}