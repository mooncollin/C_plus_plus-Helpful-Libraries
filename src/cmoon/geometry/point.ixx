module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

export module cmoon.geometry.point;

import <cmath>;
import <numeric>;
import <type_traits>;

import cmoon.linear;

namespace cmoon::geometry
{
	export
	template<class Rep, std::size_t D>
	using point = cmoon::linear::static_matrix<Rep, 1, D>;

	export
	template<class Rep>
	class point1d : public point<Rep, 1>
	{
		using base = point<Rep, 1>;

		public:
			constexpr point1d() noexcept(std::is_nothrow_default_constructible_v<base>)
				: base{}, x{(*this)[0]} {}

			constexpr point1d(const base& b) noexcept(std::is_nothrow_copy_constructible_v<base>)
				: base{b}, x{(*this)[0]} {}

			constexpr point1d(base&& b) noexcept(std::is_nothrow_move_constructible_v<base>)
				: base{std::move(b)}, x{(*this)[0]} {}

			template<std::convertible_to<Rep> Rep2>
			constexpr point1d(const Rep2& r) noexcept(std::is_nothrow_constructible_v<base, Rep2>)
				: base{r}, x{(*this)[0]} {}

			constexpr point1d& operator=(const base& b) noexcept(std::is_nothrow_copy_assignable_v<base>)
			{
				base::operator=(b);

				return *this;
			}

			constexpr point1d& operator=(base&& b) noexcept(std::is_nothrow_move_constructible_v<base>)
			{
				base::operator=(std::move(b));

				return *this;
			}

			Rep& x;
	};

	export
	template<class Rep>
	class point2d : public point<Rep, 2>
	{
		using base = point<Rep, 2>;

		public:
			constexpr point2d() noexcept(std::is_nothrow_default_constructible_v<base>)
				: base{}, x{(*this)[0]}, y{(*this)[1]} {}

			constexpr point2d(const base& b) noexcept(std::is_nothrow_copy_constructible_v<base>)
				: base{b}, x{(*this)[0]}, y{(*this)[1]} {}

			constexpr point2d(base&& b) noexcept(std::is_nothrow_move_constructible_v<base>)
				: base{std::move(b)}, x{(*this)[0]}, y{(*this)[1]} {}

			template<std::convertible_to<Rep> Rep2, std::convertible_to<Rep> Rep3>
			constexpr point2d(const Rep2& r, const Rep3 r2) noexcept(std::is_nothrow_constructible_v<base, Rep2, Rep3>)
				: base{r, r2}, x{(*this)[0]}, y{(*this)[1]} {}

			constexpr point2d& operator=(const base& b) noexcept(std::is_nothrow_copy_assignable_v<base>)
			{
				base::operator=(b);

				return *this;
			}

			constexpr point2d& operator=(base&& b) noexcept(std::is_nothrow_move_constructible_v<base>)
			{
				base::operator=(std::move(b));

				return *this;
			}

			#ifdef _WIN32
			constexpr point2d(const typename D2D1::TypeTraits<Rep>::Point& p) noexcept
				requires(requires {
					typename D2D1::TypeTraits<Rep>::Point;
				})
				: base{p.x, p.y}, x{(*this)[0]}, y{(*this)[1]} {}

			constexpr point2d& operator=(const typename D2D1::TypeTraits<Rep>::Point& p) noexcept
				requires(requires {
					typename D2D1::TypeTraits<Rep>::Point;
				})
			{
				x = p.x;
				y = p.y;

				return *this;
			}
				
			[[nodiscard]] constexpr operator typename D2D1::TypeTraits<Rep>::Point() const noexcept
				requires(requires {
					typename D2D1::TypeTraits<Rep>::Point;
				})
			{
				return {.x = x, .y = y};
			}
			#endif

			Rep& x;
			Rep& y;
	};

	export
	template<class Rep>
	class point3d : public point<Rep, 3>
	{
		using base = point<Rep, 3>;

		public:
			constexpr point3d() noexcept(std::is_nothrow_default_constructible_v<base>)
				: base{}, x{(*this)[0]}, y{(*this)[1]}, z{(*this)[2]} {}

			constexpr point3d(const base& b) noexcept(std::is_nothrow_copy_constructible_v<base>)
				: base{b}, x{(*this)[0]}, y{(*this)[1]}, z{(*this)[2]} {}

			constexpr point3d(base&& b) noexcept(std::is_nothrow_move_constructible_v<base>)
				: base{std::move(b)}, x{(*this)[0]}, y{(*this)[1]}, z{(*this)[2]} {}

			template<std::convertible_to<Rep> Rep2, std::convertible_to<Rep> Rep3, std::convertible_to<Rep> Rep4>
			constexpr point3d(const Rep2& r, const Rep3 r2, const Rep4 r3) noexcept(std::is_nothrow_constructible_v<base, Rep2, Rep3, Rep4>)
				: base{r, r2, r3}, x{(*this)[0]}, y{(*this)[1]}, z{(*this)[2]} {}

			constexpr point3d& operator=(const base& b) noexcept(std::is_nothrow_copy_assignable_v<base>)
			{
				base::operator=(b);

				return *this;
			}

			constexpr point3d& operator=(base&& b) noexcept(std::is_nothrow_move_constructible_v<base>)
			{
				base::operator=(std::move(b));

				return *this;
			}

			Rep& x;
			Rep& y;
			Rep& z;
	};

	export
	template<class Rep>
	[[nodiscard]] point1d<Rep> normalize(const point1d<Rep>& p) noexcept
	{
		return p;
	}

	export
	template<class Rep>
	[[nodiscard]] point2d<Rep> normalize(const point2d<Rep>& p) noexcept
	{
		return p / std::hypot(p.x, p.y);
	}

	export
	template<class Rep>
	[[nodiscard]] point3d<Rep> normalize(const point3d<Rep>& p) noexcept
	{
		return p / std::hypot(p.x, p.y, p.z);
	}

	export
	template<class Rep, std::size_t D>
	[[nodiscard]] point<Rep, D> normalize(const point<Rep, D>& p) noexcept
	{
		const auto sum_of_squares = std::reduce(std::begin(p), std::end(p), Rep{}, [](const auto& sum, const auto& next) { return sum + next * next; });
		const auto division = std::sqrt(sum_of_squares);

		return p / division;
	}
}