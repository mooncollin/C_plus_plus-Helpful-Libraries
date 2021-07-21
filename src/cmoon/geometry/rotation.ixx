export module cmoon.geometry.rotation;

import <concepts>;
import <cmath>;

import cmoon.linear;

import cmoon.geometry.angle;

namespace cmoon::geometry
{
	template<std::floating_point F>
	cmoon::linear::static_matrix<F, 3, 3> rotation_matrix(const angle& theta) noexcept
	{
		const auto r = theta.radians();
		const auto c = std::cos(r);
		const auto s = std::sin(r);

		return {
			c, -s, 0,
			s, c, 0,
			0, 0, 1
		};
	}

	
}