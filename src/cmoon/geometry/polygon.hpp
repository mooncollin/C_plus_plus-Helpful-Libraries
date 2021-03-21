#pragma once

#include <cstddef>
#include <cmath>
#include <array>
#include <cmath>
#include <numbers>
#include <type_traits>
#include <concepts>
#include <initializer_list>

#include "cmoon/geometry/angle.hpp"
#include "cmoon/geometry/circle.hpp"

namespace cmoon
{
	namespace geometry
	{
		template<class Rep, std::size_t S>
			requires(S >= 3)
		class polygon
		{
			public:
				using rep = Rep;

				constexpr polygon() = default;

				template<std::same_as<Rep>... Sides>
					requires(sizeof...(Sides) == S)
				constexpr polygon(const Sides&... sides) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
					: sides_{{sides...}} {}

				template<std::size_t I>
					requires(I <= S && I != 0)
				constexpr void side(rep s) noexcept
				{
					sides_[I - 1] = s;
				}

				template<std::size_t I>
					requires(I <= S && I != 0)
				[[nodiscard]] constexpr const rep& side() const noexcept
				{
					return sides_[I - 1];
				}

				[[nodiscard]] constexpr const std::array<rep, S>& sides() const noexcept
				{
					return sides_;
				}

				[[nodiscard]] static constexpr angle sum_interior_angles() noexcept
				{
					return angle{S - 2, angle_type::radian};
				}

				[[nodiscard]] friend constexpr bool operator==(const polygon&, const polygon&) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const polygon&, const polygon&) noexcept = default;
			private:
				std::array<rep, S> sides_ {};
		};

		template<class Rep, std::size_t S>
			requires(S >= 3)
		class regular_polygon
		{
			public:
				using rep = Rep;

				constexpr regular_polygon() = default;
				constexpr regular_polygon(rep s) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
					: side_{s} {}

				constexpr void side(rep s) noexcept
				{
					side_ = s;
				}

				[[nodiscard]] constexpr const rep& side() const noexcept
				{
					return side_;
				}

				[[nodiscard]] static constexpr angle sum_interior_angles() noexcept
				{
					return angle{S - 2, angle_type::radian};
				}

