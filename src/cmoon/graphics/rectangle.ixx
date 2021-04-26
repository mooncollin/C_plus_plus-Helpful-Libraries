export module cmoon.graphics.rectangle;

import <cstdint>;
import <concepts>;

import cmoon.graphics.point2D;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_rectangle
	{
		using type = T;

		constexpr basic_rectangle() = default;
		constexpr basic_rectangle(const basic_point2D<type>& lt, const basic_point2D<type>& rb) noexcept
			: top_left{lt}, bottom_right{rb} {}

		constexpr basic_rectangle(const type& left, const type& top, const type& right, const type& bottom) noexcept
			: top_left {left, top}, bottom_right {right, bottom} {}


		#ifdef _WIN32
		constexpr basic_rectangle(const typename D2D1::TypeTraits<T>::Rect& r) noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Rect;
			})
			: top_left{r.left, r.top}, bottom_right{r.right, r.bottom} {}

		constexpr basic_rectangle& operator=(const typename D2D1::TypeTraits<T>::Rect& r) noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Rect;
			})
		{
			top_left.x = r.left;
			top_left.y = r.top;
			bottom_right.x = r.right;
			bottom_right.y = r.bottom;

			return *this;
		}

		[[nodiscard]] constexpr operator typename D2D1::TypeTraits<T>::Rect() const noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Rect;
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