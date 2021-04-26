module;

#ifdef _WIN32
	#include <d2d1.h>
#endif

export module cmoon.graphics.size2D;

import <cstdint>;
import <type_traits>;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_size2D
	{
		using type = T;

		constexpr basic_size2D() = default;

		constexpr basic_size2D(const T& w, const T& h) noexcept(std::is_nothrow_copy_constructible_v<T>)
			: width{w}, height{h} {}

		#ifdef _WIN32
		constexpr basic_size2D(const typename D2D1::TypeTraits<T>::Size& s) noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Size;
			})
			: width{s.width}, height{s.height} {}

		constexpr basic_size2D& operator=(const typename D2D1::TypeTraits<T>::Size& s) noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Size;
			})
		{
			width = s.width;
			height = s.height;
			return *this;
		}

		[[nodiscard]] constexpr operator typename D2D1::TypeTraits<T>::Size() const noexcept
			requires(requires {
				typename D2D1::TypeTraits<T>::Size;
			})
		{
			return {.width = width, .height = height};
		}
		#endif

		T width {};
		T height {};
	};

	export
	using size2D_f = basic_size2D<float>;

	export
	using size2D_u32 = basic_size2D<std::uint32_t>;

	export
	using size2D_u64 = basic_size2D<std::uint64_t>;

	export
	using size2D_i32 = basic_size2D<std::int32_t>;

	export
	using size2D_i64 = basic_size2D<std::int64_t>;
}