#pragma once

#include <cmath>
#include "collin/geometry/polygon.hpp"

namespace collin
{
	namespace geometry
	{
		template<class Rep>
		class rectangle
		{
			public:
				using rep = Rep;

				constexpr rectangle() = default;
				constexpr rectangle(const rep& s1, const rep& s2)
					: length_{s1}, width_{s2} {}

				constexpr void length(const rep& s) noexcept
				{
					length_ = s;
				}

				constexpr void width(const rep& s) noexcept
				{
					width_ = s;
				}

				[[nodiscard]] constexpr rep length() const noexcept
				{
					return length_;
				}

				[[nodiscard]] constexpr rep width() const noexcept
				{
					return width_;
				}

				[[nodiscard]] long double diagonal() const noexcept
				{
					return std::hypot(length_, width_);
				}

				template<class Rep2>
				[[nodiscard]] friend constexpr bool operator==(const rectangle& lhs, const rectangle<Rep2>& rhs) noexcept
				{
					return (lhs.length() == rhs.length() &&
						   lhs.width() == rhs.width()) ||
						   (lhs.length() == rhs.width() &&
						    lhs.width() == rhs.length());
				}

				template<class Rep2>
				[[nodiscard]] friend constexpr bool operator!=(const rectangle& lhs, const rectangle<Rep2>& rhs) noexcept
				{
					return !(lhs == rhs);
				}
			private:
				rep length_ {};
				rep width_ {};
		};

		template<class Rep>
		[[nodiscard]] constexpr std::size_t number_sides(const rectangle<Rep>& r) noexcept
		{
			return 4;
		}

		template<std::size_t I, class Rep>
			requires(I <= number_sides(rectangle<Rep>{}) && I != 0)
		[[nodiscard]] constexpr auto get_angle(const rectangle<Rep>&) noexcept
		{
			return angle::quarter();
		}

		template<class Rep>
		[[nodiscard]] constexpr std::array<angle, number_sides(rectangle<Rep>{})> get_angles(const rectangle<Rep>&) noexcept
		{
			return {angle::quarter(), angle::quarter(), angle::quarter(), angle::quarter()};
		}

		template<std::size_t I, class Rep>
			requires(I <= number_sides(rectangle<Rep>{}) && I != 0)
		[[nodiscard]] constexpr auto get_side(const rectangle<Rep>& r) noexcept
		{
			if constexpr (I == 1 || I == 3)
			{
				return s.length();
			}
			else if constexpr (I == 2 || I == 4)
			{
				return s.width();
			}
		}

		template<class Rep>
		[[nodiscard]] constexpr std::array<Rep, number_sides(rectangle<Rep>{})> get_sides(const rectangle<Rep>& r) noexcept
		{
			return {r.length(), r.width(), r.length(), r.width()};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool similar(const rectangle<Rep>& lhs, const rectangle<Rep2>& rhs) noexcept
		{
			return collin::math::rational{std::max(lhs.length(), lhs.width()), std::max(rhs.length(), rhs.width())}
					==
				   collin::math::rational{std::min(lhs.length(), lhs.width()), std::min(rhs.length(), rhs.width())};
		}

		template<class Rep, class Rep2>
		[[nodiscard]] constexpr bool congruent(const rectangle<Rep>& lhs, const rectangle<Rep2>& rhs) noexcept
		{
			return lhs == rhs;
		}

		template<class Rep>
		[[nodiscard]] constexpr auto circumcircle(const rectangle<Rep>& r) noexcept
		{
			return circle{r.diagonal() / 2.0L};
		}

		template<class Rep>
		[[nodiscard]] constexpr auto incircle(const rectangle<Rep>& r) noexcept
		{
			return circle{area(r) / static_cast<long double>(semi_perimeter(r))};
		}

		template<class Rep>
		[[nodiscard]] constexpr auto area(const rectangle<Rep>& r) noexcept
		{
			return r.length() * r.width();
		}

		template<class Rep>
		[[nodiscard]] constexpr auto perimeter(const rectangle<Rep>& r) noexcept
		{
			return 2 * (r.length() + r.width());
		}

		template<class Rep>
		[[nodiscard]] constexpr auto semi_perimeter(const rectangle<Rep>& r) noexcept
		{
			return r.length() + r.width();
		}
	}
}