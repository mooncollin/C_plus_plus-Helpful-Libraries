export module cmoon.geometry.point;

import <cmath>;
import <numeric>;

import cmoon.linear;

namespace cmoon::geometry
{
	export
	template<class Rep, std::size_t D>
	using point = cmoon::linear::fixed_matrix<Rep, 1, D>;

	export
	template<class Rep>
	using point1d = point<Rep, 1>;

	export
	template<class Rep>
	using point2d = point<Rep, 2>;

	export
	template<class Rep>
	using point3d = point<Rep, 3>;

	export
	template<class Rep, std::size_t D>
	[[nodiscard]] point<Rep, D> normalize(const point<Rep, D>& p) noexcept
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
			const auto sum_of_squares = std::reduce(std::begin(p), std::end(p), Rep{}, [](const auto& sum, const auto& next) { return sum + next * next; });
			const auto division = std::sqrt(sum_of_squares);

			return p / division;
		}
	}
}