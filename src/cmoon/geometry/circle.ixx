module;

#ifdef _WIN32
	#pragma comment(lib, "d2d1")
	#include <d2d1.h>
#endif

#include <numbers>

export module cmoon.geometry.circle;

import <numbers>;
import <compare>;
import <type_traits>;
import <cstdint>;

import cmoon.platform;

import cmoon.geometry.point;

namespace cmoon::geometry
{
	export
	template<class T>
	struct circle
	{
		using type = T;

		constexpr circle() noexcept(std::is_nothrow_default_constructible_v<T> &&
									std::is_nothrow_default_constructible_v<point2d<T>>) = default;

		explicit constexpr circle(const T& r, const point2d<T>& c = {}) noexcept(std::is_nothrow_copy_constructible_v<T> &&
																					 std::is_nothrow_copy_constructible_v<point2d<T>>)
			: radius{r}, center{c} {}

		[[nodiscard]] constexpr T diameter() const noexcept
		{
			return radius * 2;
		}

		[[nodiscard]] constexpr auto circumference() const noexcept
		{
			return std::numbers::pi_v<cmoon::least_float_type<T>> * diameter();
		}

		[[nodiscard]] constexpr auto area() const noexcept
		{
			return std::numbers::pi_v<cmoon::least_float_type<T>> * radius * radius;
		}

		[[nodiscard]] friend constexpr auto operator<=>(const circle&, const circle&) noexcept = default;

		point2d<T> center{};
		T radius{};
	};

	export
	using circle_f = circle<float>;

	export
	using circle_u32 = circle<std::uint32_t>;

	export
	using circle_u64 = circle<std::uint64_t>;

	export
	using circle_i32 = circle<std::int32_t>;

	export
	using circle_i64 = circle<std::int64_t>;
}