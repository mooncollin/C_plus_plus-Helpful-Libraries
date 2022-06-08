module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

#include <cstdint>
#include <concepts>
#include <type_traits>

export module cmoon.graphics.rectangle;

import cmoon.graphics.point2D;

namespace cmoon::graphics
{
	export
	template<std::default_initializable T>
	struct basic_rectangle
	{
		using type = T;

		constexpr basic_rectangle() noexcept(std::is_nothrow_default_constructible_v<basic_point2D<type>>) = default;
		constexpr basic_rectangle(const basic_rectangle&) noexcept(std::is_nothrow_copy_constructible_v<basic_point2D<type>>) = default;
		constexpr basic_rectangle(basic_rectangle&&) noexcept(std::is_nothrow_move_constructible_v<basic_point2D<type>>) = default;
		constexpr basic_rectangle& operator=(const basic_rectangle&) noexcept(std::is_nothrow_copy_assignable_v<basic_point2D<type>>) = default;
		constexpr basic_rectangle& operator=(basic_rectangle&&) noexcept(std::is_nothrow_move_assignable_v<basic_point2D<type>>) = default;

		constexpr basic_rectangle(const basic_point2D<type>& lt, const basic_point2D<type>& rb) noexcept(std::is_nothrow_copy_constructible_v<basic_point2D<type>>)
			: top_left{lt}, bottom_right{rb} {}

		constexpr basic_rectangle(const type& left, const type& top, const type& right, const type& bottom) noexcept(std::is_nothrow_constructible_v<basic_point2D<type>, type, type>)
			: top_left {left, top}, bottom_right {right, bottom} {}

		#ifdef _WIN32
		constexpr basic_rectangle(const typename D2D1::TypeTraits<type>::Rect& r) noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Rect;
			})
			: top_left{r.left, r.top}, bottom_right{r.right, r.bottom} {}

		constexpr basic_rectangle& operator=(const typename D2D1::TypeTraits<type>::Rect& r) noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Rect;
			})
		{
			top_left.x = r.left;
			top_left.y = r.top;
			bottom_right.x = r.right;
			bottom_right.y = r.bottom;

			return *this;
		}

		[[nodiscard]] constexpr operator typename D2D1::TypeTraits<type>::Rect() const noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Rect;
			})
		{
			return {.left = top_left.x, .top = top_left.y,
					.right = bottom_right.x, .bottom = bottom_right.y};
		}
		#endif

		basic_point2D<type> top_left {};
		basic_point2D<type> bottom_right {};
	};

	export
	using rectangle_f = basic_rectangle<float>;

	export
	using rectangle_u32 = basic_rectangle<std::uint32_t>;

	export
	using rectangle_u64 = basic_rectangle<std::uint64_t>;

	export
	using rectangle_i32 = basic_rectangle<std::int32_t>;

	export
	using rectangle_i64 = basic_rectangle<std::int64_t>;
}