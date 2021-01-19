#pragma once

#include <numbers>
#include <compare>

namespace cmoon
{
	namespace geometry
	{
		template<class Rep>
		class circle
		{
			public:
				using rep = Rep;

				constexpr circle() = default;
				constexpr circle(rep r)
					: radius_{r} {}

				constexpr void radius(rep r) noexcept
				{
					radius_ = r;
				}

				[[nodiscard]] constexpr rep radius() const noexcept
				{
					return radius_;
				}

				[[nodiscard]] constexpr rep diameter() const noexcept
				{
					return radius_ * 2;
				}

				[[nodiscard]] constexpr auto circumference() const noexcept
				{
					return std::numbers::pi_v<long double> * diameter();
				}

				[[nodiscard]] constexpr auto area() const noexcept
				{
					return std::numbers::pi_v<long double> * radius_ * radius_;
				}

				[[nodiscard]] friend constexpr auto operator<=>(const circle&, const circle&) noexcept = default;
			private:
				rep radius_ {};
		};
	}
}