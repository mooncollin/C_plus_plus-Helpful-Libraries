#pragma once

#include <numbers>
#include <compare>
#include <type_traits>

namespace cmoon
{
	namespace geometry
	{
		template<class Rep>
		class sphere
		{
			public:
				using rep = Rep;

				constexpr sphere() = default;
				constexpr sphere(rep r) noexcept(std::is_nothrow_copy_constructible_v<rep>)
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

				[[nodiscard]] constexpr long double volume() const noexcept
				{
					return (4.0L / 3.0L) * std::numbers::pi_v<long double> * radius_ * radius_ * radius_;
				}

				[[nodiscard]] constexpr long double surface_area() const noexcept
				{
					return 4.0L * std::numbers::pi_v<long double> * radius_ * radius_;
				}

				[[nodiscard]] friend constexpr auto operator<=>(const sphere&, const sphere&) noexcept = default;
			private:
				rep radius_ {};
		};
	}
}