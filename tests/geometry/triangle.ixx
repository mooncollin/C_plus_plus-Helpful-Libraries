export module cmoon.tests.geometry.triangle;

import cmoon.test;
import cmoon.geometry;

namespace cmoon::test::geometry
{
	export
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

	export
	class triangle_perimeter_test : public cmoon::test::test_case
	{
		public:
			triangle_perimeter_test()
				: cmoon::test::test_case{"triangle_perimeter_test"} {}

			void operator()() override
			{
				constexpr cmoon::geometry::triangle<int> t{8, 6, 7};

				static_assert(cmoon::geometry::perimeter(t) == 21);
				cmoon::test::assert_equal(cmoon::geometry::perimeter(t), 21);
			}
	};
}