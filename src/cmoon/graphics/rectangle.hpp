#pragma once

#include <cstdint>
#include <concepts>

#include "cmoon/graphics/point2D.hpp"

namespace cmoon
{
	namespace graphics
	{
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

		using rectangle_f = basic_rectangle<float>;
		using rectangle_u32 = basic_rectangle<std::uint32_t>;
		using rectangle_u64 = basic_rectangle<std::uint64_t>;
	}
}