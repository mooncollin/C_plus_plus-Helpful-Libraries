export module cmoon.tests.geometry.point;

import cmoon.tests;
import cmoon.geometry;

namespace cmoon::tests::geometry
{
	export
	class point_normalize_test : public cmoon::test::test_case
	{
		public:
			point_normalize_test()
				: cmoon::test::test_case {"point_normalize_test"} {}

			void operator()() override
			{
				cmoon::geometry::point1d<double> p1d{4};
				cmoon::geometry::point2d<double> p2d{4, 2};
				cmoon::geometry::point3d<double> p3d{4, 2, 4};
				cmoon::geometry::point<double, 4> p4d{4, 2, 4, 2};
			
				const auto p1d_n = cmoon::geometry::normalize(p1d);
				const auto p2d_n = cmoon::geometry::normalize(p2d);
				const auto p3d_n = cmoon::geometry::normalize(p3d);
				const auto p4d_n = cmoon::geometry::normalize(p4d);
			}
	};
}