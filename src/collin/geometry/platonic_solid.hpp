#pragma once

#include <cstddef>
#include "collin/geometry/angle.hpp"
#include "collin/geometry/sphere.hpp"

namespace collin
{
	namespace geometry
	{
		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		class regular_platonic_solid
		{
			static_assert((V - E + F) == 2);

			public:
				using rep = Rep;

				constexpr regular_platonic_solid() = default;
				constexpr regular_platonic_solid(rep s)
					: edge_{s} {}

				constexpr void edge(rep s) noexcept
				{
					edge_ = s;
				}

				[[nodiscard]] constexpr rep edge() const noexcept
				{
					return edge_;
				}

				template<class Rep2>
				[[nodiscard]] friend constexpr bool operator==(const regular_platonic_solid& lhs, const regular_platonic_solid<Rep2, V, E, F>& rhs) noexcept
				{
					return lhs.edge() == rhs.edge();
				}

				template<class Rep2>
				[[nodiscard]] friend constexpr bool operator!=(const regular_platonic_solid& lhs, const regular_platonic_solid<Rep2, V, E, F>& rhs) noexcept
				{
					return !(lhs == rhs);
				}
			private:
				rep edge_{};
		};

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr std::size_t number_edges(const regular_platonic_solid<Rep, V, E, F>&) noexcept
		{
			return E;
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr std::size_t number_vertices(const regular_platonic_solid<Rep, V, E, F>&) noexcept
		{
			return V;
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr std::size_t number_faces(const regular_platonic_solid<Rep, V, E, F>&) noexcept
		{
			return F;
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr std::size_t number_of_edges_per_face(const regular_platonic_solid<Rep, V, E, F>&) noexcept
		{
			return (2 * E) / F;
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr std::size_t number_of_faces_per_vertex(const regular_platonic_solid<Rep, V, E, F>&) noexcept
		{
			return (2 * E) / V;
		}

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

		template<class Rep>
		[[nodiscard]] constexpr std::size_t dihedral_angle(const tetrahedron<Rep>&) noexcept
		{
			return angle{70.5288, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t dihedral_angle(const cube<Rep>&) noexcept
		{
			return angle::quarter();
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t dihedral_angle(const octahedron<Rep>&) noexcept
		{
			return angle{109.47, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t dihedral_angle(const dodecahedron<Rep>&) noexcept
		{
			return angle{116.57, angle_type::degree};
		}

		template<class Rep>
		[[nodiscard]] constexpr std::size_t dihedral_angle(const icosahedron<Rep>&) noexcept
		{
			return angle{138.19, angle_type::degree};
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr auto surface_area(const regular_platonic_solid<Rep, V, E, F>& s) noexcept
		{
			const auto n = number_of_edges_per_face(s);
			return static_cast<long double>(F * n * s.edge() * s.edge()) /
				   static_cast<long double>(4 * std::tan(std::numbers::pi_v<long double> / n));
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr auto insphere(const regular_platonic_solid<Rep, V, E, F>& s) noexcept
		{
			const auto c1 = s.edge() / 2.0L;
			const auto c2 = collin::math::cot(std::numbers::pi_v<long double> / number_of_edges_per_face(s));
			const auto c3 = std::tan(dihedral_angle(s) / 2.0L);

			return sphere{c1 * c2 * c3};
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr auto circumsphere(const regular_platonic_solid<Rep, V, E, F>& s) noexcept
		{
			const auto c1 = s.edge() / 2.0L;
			const auto c2 = std::tan(std::numbers::pi_v<long double> / number_of_faces_per_vertex(s));
			const auto c3 = std::tan(dihedral_angle(s) / 2.0L);

			return sphere{c1 * c2 * c3};
		}

		template<class Rep, std::size_t V, std::size_t E, std::size_t F>
		[[nodiscard]] constexpr auto volume(const regular_platonic_solid<Rep, V, E, F>& s) noexcept
		{
			return (1.0L / 3.0L) * insphere(s).radius() * surface_area(s);
		}
	}
}