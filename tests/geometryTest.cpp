#include <iostream>

#include "collin/geometry/angle.hpp"
#include "collin/geometry/circle.hpp"
#include "collin/geometry/line.hpp"
#include "collin/geometry/point.hpp"
#include "collin/geometry/polygon.hpp"
#include "collin/geometry/triangle.hpp"
#include "collin/geometry/rectangle.hpp"
#include "collin/geometry/platonic_solid.hpp"
#include "collin/math.hpp"
#include "collin/test.hpp"

class regular_polygon_construction_test : public collin::test::test_case
{
	public:
		regular_polygon_construction_test()
			: collin::test::test_case{"regular_polygon_construction_test"} {}

		void operator()() override
		{
			constexpr collin::geometry::regular_polygon<int, 3> tri{10};
			constexpr collin::geometry::regular_polygon<int, 3> tri2{20};

			static_assert(tri.side() == 10);
			collin::test::assert_equal(tri.side(), 10);

			static_assert(collin::geometry::get_angle<1>(tri) == collin::geometry::angle{60, collin::geometry::angle_type::degree});
			static_assert(collin::geometry::get_angle<2>(tri) == collin::geometry::angle{60, collin::geometry::angle_type::degree});
			static_assert(collin::geometry::get_angle<3>(tri) == collin::geometry::angle{60, collin::geometry::angle_type::degree});

			collin::test::assert_equal(collin::geometry::get_angle<1>(tri), collin::geometry::angle{60, collin::geometry::angle_type::degree});
			collin::test::assert_equal(collin::geometry::get_angle<2>(tri), collin::geometry::angle{60, collin::geometry::angle_type::degree});
			collin::test::assert_equal(collin::geometry::get_angle<3>(tri), collin::geometry::angle{60, collin::geometry::angle_type::degree});
			
			constexpr auto angles = collin::geometry::get_angles(tri);

			static_assert(std::get<0>(angles) == collin::geometry::angle{60, collin::geometry::angle_type::degree});
			static_assert(std::get<1>(angles) == collin::geometry::angle{60, collin::geometry::angle_type::degree});
			static_assert(std::get<1>(angles) == collin::geometry::angle{60, collin::geometry::angle_type::degree});

			collin::test::assert_equal(std::get<0>(angles), collin::geometry::angle{60, collin::geometry::angle_type::degree});
			collin::test::assert_equal(std::get<1>(angles), collin::geometry::angle{60, collin::geometry::angle_type::degree});
			collin::test::assert_equal(std::get<1>(angles), collin::geometry::angle{60, collin::geometry::angle_type::degree});

			static_assert(collin::geometry::get_side<1>(tri) == 10);
			static_assert(collin::geometry::get_side<2>(tri) == 10);
			static_assert(collin::geometry::get_side<3>(tri) == 10);

			collin::test::assert_equal(collin::geometry::get_side<1>(tri), 10);
			collin::test::assert_equal(collin::geometry::get_side<2>(tri), 10);
			collin::test::assert_equal(collin::geometry::get_side<3>(tri), 10);

			constexpr auto sides = collin::geometry::get_sides(tri);

			static_assert(std::get<0>(sides) == 10);
			static_assert(std::get<1>(sides) == 10);
			static_assert(std::get<2>(sides) == 10);

			collin::test::assert_equal(std::get<0>(sides), 10);
			collin::test::assert_equal(std::get<1>(sides), 10);
			collin::test::assert_equal(std::get<2>(sides), 10);

			static_assert(collin::geometry::similar(tri, tri2));
			static_assert(!collin::geometry::congruent(tri, tri2));

			collin::test::assert_true(collin::geometry::similar(tri, tri2));
			collin::test::assert_false(collin::geometry::congruent(tri, tri2));

			static_assert(collin::geometry::perimeter(tri) == 30);
			collin::test::assert_equal(collin::geometry::perimeter(tri), 30);

			static_assert(collin::geometry::semi_perimeter(tri) == 15);
			collin::test::assert_equal(collin::geometry::semi_perimeter(tri), 15);

			static_assert(collin::geometry::regular(tri));
			collin::test::assert_true(collin::geometry::regular(tri));
		}
};

class point_normalize_test : public collin::test::test_case
{
	public:
		point_normalize_test()
			: collin::test::test_case {"point_normalize_test"} {}

		void operator()() override
		{
			collin::geometry::point1d<double> p1d{4};
			collin::geometry::point2d<double> p2d{4, 2};
			collin::geometry::point3d<double> p3d{4, 2, 4};
			collin::geometry::point<double, 4> p4d{4, 2, 4, 2};
			
			const auto p1d_n = collin::geometry::normalize(p1d);
			const auto p2d_n = collin::geometry::normalize(p2d);
			const auto p3d_n = collin::geometry::normalize(p3d);
			const auto p4d_n = collin::geometry::normalize(p4d);
		}
};

class angle_construction_test : public collin::test::test_case
{
	public:
		angle_construction_test()
			: collin::test::test_case {"angle_construction_test"} {}

