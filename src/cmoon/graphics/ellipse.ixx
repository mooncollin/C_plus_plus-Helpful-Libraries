module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

export module cmoon.graphics.ellipse;

import <concepts>;
import <cstdint>;

import cmoon.graphics.point2D;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_ellipse
	{
		using type = T;

		constexpr basic_ellipse() = default;
		constexpr basic_ellipse(const basic_point2D<T>& center, const T& rad_x, const T& rad_y) noexcept
			: point{center}, radius_x{rad_x}, radius_y{rad_y} {}

		#ifdef _WIN32
		constexpr basic_ellipse(const ::D2D1_ELLIPSE& e) noexcept
			requires(std::is_convertible_v<FLOAT, T>)
			: point{e.point}, radius_x{e.radiusX}, radius_y{e.radiusY} {}

		constexpr basic_ellipse& operator=(const ::D2D1_ELLIPSE& e) noexcept
			requires(std::is_convertible_v<FLOAT, T>)
		{
			point = e.point;
			radius_x = e.radiusX;
			radius_y = e.radiusY;
			return *this;
		}

		[[nodiscard]] constexpr operator ::D2D1_ELLIPSE() const noexcept
			requires(std::is_convertible_v<T, FLOAT>)
		{
			return {.point = point, .radiusX = radius_x, .radiusY = radius_y};
		}
		#endif

		basic_point2D<T> point {};
		T radius_x {};
		T radius_y {};
	};

	export
	using ellipse_f = basic_ellipse<float>;

	export
	using ellipse_u32 = basic_ellipse<std::uint32_t>;

	export
	using ellipse_u64 = basic_ellipse<std::uint64_t>;

	export
	using ellipse_i32 = basic_ellipse<std::int32_t>;

	export
	using ellipse_i64 = basic_ellipse<std::int64_t>;
}