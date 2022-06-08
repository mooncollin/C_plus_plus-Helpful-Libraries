module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

#include <cstdint>
#include <concepts>
#include <type_traits>

export module cmoon.graphics.ellipse;

import cmoon.graphics.point2D;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_ellipse
	{
		using type = T;

		constexpr basic_ellipse() noexcept(std::is_nothrow_default_constructible_v<type>) = default;
		constexpr basic_ellipse(const basic_point2D<type>& center, const type& rad_x, const type& rad_y) noexcept
			: point{center}, radius_x{rad_x}, radius_y{rad_y} {}

		constexpr basic_ellipse(const basic_ellipse&) noexcept(std::is_nothrow_copy_constructible_v<type>) = default;
		constexpr basic_ellipse(basic_ellipse&&) noexcept(std::is_nothrow_move_constructible_v<type>) = default;

		constexpr basic_ellipse& operator=(const basic_ellipse&) noexcept(std::is_nothrow_copy_assignable_v<type>) = default;
		constexpr basic_ellipse& operator=(basic_ellipse&&) noexcept(std::is_nothrow_move_assignable_v<type>) = default;

		#ifdef _WIN32
		constexpr basic_ellipse(const ::D2D1_ELLIPSE& e) noexcept
			requires(std::convertible_to<FLOAT, type>)
			: point{e.point}, radius_x{e.radiusX}, radius_y{e.radiusY} {}

		constexpr basic_ellipse& operator=(const ::D2D1_ELLIPSE& e) noexcept
			requires(std::convertible_to<FLOAT, type>)
		{
			point = e.point;
			radius_x = e.radiusX;
			radius_y = e.radiusY;
			return *this;
		}

		[[nodiscard]] constexpr operator ::D2D1_ELLIPSE() const noexcept
			requires(std::convertible_to<type, FLOAT>)
		{
			return {.point = point, .radiusX = radius_x, .radiusY = radius_y};
		}
		#endif

		basic_point2D<type> point {};
		type radius_x {};
		type radius_y {};
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