		void operator()() override
		{
			constexpr collin::geometry::angle a_radian{{1, 2}, collin::geometry::angle_type::radian};
			constexpr collin::geometry::angle a_turn{{1, 4}, collin::geometry::angle_type::turn};
			constexpr collin::geometry::angle a_degree{{90, 1}, collin::geometry::angle_type::degree};
			constexpr collin::geometry::angle a_grad{{100, 1}, collin::geometry::angle_type::grad};
			constexpr collin::geometry::angle a_minute_arc{{5400, 1}, collin::geometry::angle_type::minute_arc};
			constexpr collin::geometry::angle a_second_arc{{324000, 1}, collin::geometry::angle_type::second_arc};

			static_assert(a_radian.degrees<int>() == 90);
			static_assert(a_turn.degrees<int>() == 90);
			static_assert(a_degree.degrees<int>() == 90);
			static_assert(a_grad.degrees<int>() == 90);
			static_assert(a_minute_arc.degrees<int>() == 90);
			static_assert(a_second_arc.degrees<int>() == 90);

			collin::test::assert_equal(a_radian.degrees<int>(), 90);
			collin::test::assert_equal(a_turn.degrees<int>(), 90);
			collin::test::assert_equal(a_degree.degrees<int>(), 90);
			collin::test::assert_equal(a_grad.degrees<int>(), 90);
			collin::test::assert_equal(a_minute_arc.degrees<int>(), 90);
			collin::test::assert_equal(a_second_arc.degrees<int>(), 90);
		}
};

class angle_constants_test : public collin::test::test_case
{
	public:
		angle_constants_test()
			: collin::test::test_case{"angle_constants_test"} {}

		void operator()() override
		{
			static_assert(collin::geometry::angle::zero().degrees<int>() == 0);
			static_assert(collin::geometry::angle::quarter().degrees<int>() == 90);
			static_assert(collin::geometry::angle::half().degrees<int>() == 180);
			static_assert(collin::geometry::angle::three_quarters().degrees<int>() == 270);
			static_assert(collin::geometry::angle::full().degrees<int>() == 360);

			collin::test::assert_equal(collin::geometry::angle::zero().degrees<int>(), 0);
			collin::test::assert_equal(collin::geometry::angle::quarter().degrees<int>(), 90);
			collin::test::assert_equal(collin::geometry::angle::half().degrees<int>(), 180);
			collin::test::assert_equal(collin::geometry::angle::three_quarters().degrees<int>(), 270);
			collin::test::assert_equal(collin::geometry::angle::full().degrees<int>(), 360);
		}
};

class triangle_construction_test : public collin::test::test_case
{
	public:
		triangle_construction_test()
			: collin::test::test_case{"triangle_construction_test"} {}

		void operator()() override
		{
			constexpr collin::geometry::triangle<int> t{1, 2, 3};
			static_assert(t.side<1>() == 1);
			static_assert(t.side<2>() == 2);
			static_assert(t.side<3>() == 3);
			static_assert(!collin::geometry::valid_triangle(t));

			collin::test::assert_equal(t.side<1>(), 1);
			collin::test::assert_equal(t.side<2>(), 2);
			collin::test::assert_equal(t.side<3>(), 3);
			collin::test::assert_false(collin::geometry::valid_triangle(t));

			constexpr collin::geometry::triangle<int> t2{8, 6, 7};
			static_assert(collin::geometry::valid_triangle(t2));
			collin::test::assert_true(collin::geometry::valid_triangle(t2));

			collin::test::assert_almost_equal(collin::geometry::get_angle<1>(t2).degrees(), 75.5224, 0.0001);
			collin::test::assert_almost_equal(collin::geometry::get_angle<2>(t2).degrees(), 46.5674, 0.0001);
			collin::test::assert_almost_equal(collin::geometry::get_angle<3>(t2).degrees(), 57.9100, 0.0001);
			collin::test::assert_almost_equal(collin::geometry::get_exterior_angle<1>(t2).degrees(), 104.4776, 0.001);
			collin::test::assert_almost_equal(collin::geometry::get_exterior_angle<2>(t2).degrees(), 133.4326, 0.001);
			collin::test::assert_almost_equal(collin::geometry::get_exterior_angle<3>(t2).degrees(), 122.0900, 0.001);

			const auto angles = collin::geometry::get_angles(t2);
			const auto ex_angles = collin::geometry::get_exterior_angles(t2);
			collin::test::assert_almost_equal(std::get<0>(angles).degrees(), 75.5224, 0.0001);
			collin::test::assert_almost_equal(std::get<1>(angles).degrees(), 46.5674, 0.0001);
			collin::test::assert_almost_equal(std::get<2>(angles).degrees(), 57.9100, 0.0001);
			collin::test::assert_almost_equal(std::get<0>(ex_angles).degrees(), 104.4776, 0.001);
			collin::test::assert_almost_equal(std::get<1>(ex_angles).degrees(), 133.4326, 0.001);
			collin::test::assert_almost_equal(std::get<2>(ex_angles).degrees(), 122.0900, 0.001);

			static_assert(collin::geometry::perimeter(t2) == 21);
			collin::test::assert_equal(collin::geometry::perimeter(t2), 21);

			static_assert(collin::geometry::semi_perimeter(t) == 3);
			collin::test::assert_equal(collin::geometry::semi_perimeter(t), 3);
		}
};

class perimeter_test : public collin::test::test_case
{
	public:
		perimeter_test()
			: collin::test::test_case{"perimeter_test"} {}

		void operator()() override
		{
			constexpr collin::geometry::triangle<int> t{8, 6, 7};

			static_assert(collin::geometry::perimeter(t) == 21);
			collin::test::assert_equal(collin::geometry::perimeter(t), 21);
		}
};

int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<point_normalize_test>();
	suite.add_test_case<angle_construction_test>();
	suite.add_test_case<angle_constants_test>();
	suite.add_test_case<triangle_construction_test>();
	suite.add_test_case<perimeter_test>();
	suite.add_test_case<regular_polygon_construction_test>();

	collin::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}