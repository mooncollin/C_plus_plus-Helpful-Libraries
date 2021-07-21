export module cmoon.tests.geometry;
export import cmoon.tests.geometry.polygon;
export import cmoon.tests.geometry.point;
export import cmoon.tests.geometry.angle;
export import cmoon.tests.geometry.triangle;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::geometry>()
	{
		cmoon::test::test_suite suite{"geometry library tests"};
		suite.add_test_case<geometry::regular_polygon_construction_test>();
		suite.add_test_case<geometry::point_normalize_test>();
		suite.add_test_case<geometry::angle_construction_test>();
		suite.add_test_case<geometry::angle_constants_test>();
		suite.add_test_case<geometry::triangle_construction_test>();
		suite.add_test_case<geometry::triangle_perimeter_test>();

		return std::move(suite);
	}
}