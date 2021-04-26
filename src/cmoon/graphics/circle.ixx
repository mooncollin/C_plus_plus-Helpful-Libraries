module;

#ifdef _WIN32
	#include <d2d1.h>
#endif

export module cmoon.graphics.circle;

import <cstdint>;
import <concepts>;

import cmoon.graphics.point2D;
import cmoon.graphics.ellipse;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_circle
	{
		constexpr basic_circle() = default;
		constexpr basic_circle(const basic_point2D<T>& center, const T& r)
			: point{center}, radius{r} {}

		[[nodiscard]] constexpr operator basic_ellipse<T>() const noexcept
		{
			return {point, radius, radius};
		}

		#ifdef _WIN32
		[[nodiscard]] constexpr operator ::D2D1_ELLIPSE() const noexcept
			requires(std::is_convertible_v<T, FLOAT>)
		{
			return {.point = point, .radiusX = radius, .radiusY = radius};
		}
		#endif

		basic_point2D<T> point {};
		T radius {};
	};

	export
	using circle_f = basic_circle<float>;

	export
	using circle_u32 = basic_circle<std::uint32_t>;

	export
	using circle_u64 = basic_circle<std::uint64_t>;

	export
	using circle_i32 = basic_circle<std::int32_t>;

	export
	using circle_i64 = basic_circle<std::int64_t>;
}