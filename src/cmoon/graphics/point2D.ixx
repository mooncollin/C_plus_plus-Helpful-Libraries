module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

#include <cstdint>
#include <type_traits>

export module cmoon.graphics.point2D;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_point2D
	{
		using type = T;

		constexpr basic_point2D() noexcept(std::is_nothrow_default_constructible_v<type>) = default;
		constexpr basic_point2D(const type& t1, const type& t2) noexcept(std::is_nothrow_copy_constructible_v<type>)
			: x{t1}, y{t2} {}

		constexpr basic_point2D(const basic_point2D&) noexcept(std::is_nothrow_copy_constructible_v<type>) = default;
		constexpr basic_point2D(basic_point2D&&) noexcept(std::is_nothrow_move_constructible_v<type>) = default;

		constexpr basic_point2D& operator=(const basic_point2D&) noexcept(std::is_nothrow_copy_assignable_v<type>) = default;
		constexpr basic_point2D& operator=(basic_point2D&&) noexcept(std::is_nothrow_move_assignable_v<type>) = default;

		#ifdef _WIN32
		constexpr basic_point2D(const typename D2D1::TypeTraits<type>::Point& p) noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Point;
			})
			: x{p.x}, y{p.y} {}

		constexpr basic_point2D& operator=(const typename D2D1::TypeTraits<type>::Point& p) noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Point;
			})
		{
			x = p.x;
			y = p.y;

			return *this;
		}
			
		[[nodiscard]] constexpr operator typename D2D1::TypeTraits<type>::Point() const noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Point;
			})
		{
			return {.x = x, .y = y};
		}
		#endif

		type x {};
		type y {};
	};

	export
	using point2D_f = basic_point2D<float>;

	export
	using point2D_u32 = basic_point2D<std::uint32_t>;

	export
	using point2D_u64 = basic_point2D<std::uint64_t>;

	export
	using point2D_i32 = basic_point2D<std::int32_t>;

	export
	using point2D_i64 = basic_point2D<std::int64_t>;
}