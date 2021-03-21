#pragma once

#include <cstddef>
#include <type_traits>

#include "cmoon/concepts.hpp"
#include "cmoon/geometry/angle.hpp"
#include "cmoon/geometry/sphere.hpp"

namespace cmoon
{
	namespace geometry
	{
		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
			requires((V - E + F) == 2)
		class regular_platonic_solid;

		template<class Rep>
		using tetrahedron = regular_platonic_solid<Rep, 4, 6, 4>;

		template<class Rep>
		using cube = regular_platonic_solid<Rep, 8, 12, 6>;

		template<class Rep>
		using octahedron = regular_platonic_solid<Rep, 6, 12, 8>;

		template<class Rep>
		using dodecahedron = regular_platonic_solid<Rep, 20, 30, 12>;

		template<class Rep>
		using icosahedron = regular_platonic_solid<Rep, 12, 30, 20>;

		template<class Rep>
		[[nodiscard]] constexpr angle dihedral_angle(const tetrahedron<Rep>&) noexcept
		{
			return angle{70.5288, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr angle dihedral_angle(const cube<Rep>&) noexcept
		{
			return angle::quarter();
		}

		template<class Rep>
		[[nodiscard]] constexpr angle dihedral_angle(const octahedron<Rep>&) noexcept
		{
			return angle{109.47, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr angle dihedral_angle(const dodecahedron<Rep>&) noexcept
		{
			return angle{116.57, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr angle dihedral_angle(const icosahedron<Rep>&) noexcept
		{
			return angle{138.19, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t coxeter_number(const tetrahedron<Rep>&) noexcept
		{
			return 4;
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t coxeter_number(const cube<Rep>&) noexcept
		{
			return 6;
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t coxeter_number(const octahedron<Rep>&) noexcept
		{
			return 6;
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t coxeter_number(const dodecahedron<Rep>&) noexcept
		{
			return 10;
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t coxeter_number(const icosahedron<Rep>&) noexcept
		{
			return 10;
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
			requires((V - E + F) == 2)
		class regular_platonic_solid
		{
			public:
				using rep = Rep;

				constexpr regular_platonic_solid() = default;
				explicit constexpr regular_platonic_solid(const rep& s) noexcept(std::is_nothrow_copy_constructible_v<Rep>)
					: edge_{s} {}

				constexpr void edge(const rep& s) noexcept(std::is_nothrow_copy_assignable_v<Rep>)
				{
					edge_ = s;
				}

				[[nodiscard]] constexpr rep edge() const noexcept
				{
					return edge_;
				}

				[[nodiscard]] static constexpr std::size_t edges() noexcept
				{
					return E;
				}

				[[nodiscard]] static constexpr std::size_t vertices() noexcept
				{
					return V;
				}

				[[nodiscard]] static constexpr std::size_t faces() noexcept
				{
					return F;
				}

				[[nodiscard]] static constexpr std::size_t edges_per_face() noexcept
				{
					return (2u * E) / F;
				}

				[[nodiscard]] static constexpr std::size_t faces_per_vertex() noexcept
				{
					return (2u * E) / V;
				}

				[[nodiscard]] long double surface_area() const noexcept
				{
					constexpr auto n = edges_per_face();
					return static_cast<long double>(faces() * n * edge() * edge()) /
						   static_cast<long double>(4 * std::tan(std::numbers::pi_v<long double> / n));
				}

				[[nodiscard]] sphere<long double> insphere() const noexcept
				{
					const auto c1 = edge() / 2.0L;
					const auto c2 = cmoon::cot(std::numbers::pi_v<long double> / edges_per_face());
					const auto c3 = std::tan(dihedral_angle(*this) / 2.0L);

					return sphere{c1 * c2 * c3};
				}

				[[nodiscard]] sphere<long double> circumsphere() const noexcept
				{
					const auto c1 = edge() / 2.0L;
					const auto c2 = std::tan(std::numbers::pi_v<long double> / faces_per_vertex());
					const auto c3 = std::tan(dihedral_angle(*this) / 2.0L);

					return sphere{c1 * c2 * c3};
				}

				[[nodiscard]] long double volume() const noexcept
				{
					(1.0L / 3.0L) * insphere().radius() * surface_area();
				}

				[[nodiscard]] friend constexpr bool operator==(const regular_platonic_solid&, const regular_platonic_solid&) noexcept = default;
				[[nodiscard]] friend constexpr bool operator!=(const regular_platonic_solid&, const regular_platonic_solid&) noexcept = default;
			private:
				rep edge_ {};
		};
	}
}