#pragma once

#include <cmath>

#include "collin/linear/matrix.hpp"

namespace collin
{
	namespace geometry
	{
		template<class Rep, std::size_t D>
		using point = collin::linear::fixed_matrix<Rep, 1, D>;

		template<class Rep>
		using point1d = point<Rep, 1>;

		template<class Rep>
		using point2d = point<Rep, 2>;

		template<class Rep>
		using point3d = point<Rep, 3>;

		namespace details
		{
			template<class Rep, std::size_t... I>
			[[nodiscard]] constexpr auto normalize_helper(const point<Rep, sizeof...(I)>& p, std::index_sequence<I...>) noexcept
			{
				return ((p[I] * p[I]) + ...);
			}
		}

		template<class Rep, std::size_t D>
		[[nodiscard]] constexpr point<Rep, D> normalize(const point<Rep, D>& p) noexcept
		{
			if constexpr (D == 1)
			{
				return p;
			}
			else if constexpr (D == 2)
			{
				return p / std::hypot(p[0], p[1]);
			}
			else if constexpr (D == 3)
			{
				return p / std::hypot(p[0], p[1], p[2]);
			}
			else
			{
				const auto sum_of_squares = details::normalize_helper(p, std::make_index_sequence<D>{});
				const auto division = std::sqrt(sum_of_squares);

				return p / division;
			}
		}
	}
}