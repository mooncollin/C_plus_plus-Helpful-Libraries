#include <iostream>

#include "cmoon/geometry/angle.hpp"
#include "cmoon/geometry/circle.hpp"
#include "cmoon/geometry/line.hpp"
#include "cmoon/geometry/point.hpp"
#include "cmoon/geometry/polygon.hpp"
#include "cmoon/geometry/triangle.hpp"
#include "cmoon/geometry/rectangle.hpp"
#include "cmoon/geometry/platonic_solid.hpp"
#include "cmoon/math.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

class regular_polygon_construction_test : public cmoon::test::test_case
{
	public:
		regular_polygon_construction_test()
			: cmoon::test::test_case{"regular_polygon_construction_test"} {}

		void operator()() override
		{
			constexpr cmoon::geometry::regular_polygon<int, 3> tri{10};
			constexpr cmoon::geometry::regular_polygon<int, 3> tri2{20};

			static_assert(tri.side() == 10);
			cmoon::test::assert_equal(tri.side(), 10);

			static_assert(cmoon::geometry::get_angle<1>(tri) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			static_assert(cmoon::geometry::get_angle<2>(tri) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			static_assert(cmoon::geometry::get_angle<3>(tri) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});

			cmoon::test::assert_equal(cmoon::geometry::get_angle<1>(tri), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			cmoon::test::assert_equal(cmoon::geometry::get_angle<2>(tri), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			cmoon::test::assert_equal(cmoon::geometry::get_angle<3>(tri), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			
			constexpr auto angles = cmoon::geometry::get_angles(tri);

			static_assert(std::get<0>(angles) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			static_assert(std::get<1>(angles) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			static_assert(std::get<1>(angles) == cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});

			cmoon::test::assert_equal(std::get<0>(angles), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			cmoon::test::assert_equal(std::get<1>(angles), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});
			cmoon::test::assert_equal(std::get<1>(angles), cmoon::geometry::angle{60, cmoon::geometry::angle_type::degree});

			static_assert(cmoon::geometry::get_side<1>(tri) == 10);
			static_assert(cmoon::geometry::get_side<2>(tri) == 10);
			static_assert(cmoon::geometry::get_side<3>(tri) == 10);

			cmoon::test::assert_equal(cmoon::geometry::get_side<1>(tri), 10);
			cmoon::test::assert_equal(cmoon::geometry::get_side<2>(tri), 10);
			cmoon::test::assert_equal(cmoon::geometry::get_side<3>(tri), 10);

			constexpr auto sides = cmoon::geometry::get_sides(tri);

			static_assert(std::get<0>(sides) == 10);
			static_assert(std::get<1>(sides) == 10);
			static_assert(std::get<2>(sides) == 10);

			cmoon::test::assert_equal(std::get<0>(sides), 10);
			cmoon::test::assert_equal(std::get<1>(sides), 10);
			cmoon::test::assert_equal(std::get<2>(sides), 10);

			static_assert(cmoon::geometry::similar(tri, tri2));
			static_assert(!cmoon::geometry::congruent(tri, tri2));

			cmoon::test::assert_true(cmoon::geometry::similar(tri, tri2));
			cmoon::test::assert_false(cmoon::geometry::congruent(tri, tri2));

			static_assert(cmoon::geometry::perimeter(tri) == 30);
			cmoon::test::assert_equal(cmoon::geometry::perimeter(tri), 30);

			static_assert(cmoon::geometry::semi_perimeter(tri) == 15);
			cmoon::test::assert_equal(cmoon::geometry::semi_perimeter(tri), 15);

			static_assert(cmoon::geometry::regular(tri));
			cmoon::test::assert_true(cmoon::geometry::regular(tri));
		}
};

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

class angle_construction_test : public cmoon::test::test_case
{
	public:
		angle_construction_test()
			: cmoon::test::test_case {"angle_construction_test"} {}

		void operator()() override
		{
			constexpr cmoon::geometry::angle a_radian{{1, 2}, cmoon::geometry::angle_type::radian};
			constexpr cmoon::geometry::angle a_turn{{1, 4}, cmoon::geometry::angle_type::turn};
			constexpr cmoon::geometry::angle a_degree{{90, 1}, cmoon::geometry::angle_type::degree};
			constexpr cmoon::geometry::angle a_grad{{100, 1}, cmoon::geometry::angle_type::grad};
			constexpr cmoon::geometry::angle a_minute_arc{{5400, 1}, cmoon::geometry::angle_type::minute_arc};
			constexpr cmoon::geometry::angle a_second_arc{{324000, 1}, cmoon::geometry::angle_type::second_arc};

			static_assert(a_radian.degrees<int>() == 90);
			static_assert(a_turn.degrees<int>() == 90);
			static_assert(a_degree.degrees<int>() == 90);
			static_assert(a_grad.degrees<int>() == 90);
			static_assert(a_minute_arc.degrees<int>() == 90);
			static_assert(a_second_arc.degrees<int>() == 90);

			cmoon::test::assert_equal(a_radian.degrees<int>(), 90);
			cmoon::test::assert_equal(a_turn.degrees<int>(), 90);
			cmoon::test::assert_equal(a_degree.degrees<int>(), 90);
			cmoon::test::assert_equal(a_grad.degrees<int>(), 90);
			cmoon::test::assert_equal(a_minute_arc.degrees<int>(), 90);
			cmoon::test::assert_equal(a_second_arc.degrees<int>(), 90);
		}
};

class angle_constants_test : public cmoon::test::test_case
{
	public:
		angle_constants_test()
			: cmoon::test::test_case{"angle_constants_test"} {}

		void operator()() override
		{
			static_assert(cmoon::geometry::angle::zero().degrees<int>() == 0);
			static_assert(cmoon::geometry::angle::quarter().degrees<int>() == 90);
			static_assert(cmoon::geometry::angle::half().degrees<int>() == 180);
			static_assert(cmoon::geometry::angle::three_quarters().degrees<int>() == 270);
			static_assert(cmoon::geometry::angle::full().degrees<int>() == 360);

			cmoon::test::assert_equal(cmoon::geometry::angle::zero().degrees<int>(), 0);
			cmoon::test::assert_equal(cmoon::geometry::angle::quarter().degrees<int>(), 90);
			cmoon::test::assert_equal(cmoon::geometry::angle::half().degrees<int>(), 180);
			cmoon::test::assert_equal(cmoon::geometry::angle::three_quarters().degrees<int>(), 270);
			cmoon::test::assert_equal(cmoon::geometry::angle::full().degrees<int>(), 360);
		}
};

class triangle_construction_test : public cmoon::test::test_case
{
	public:
		triangle_construction_test()
			: cmoon::test::test_case{"triangle_construction_test"} {}

		void operator()() override
		{
			constexpr cmoon::geometry::triangle<int> t{1, 2, 3};
			static_assert(t.side<1>() == 1);
			static_assert(t.side<2>() == 2);
			static_assert(t.side<3>() == 3);
			static_assert(!cmoon::geometry::valid_triangle(t));

			cmoon::test::assert_equal(t.side<1>(), 1);
			cmoon::test::assert_equal(t.side<2>(), 2);
			cmoon::test::assert_equal(t.side<3>(), 3);
			cmoon::test::assert_false(cmoon::geometry::valid_triangle(t));

			constexpr cmoon::geometry::triangle<int> t2{8, 6, 7};
			static_assert(cmoon::geometry::valid_triangle(t2));
			cmoon::test::assert_true(cmoon::geometry::valid_triangle(t2));

			cmoon::test::assert_almost_equal(cmoon::geometry::get_angle<1>(t2).degrees(), 75.5224, 0.0001);
			cmoon::test::assert_almost_equal(cmoon::geometry::get_angle<2>(t2).degrees(), 46.5674, 0.0001);
			cmoon::test::assert_almost_equal(cmoon::geometry::get_angle<3>(t2).degrees(), 57.9100, 0.0001);
			cmoon::test::assert_almost_equal(cmoon::geometry::get_exterior_angle<1>(t2).degrees(), 104.4776, 0.001);
			cmoon::test::assert_almost_equal(cmoon::geometry::get_exterior_angle<2>(t2).degrees(), 133.4326, 0.001);
			cmoon::test::assert_almost_equal(cmoon::geometry::get_exterior_angle<3>(t2).degrees(), 122.0900, 0.001);

			const auto angles = cmoon::geometry::get_angles(t2);
			const auto ex_angles = cmoon::geometry::get_exterior_angles(t2);
			cmoon::test::assert_almost_equal(std::get<0>(angles).degrees(), 75.5224, 0.0001);
			cmoon::test::assert_almost_equal(std::get<1>(angles).degrees(), 46.5674, 0.0001);
			cmoon::test::assert_almost_equal(std::get<2>(angles).degrees(), 57.9100, 0.0001);
			cmoon::test::assert_almost_equal(std::get<0>(ex_angles).degrees(), 104.4776, 0.001);
			cmoon::test::assert_almost_equal(std::get<1>(ex_angles).degrees(), 133.4326, 0.001);
			cmoon::test::assert_almost_equal(std::get<2>(ex_angles).degrees(), 122.0900, 0.001);

			static_assert(cmoon::geometry::perimeter(t2) == 21);
			cmoon::test::assert_equal(cmoon::geometry::perimeter(t2), 21);

			static_assert(cmoon::geometry::semi_perimeter(t) == 3);
			cmoon::test::assert_equal(cmoon::geometry::semi_perimeter(t), 3);
		}
};

class perimeter_test : public cmoon::test::test_case
{
	public:
		perimeter_test()
			: cmoon::test::test_case{"perimeter_test"} {}

		void operator()() override
		{
			constexpr cmoon::geometry::triangle<int> t{8, 6, 7};

			static_assert(cmoon::geometry::perimeter(t) == 21);
			cmoon::test::assert_equal(cmoon::geometry::perimeter(t), 21);
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<point_normalize_test>();
	suite.add_test_case<angle_construction_test>();
	suite.add_test_case<angle_constants_test>();
	suite.add_test_case<triangle_construction_test>();
	suite.add_test_case<perimeter_test>();
	suite.add_test_case<regular_polygon_construction_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}