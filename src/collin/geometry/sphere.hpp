#pragma once

#include <numbers>

namespace collin
{
	namespace geometry
	{
		template<class Rep>
		class sphere
		{
			public:
				using rep = Rep;

				constexpr sphere() = default;
				constexpr sphere(rep r)
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
			private:
				rep radius_ {};
		};

		template<class Rep>
		[[nodiscard]] constexpr auto volume(const sphere<Rep>& s) noexcept
		{
			return (4.0L / 3.0L) * std::numbers::pi_v<long double> * s.radius() * s.radius() * s.radius();
		}

		template<class Rep>
		[[nodiscard]] constexpr auto surface_area(const sphere<Rep>& s) noexcept
		{
			return 4 * std::numbers::pi_v<long double> * s.radius() * s.radius();
		}
	}
}