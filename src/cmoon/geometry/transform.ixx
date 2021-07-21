export module cmoon.geometry.transform;

import cmoon.linear;

import cmoon.geometry.point;

namespace cmoon::geometry
{
	export
	template<class T>
	constexpr point2d<T> transform_point(const matrix3x2<T>& mat, const point2d<T>& p) noexcept
	{
		return {
			p.x * mat(0, 0) + p.y * mat(1, 0) + mat(2, 0),
			p.x * mat(0, 1) + p.y * mat(1, 1) + mat(2, 1)
		};
	}
}