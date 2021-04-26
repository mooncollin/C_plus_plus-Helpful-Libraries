export module cmoon.geometry.circle;

import <numbers>;
import <compare>;
import <type_traits>;

namespace cmoon::geometry
{
	export
	template<class Rep>
	class circle
	{
		public:
			using rep = Rep;

			constexpr circle() = default;
			explicit constexpr circle(const rep& r) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
				: radius_{r} {}

			constexpr void radius(rep r) noexcept(std::is_nothrow_copy_assignable_v<Rep>)
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