module;

#ifdef _WIN32
	#include <d2d1.h>
#endif

export module cmoon.graphics.point2D;

import <cstdint>;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_point2D
	{
		using type = T;

		constexpr basic_point2D() = default;
		constexpr basic_point2D(const T& t1, const T& t2)
			: x{t1}, y{t2} {}

		#ifdef _WIN32
		constexpr basic_point2D(const typename D2D1::TypeTraits<T>::Point& p)
			requires(requires {
				typename D2D1::TypeTraits<T>::Point;
			})
			: x{p.x}, y{p.y} {}

		constexpr basic_point2D& operator=(const typename D2D1::TypeTraits<T>::Point& p)
			requires(requires {
				typename D2D1::TypeTraits<T>::Point;
			})
		{
			x = p.x;
			y = p.y;

			return *this;
		}
			
		[[nodiscard]] constexpr operator typename D2D1::TypeTraits<T>::Point() const noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Point;
			})
		{
			return {.x = x, .y = y};
		}
		#endif

		T x {};
		T y {};
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