				[[nodiscard]] friend constexpr bool operator==(const regular_polygon&, const regular_polygon&) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const regular_polygon&, const regular_polygon&) noexcept = default;
			private:
				rep side_ {};
		};

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool operator==(const polygon<Rep, S>& lhs, const regular_polygon<Rep, S>& rhs) noexcept
		{
			const auto& lhs_s = lhs.sides();
			const auto& s = rhs.side();
			return std::all_of(std::begin(lhs_s), std::end(lhs_s), [&s](const auto& side) { return side == s; });
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool operator==(const regular_polygon<Rep, S>& lhs, const polygon<Rep, S>& rhs) noexcept
		{
			return rhs == lhs;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool operator!=(const polygon<Rep, S>& lhs, const regular_polygon<Rep, S>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool operator!=(const regular_polygon<Rep, S>& lhs, const polygon<Rep, S>& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		template<std::size_t I, class Rep, std::size_t S>
			requires(I <= S && I != 0)
		[[nodiscard]] constexpr angle get_angle(const regular_polygon<Rep, S>&) noexcept
		{
			return regular_polygon<Rep, S>::sum_interior_angles() / S;
		}

		namespace details
		{
			template<class Rep, std::size_t... I>
			[[nodiscard]] constexpr std::array<angle, sizeof...(I)> get_angles_helper(const regular_polygon<Rep, sizeof...(I)>& p, std::index_sequence<I...>) noexcept
			{
				return {{get_angle<I + 1>(p)...}};
			}

			template<class Rep, std::size_t... I>
			[[nodiscard]] constexpr std::array<angle, sizeof...(I)> get_exterior_angles_helper(const regular_polygon<Rep, sizeof...(I)>& p, std::index_sequence<I...>) noexcept
			{
				return {{get_exterior_angle<I + 1>(p)...}};
			}
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr std::array<angle, S> get_angles(const regular_polygon<Rep, S>& p) noexcept
		{
			return details::get_angles_helper(p, std::make_index_sequence<S>{});
		}

		template<std::size_t I, class Rep, std::size_t S>
			requires(I <= S && I != 0)
		[[nodiscard]] constexpr angle get_exterior_angle(const regular_polygon<Rep, S>& p) noexcept
		{
			return angle::half() - get_angle<I>(p);
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr std::array<angle, S> get_exterior_angles(const regular_polygon<Rep, S>& p) noexcept
		{
			return details::get_exterior_angles_helper(p, std::make_index_sequence<S>{});
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr std::size_t number_sides(const regular_polygon<Rep, S>&) noexcept
		{
			return S;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr std::size_t number_sides(const polygon<Rep, S>&) noexcept
		{
			return S;
		}

		template<std::size_t I, class Rep, std::size_t S>
			requires(I <= S && I != 0)
		[[nodiscard]] constexpr Rep get_side(const regular_polygon<Rep, S>& p) noexcept
		{
			return p.side();
		}

		template<std::size_t I, class Rep, std::size_t S>
			requires(I <= S && I != 0)
		[[nodiscard]] constexpr Rep get_side(const polygon<Rep, S>& p) noexcept
		{
			return p.side<I>();
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr std::array<Rep, S> get_sides(const regular_polygon<Rep, S>& p) noexcept
		{
			std::array<Rep, S> results;
			std::fill(std::begin(results), std::end(results), p.side());
			return results;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto get_sides(const polygon<Rep, S>& p) noexcept
		{
			return p.sides();
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool similar(const regular_polygon<Rep, S>&, const regular_polygon<Rep, S>&) noexcept
		{
			return true;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool congruent(const regular_polygon<Rep, S>& lhs, const regular_polygon<Rep, S>& rhs) noexcept
		{
			return lhs == rhs;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto circumcircle(const regular_polygon<Rep, S>& p) noexcept
		{
			using common_type = std::common_type_t<Rep, long double>;

			const common_type numerator = p.side();
			const common_type denominator = 2.0L * std::sin(std::numbers::pi_v<long double> / S);
			
			const auto radius = numerator / denominator;

			return circle{radius};
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto apothem(const regular_polygon<Rep, S>& p) noexcept
		{
			using common_type = std::common_type_t<Rep, long double>;

			return static_cast<common_type>(p.side()) / 
				   static_cast<common_type>(2.0L * std::tan(std::numbers::pi_v<long double> / S));
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto incircle(const regular_polygon<Rep, S>& p) noexcept
		{
			return circle{apothem(p)};
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto area(const regular_polygon<Rep, S>& p) noexcept
		{
			return 0.25L * S * p.side() * p.side() * cmoon::cot(std::numbers::pi_v<long double> / S);
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto perimeter(const regular_polygon<Rep, S>& p) noexcept
		{
			return p.side() * S;
		}

		namespace details
		{
			template<class Rep, std::size_t... I>
			[[nodiscard]] constexpr auto perimeter_helper(const polygon<Rep, sizeof...(I)>& p, std::index_sequence<I...>) noexcept
			{
				return (... + p.sides()[I]);
			}
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto perimeter(const polygon<Rep, S>& p) noexcept
		{
			return details::perimeter_helper(p, std::make_index_sequence<S>{});
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto semi_perimeter(const regular_polygon<Rep, S>& p) noexcept
		{
			return perimeter(p) / 2;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr auto semi_perimeter(const polygon<Rep, S>& p) noexcept
		{
			return perimeter(p) / 2;
		}

		template<class Rep, std::size_t S>
		[[nodiscard]] constexpr bool regular(const regular_polygon<Rep, S>&) noexcept
		{
			return true;
		}

		/*
 		 * Standard functions all polygons should be able to used on.
		 * Not all specializations need to be constexpr. Extra requires
		 * clauses may be added to specializations. Extra template
		 * arguments may be added to specializations. No extra
		 * non-default arguments may be added.
		 *
		 * template<std::size_t I>
		 * [[nodiscard]] constexpr angle get_angle(POLYGON) noexcept;
		 * 
		 * template<std::size_t I>
		 * [[nodiscard]] constexpr angle get_exterior_angle(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr auto get_angles(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr auto get_exterior_angles(POLYGON) noexcept;
		 * 
		 * template<std::size_t I>
		 * [[nodiscard]] constexpr auto get_side(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr auto get_sides(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr std::size_t number_sides(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr bool similar(POLYGON, POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr bool congruent(POLYGON, POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr circle<REP> circumcircle(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr circle<REP> incircle(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr auto area(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr auto perimeter(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr auto semi_perimeter(POLYGON) noexcept;
		 * 
		 * [[nodiscard]] constexpr bool regular(POLYGON) noexcept;
		 */

		template<class T>
		concept polygon_type =
			requires(const T & t)
		{
			get_side<1>(t);
			get_sides(t);
			number_sides(t);
			//circumcircle(t);
			//incircle(t);
			//area(t);
			perimeter(t);
			semi_perimeter(t);
		};

		template<class T>
		concept full_polygon_type = 
			requires(const T& t)
		{
			get_angle<1>(t);
			get_angles(t);
			get_exterior_angle<1>(t);
			get_exterior_angles(t);
			get_side<1>(t);
			get_sides(t);
			number_sides(t);
			similar(t, t);
			congruent(t, t);
			circumcircle(t);
			incircle(t);
			area(t);
			perimeter(t);
			semi_perimeter(t);
			regular(t);
		};

		static_assert(polygon_type<regular_polygon<int, 3>>);
		static_assert(full_polygon_type<regular_polygon<int, 3>>);

		static_assert(polygon_type<polygon<int, 3>>);

		template<class Rep>
		using regular_triangle = regular_polygon<Rep, 3>;

		template<class Rep>
		using square = regular_polygon<Rep, 4>;

		template<class Rep>
		using regular_pentagon = regular_polygon<Rep, 5>;

		template<class Rep>
		using regular_hexagon = regular_polygon<Rep, 6>;

		template<class Rep>
		using regular_heptagon = regular_polygon<Rep, 7>;

		template<class Rep>
		using regular_octagon = regular_polygon<Rep, 8>;

		template<class Rep>
		using regular_nonagon = regular_polygon<Rep, 9>;

		template<class Rep>
		using regular_decagon = regular_polygon<Rep, 10>;

		template<class Rep>
		using regular_hendecagon = regular_polygon<Rep, 11>;

		template<class Rep>
		using regular_dodecagon = regular_polygon<Rep, 12>;

		template<class Rep>
		using regular_triskaidecagon = regular_polygon<Rep, 13>;

		template<class Rep>
		using regular_tetradecagon = regular_polygon<Rep, 14>;

		template<class Rep>
		using regular_pentadecagon = regular_polygon<Rep, 15>;

		template<class Rep>
		using regular_hexadecagon = regular_polygon<Rep, 16>;

		template<class Rep>
		using regular_heptadecagon = regular_polygon<Rep, 17>;

		template<class Rep>
		using regular_octadecagon = regular_polygon<Rep, 18>;

		template<class Rep>
		using regular_enneadecagon = regular_polygon<Rep, 19>;

		template<class Rep>
		using regular_icosagon = regular_polygon<Rep, 20>;

		template<class Rep>
		using regular_icosihenagon = regular_polygon<Rep, 21>;

		template<class Rep>
		using regular_icosidigon = regular_polygon<Rep, 22>;

		template<class Rep>
		using regular_icositrigon = regular_polygon<Rep, 23>;

		template<class Rep>
		using regular_icositetragon = regular_polygon<Rep, 24>;

		template<class Rep>
		using regular_icosipentagon = regular_polygon<Rep, 25>;

		template<class Rep>
		using regular_icosihexagon = regular_polygon<Rep, 26>;

		template<class Rep>
		using regular_icosiheptagon = regular_polygon<Rep, 27>;

		template<class Rep>
		using regular_icosioctagon = regular_polygon<Rep, 28>;

		template<class Rep>
		using regular_icosienneagon = regular_polygon<Rep, 29>;

		template<class Rep>
		using regular_triacontagon = regular_polygon<Rep, 30>;

		template<class Rep>
		using regular_tetracontagon = regular_polygon<Rep, 40>;

		template<class Rep>
		using regular_hectogon = regular_polygon<Rep, 100>;

		template<class Rep>
		using regular_chiliagon = regular_polygon<Rep, 1'000>;

		template<class Rep>
		using regular_myriagon = regular_polygon<Rep, 10'000>;

		template<class Rep>
		using regular_megagon = regular_polygon<Rep, 1'000'000>;

		template<class Rep>
		[[nodiscard]] constexpr auto area(const regular_triangle<Rep>& p) noexcept
		{
			return std::numbers::sqrt3_v<long double> / 4.0L * p.side() * p.side();
		}

		template<class Rep>
		[[nodiscard]] constexpr auto area(const square<Rep>& p) noexcept
		{
			return p.side() * p.side();
		}

		template<class Rep>
		[[nodiscard]] constexpr auto area(const regular_hexagon<Rep>& p) noexcept
		{
			return (1.5L * std::numbers::pi_v<long double>) * p.side() * p.side();
		}

		template<class Rep>
		[[nodiscard]] constexpr auto area(const regular_octagon<Rep>& p) noexcept
		{
			return 2.0L * (1.0L + std::numbers::sqrt2_v<long double>) * p.side() * p.side();
		}
	}
}