export module cmoon.geometry.polygon;

import <cstddef>;
import <cmath>;
import <array>;
import <cmath>;
import <numbers>;
import <type_traits>;
import <concepts>;
import <initializer_list>;

import cmoon.geometry.angle;
import cmoon.geometry.circle;

namespace cmoon::geometry
{
	export
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

	export
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

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr bool operator==(const polygon<Rep, S>& lhs, const regular_polygon<Rep, S>& rhs) noexcept
	{
		const auto& lhs_s = lhs.sides();
		const auto& s = rhs.side();
		return std::all_of(std::begin(lhs_s), std::end(lhs_s), [&s](const auto& side) { return side == s; });
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr bool operator==(const regular_polygon<Rep, S>& lhs, const polygon<Rep, S>& rhs) noexcept
	{
		return rhs == lhs;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr bool operator!=(const polygon<Rep, S>& lhs, const regular_polygon<Rep, S>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr bool operator!=(const regular_polygon<Rep, S>& lhs, const polygon<Rep, S>& rhs) noexcept
	{
		return !(lhs == rhs);
	}

	export
	template<std::size_t I, class Rep, std::size_t S>
		requires(I <= S && I != 0)
	[[nodiscard]] constexpr angle get_angle(const regular_polygon<Rep, S>&) noexcept
	{
		return regular_polygon<Rep, S>::sum_interior_angles() / S;
	}

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

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr std::array<angle, S> get_angles(const regular_polygon<Rep, S>& p) noexcept
	{
		return get_angles_helper(p, std::make_index_sequence<S>{});
	}

	export
	template<std::size_t I, class Rep, std::size_t S>
		requires(I <= S && I != 0)
	[[nodiscard]] constexpr angle get_exterior_angle(const regular_polygon<Rep, S>& p) noexcept
	{
		return angle::half() - get_angle<I>(p);
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr std::array<angle, S> get_exterior_angles(const regular_polygon<Rep, S>& p) noexcept
	{
		return get_exterior_angles_helper(p, std::make_index_sequence<S>{});
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr std::size_t number_sides(const regular_polygon<Rep, S>&) noexcept
	{
		return S;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr std::size_t number_sides(const polygon<Rep, S>&) noexcept
	{
		return S;
	}

	export
	template<std::size_t I, class Rep, std::size_t S>
		requires(I <= S && I != 0)
	[[nodiscard]] constexpr Rep get_side(const regular_polygon<Rep, S>& p) noexcept
	{
		return p.side();
	}

	export
	template<std::size_t I, class Rep, std::size_t S>
		requires(I <= S && I != 0)
	[[nodiscard]] constexpr Rep get_side(const polygon<Rep, S>& p) noexcept
	{
		return p.side<I>();
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr std::array<Rep, S> get_sides(const regular_polygon<Rep, S>& p) noexcept
	{
		std::array<Rep, S> results;
		std::fill(std::begin(results), std::end(results), p.side());
		return results;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto get_sides(const polygon<Rep, S>& p) noexcept
	{
		return p.sides();
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr bool similar(const regular_polygon<Rep, S>&, const regular_polygon<Rep, S>&) noexcept
	{
		return true;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr bool congruent(const regular_polygon<Rep, S>& lhs, const regular_polygon<Rep, S>& rhs) noexcept
	{
		return lhs == rhs;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto circumcircle(const regular_polygon<Rep, S>& p) noexcept
	{
		using common_type = std::common_type_t<Rep, long double>;

		const common_type numerator = p.side();
		const common_type denominator = 2.0L * std::sin(std::numbers::pi_v<long double> / S);
			
		const auto radius = numerator / denominator;

		return circle{radius};
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto apothem(const regular_polygon<Rep, S>& p) noexcept
	{
		using common_type = std::common_type_t<Rep, long double>;

		return static_cast<common_type>(p.side()) / 
				static_cast<common_type>(2.0L * std::tan(std::numbers::pi_v<long double> / S));
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto incircle(const regular_polygon<Rep, S>& p) noexcept
	{
		return circle{apothem(p)};
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto area(const regular_polygon<Rep, S>& p) noexcept
	{
		return 0.25L * S * p.side() * p.side() * cmoon::cot(std::numbers::pi_v<long double> / S);
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto perimeter(const regular_polygon<Rep, S>& p) noexcept
	{
		return p.side() * S;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto perimeter(const polygon<Rep, S>& p) noexcept
	{
		const auto& sides = p.sides();
		return std::reduce(std::begin(sides), std::end(sides));
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto semi_perimeter(const regular_polygon<Rep, S>& p) noexcept
	{
		return perimeter(p) / 2;
	}

	export
	template<class Rep, std::size_t S>
	[[nodiscard]] constexpr auto semi_perimeter(const polygon<Rep, S>& p) noexcept
	{
		return perimeter(p) / 2;
	}

	export
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

	export
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

	export
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

	export
	template<class Rep>
	using regular_triangle = regular_polygon<Rep, 3>;

	export
	template<class Rep>
	using square = regular_polygon<Rep, 4>;

	export
	template<class Rep>
	using regular_pentagon = regular_polygon<Rep, 5>;

	export
	template<class Rep>
	using regular_hexagon = regular_polygon<Rep, 6>;

	export
	template<class Rep>
	using regular_heptagon = regular_polygon<Rep, 7>;

	export
	template<class Rep>
	using regular_octagon = regular_polygon<Rep, 8>;

	export
	template<class Rep>
	using regular_nonagon = regular_polygon<Rep, 9>;

	export
	template<class Rep>
	using regular_decagon = regular_polygon<Rep, 10>;

	export
	template<class Rep>
	using regular_hendecagon = regular_polygon<Rep, 11>;

	export
	template<class Rep>
	using regular_dodecagon = regular_polygon<Rep, 12>;

	export
	template<class Rep>
	using regular_triskaidecagon = regular_polygon<Rep, 13>;

	export
	template<class Rep>
	using regular_tetradecagon = regular_polygon<Rep, 14>;

	export
	template<class Rep>
	using regular_pentadecagon = regular_polygon<Rep, 15>;

	export
	template<class Rep>
	using regular_hexadecagon = regular_polygon<Rep, 16>;

	export
	template<class Rep>
	using regular_heptadecagon = regular_polygon<Rep, 17>;

	export
	template<class Rep>
	using regular_octadecagon = regular_polygon<Rep, 18>;

	export
	template<class Rep>
	using regular_enneadecagon = regular_polygon<Rep, 19>;

	export
	template<class Rep>
	using regular_icosagon = regular_polygon<Rep, 20>;

	export
	template<class Rep>
	using regular_icosihenagon = regular_polygon<Rep, 21>;

	export
	template<class Rep>
	using regular_icosidigon = regular_polygon<Rep, 22>;

	export
	template<class Rep>
	using regular_icositrigon = regular_polygon<Rep, 23>;

	export
	template<class Rep>
	using regular_icositetragon = regular_polygon<Rep, 24>;

	export
	template<class Rep>
	using regular_icosipentagon = regular_polygon<Rep, 25>;

	export
	template<class Rep>
	using regular_icosihexagon = regular_polygon<Rep, 26>;

	export
	template<class Rep>
	using regular_icosiheptagon = regular_polygon<Rep, 27>;

	export
	template<class Rep>
	using regular_icosioctagon = regular_polygon<Rep, 28>;

	export
	template<class Rep>
	using regular_icosienneagon = regular_polygon<Rep, 29>;

	export
	template<class Rep>
	using regular_triacontagon = regular_polygon<Rep, 30>;

	export
	template<class Rep>
	using regular_tetracontagon = regular_polygon<Rep, 40>;

	export
	template<class Rep>
	using regular_hectogon = regular_polygon<Rep, 100>;

	export
	template<class Rep>
	using regular_chiliagon = regular_polygon<Rep, 1'000>;

	export
	template<class Rep>
	using regular_myriagon = regular_polygon<Rep, 10'000>;

	export
	template<class Rep>
	using regular_megagon = regular_polygon<Rep, 1'000'000>;

	export
	template<class Rep>
	[[nodiscard]] constexpr auto area(const regular_triangle<Rep>& p) noexcept
	{
		return std::numbers::sqrt3_v<long double> / 4.0L * p.side() * p.side();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr auto area(const square<Rep>& p) noexcept
	{
		return p.side() * p.side();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr auto area(const regular_hexagon<Rep>& p) noexcept
	{
		return (1.5L * std::numbers::pi_v<long double>) * p.side() * p.side();
	}

	export
	template<class Rep>
	[[nodiscard]] constexpr auto area(const regular_octagon<Rep>& p) noexcept
	{
		return 2.0L * (1.0L + std::numbers::sqrt2_v<long double>) * p.side() * p.side();
	}
}