module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

#include <cstdint>
#include <type_traits>

export module cmoon.graphics.size2D;

namespace cmoon::graphics
{
	export
	template<class T>
	struct basic_size2D
	{
		using type = T;

		constexpr basic_size2D() noexcept(std::is_nothrow_default_constructible_v<type>) = default;
		constexpr basic_size2D(const basic_size2D&) noexcept(std::is_nothrow_copy_constructible_v<type>) = default;
		constexpr basic_size2D(basic_size2D&&) noexcept(std::is_nothrow_move_assignable_v<type>) = default;
		constexpr basic_size2D& operator=(const basic_size2D&) noexcept(std::is_nothrow_copy_assignable_v<type>) = default;
		constexpr basic_size2D& operator=(basic_size2D&&) noexcept(std::is_nothrow_move_assignable_v<type>) = default;

		constexpr basic_size2D(const type& w, const type& h) noexcept(std::is_nothrow_copy_constructible_v<type>)
			: width{w}, height{h} {}

		#ifdef _WIN32
		constexpr basic_size2D(const typename D2D1::TypeTraits<type>::Size& s) noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Size;
			})
			: width{s.width}, height{s.height} {}

		constexpr basic_size2D& operator=(const typename D2D1::TypeTraits<type>::Size& s) noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Size;
			})
		{
			width = s.width;
			height = s.height;
			return *this;
		}

		[[nodiscard]] constexpr operator typename D2D1::TypeTraits<type>::Size() const noexcept
			requires(requires {
				typename D2D1::TypeTraits<type>::Size;
			})
		{
			return {.width = width, .height = height};
		}
		#endif

		type width {};
		type height {};
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