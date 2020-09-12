#pragma once

#include <numbers>
#include <type_traits>

#include "collin/math.hpp"

namespace collin
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
			private:
				rep radius_ {};
		};

		template<class Rep>
		[[nodiscard]] constexpr auto area(const circle<Rep>& r) noexcept
		{
			return std::numbers::pi_v<long double> * r.radius() * r.radius();
		}
	}
}