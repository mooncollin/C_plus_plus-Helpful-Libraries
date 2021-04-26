export module cmoon.geometry.triangle;

import <optional>;
import <type_traits>;

import cmoon.geometry.polygon;
import cmoon.geometry.circle;

namespace cmoon::geometry
{
	export
	template<class Rep>
	class triangle;

	export
	template<class Rep>
	[[nodiscard]] constexpr bool valid_triangle(const triangle<Rep>& t) noexcept
	{
		return t.side<1>() + t.side<2>() > t.side<3>() &&
				t.side<2>() + t.side<3>() > t.side<1>() &&
				t.side<1>() + t.side<3>() > t.side<2>();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool valid_triangle(const regular_triangle<Rep>&) noexcept
	{
		return true;
	}

	export
	template<class Rep>
	class triangle : public polygon<Rep, 3>
	{
		public:
			using rep = Rep;

			constexpr triangle() = default;
			constexpr triangle(rep s1, rep s2, rep s3) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
				: polygon<Rep, 3>{s1, s2, s3} {}

			[[nodiscard]] friend constexpr bool operator==(const triangle&, const triangle&) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const triangle&, const triangle&) noexcept = default;

			template<class Rep2>
			[[nodiscard]] friend constexpr bool operator==(const triangle& lhs, const triangle<Rep2>& rhs) noexcept
			{
				return cmoon::geometry::get_sides(lhs) == cmoon::geometry::get_sides(rhs) &&
						cmoon::geometry::get_angles(lhs) == cmoon::geometry::get_angles(rhs);
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr bool operator!=(const triangle& lhs, const triangle<Rep2>& rhs) noexcept
			{
				return !(lhs == rhs);
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr bool operator==(const triangle& lhs, const regular_triangle<Rep2>& rhs) noexcept
			{
				return cmoon::geometry::get_sides(lhs) == cmoon::geometry::get_sides(rhs) &&
						cmoon::geometry::get_angles(lhs) == cmoon::geometry::get_angles(rhs);
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr bool operator!=(const triangle& lhs, const regular_triangle<Rep2>& rhs) noexcept
			{
				return !(lhs == rhs);
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr bool operator==(const regular_triangle<Rep2>& lhs, const triangle& rhs) noexcept
			{
				return cmoon::geometry::get_sides(lhs) == cmoon::geometry::get_sides(rhs) &&
						cmoon::geometry::get_angles(lhs) == cmoon::geometry::get_angles(rhs);
			}

			template<class Rep2>
			[[nodiscard]] friend constexpr bool operator!=(const regular_triangle<Rep2>& lhs, const triangle& rhs) noexcept
			{
				return !(lhs == rhs);
			}
	};

	export
	template<std::size_t I, class Rep>
		requires(I <= number_sides(triangle<Rep>{}) && I != 0)
	[[nodiscard]] constexpr auto get_angle(const triangle<Rep>& t) noexcept
	{
		long double sides_calculation;
		if constexpr (I == 1)
		{
			sides_calculation = (std::fma(t.side<2>(), t.side<2>(), 
									std::fma(t.side<3>(), t.side<3>(), -(t.side<1>() * t.side<1>()))))
									/ (2.0L * t.side<2>() * t.side<3>());
		}
		else if constexpr (I == 2)
		{
			sides_calculation = (std::fma(t.side<3>(), t.side<3>(),
									std::fma(t.side<1>(), t.side<1>(), -(t.side<2>() * t.side<2>()))))
									/ (2.0L * t.side<3>() * t.side<1>());
		}
		else if constexpr (I == 3)
		{
			sides_calculation = (std::fma(t.side<1>(), t.side<1>(),
									std::fma(t.side<2>(), t.side<2>(), -(t.side<3>() * t.side<3>()))))
									/ (2.0L * t.side<1>() * t.side<2>());
		}

		return cmoon::geometry::acos(sides_calculation);
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr std::array<angle, number_sides(triangle<Rep>{})> get_angles(const triangle<Rep>& t) noexcept
	{
		const auto side1_calculation = (std::fma(t.side<2>(), t.side<2>(), 
											std::fma(t.side<3>(), t.side<3>(), -(t.side<1>() * t.side<1>()))))
											/ (2.0L * t.side<2>() * t.side<3>());

		const auto side2_calculation = (std::fma(t.side<3>(), t.side<3>(), 
											std::fma(t.side<1>(), t.side<1>(), -(t.side<2>() * t.side<2>()))))
											/ (2.0L * t.side<3>() * t.side<1>());

		const auto angle1_calculation = std::acos(side1_calculation);
		const auto angle2_calculation = std::acos(side2_calculation);
		const auto angle3_calculation = std::numbers::pi - angle1_calculation - angle2_calculation;

		return {
			angle{angle1_calculation, angle_type::radian},
			angle{angle2_calculation, angle_type::radian},
			angle{angle3_calculation, angle_type::radian}
		};
	}

	export
	template<std::size_t I, class Rep>
		requires(I <= number_sides(triangle<Rep>{}) && I != 0)
	[[nodiscard]] constexpr auto get_exterior_angle(const triangle<Rep>& t) noexcept
	{
		return angle::half() - get_angle<I>(t);
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr std::array<angle, number_sides(triangle<Rep>{})> get_exterior_angles(const triangle<Rep>& t) noexcept
	{
		const auto angles = get_angles(t);
		return {
			angle::half() - std::get<0>(angles),
			angle::half() - std::get<1>(angles),
			angle::half() - std::get<2>(angles)
		};
	}

	export
	template<std::size_t I, class Rep>
		requires(I <= 3)
	[[nodiscard]] constexpr auto get_side(const triangle<Rep>& t) noexcept
	{
		return t.side<I>();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool similar(const triangle<Rep>& lhs, const triangle<Rep>& rhs) noexcept
	{
		const auto first_proportion = cmoon::rational{lhs.side<1>() * lhs.side<2>(), rhs.side<1>() * rhs.side<2>()};
		const auto second_proportion = cmoon::rational{lhs.side<2>() * lhs.side<3>(), rhs.side<2>() * rhs.side<3>()};
		const auto third_proportion = cmoon::rational{lhs.side<3>() * lhs.side<1>(), rhs.side<3>() * rhs.side<1>()};

		return first_proportion == second_proportion && 
				second_proportion == third_proportion && 
				lhs.angles() == rhs.angles();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool congruent(const triangle<Rep>& lhs, const triangle<Rep>& rhs) noexcept
	{
		return lhs == rhs;
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr auto circumcircle(const triangle<Rep>& t) noexcept
	{
		using common_type = std::common_type_t<Rep, long double>;

		const common_type numerator = t.side<1>() * t.side<2>() * t.side<3>();
		const common_type denominator = std::sqrt((t.side<1>() + t.side<2>() + t.side<3>()) *
													(t.side<2>() + t.side<3>() - t.side<1>()) *
													(t.side<1>() + t.side<2>() - t.side<3>()));

		const auto radius = numerator / denominator;

		return circle{radius};
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr auto incircle(const triangle<Rep>& t) noexcept
	{
		using common_type = std::common_type_t<Rep, long double>;

		const common_type denominator = semi_perimeter(t);
		const common_type numerator = (denominator - t.side<1>()) *
										(denominator - t.side<2>()) *
										(denominator - t.side<3>());

		const auto radius = std::sqrt(numerator / denominator);

		return circle{radius};
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr long double area(const triangle<Rep>& t) noexcept
	{
		if (regular(t))
		{
			return (std::numbers::sqrt3_v<long double> / 4) * t.side<1>() * t.side<1>();
		}

		const auto s = semi_perimeter(t);
		return std::sqrt(s * (s - t.side<1>()) * (s - t.side<2>()) * (s - t.side<3>()));
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr auto regular(const triangle<Rep>& t) noexcept
	{
		return t.side<1>() == t.side<2>() && t.side<2>() == t.side<3>();
	}

	export
	template<class Rep>
	[[nodiscard]] std::optional<Rep> hypotenuse(const triangle<Rep>& t) noexcept
	{
		const auto angles = t.angles();
		if (right_angle(std::get<0>(angles)))
		{
			return t.side<1>();
		}

		if (right_angle(std::get<1>(angles)))
		{
			return t.side<2>();
		}

		if (right_angle(std::get<2>(angles)))
		{
			return t.side<3>();
		}

		return {};
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr std::optional<Rep> hypotenuse(const regular_triangle<Rep>&) noexcept
	{
		return {};
	}

	export
	template<class Rep>
	[[nodiscard]] bool right_triangle(const triangle<Rep>& t) noexcept
	{
		const auto angles = t.angles();
		return right_angle(std::get<0>(angles)) ||
				right_angle(std::get<1>(angles)) ||
				right_angle(std::get<2>(angles));
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool right_triangle(const regular_triangle<Rep>&) noexcept
	{
		return false;
	}

	export
	template<class Rep>
	[[nodiscard]] bool obtuse_triangle(const triangle<Rep>& t) noexcept
	{
		const auto angles = t.angles();
		return obtuse_angle(std::get<0>(angles)) ||
				obtuse_angle(std::get<1>(angles)) ||
				obtuse_angle(std::get<2>(angles));
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool obtuse_triangle(const regular_triangle<Rep>&) noexcept
	{
		return false;
	}

	export
	template<class Rep>
	[[nodiscard]] bool acute_triangle(const triangle<Rep>& t) noexcept
	{
		const auto angles = t.angles();
		return acute_angle(std::get<0>(angles)) &&
				acute_angle(std::get<1>(angles)) &&
				acute_angle(std::get<2>(angles));
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool acute_triangle(const regular_triangle<Rep>&) noexcept
	{
		return true;
	}

	export
	template<class Rep>
	[[nodiscard]] bool oblique_triangle(const triangle<Rep>& t) noexcept
	{
		return !right_triangle(t);
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool oblique_triangle(const regular_triangle<Rep>&) noexcept
	{
		return true;
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool equilateral_triangle(const triangle<Rep>& t) noexcept
	{
		return regular(t);
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool equilateral_triangle(const regular_triangle<Rep>&) noexcept
	{
		return true;
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool isosceles_triangle(const triangle<Rep>& t) noexcept
	{
		const auto s12 = t.side<1>() == t.side<2>();
		const auto s13 = t.side<1>() == t.side<3>();
		const auto s23 = t.side<2>() == t.side<3>();

		return (s12 && !s13 && !s23) ||
			(s13 && !s12 && !s23) ||
			(s23 && !s12 && !s13);
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool isosceles_triangle(const regular_triangle<Rep>& t) noexcept
	{
		return false;
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool scalene_triangle(const triangle<Rep>& t) noexcept
	{
		return t.side<1>() != t.side<2>() &&
				t.side<1>() != t.side<3>() &&
				t.side<2>() != t.side<3>();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr bool scalene_triangle(const regular_triangle<Rep>& t) noexcept
	{
		return false;
	}

	static_assert(polygon_type<triangle<int>>);
}