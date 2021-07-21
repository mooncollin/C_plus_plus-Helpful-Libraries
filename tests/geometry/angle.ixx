export module cmoon.tests.geometry.angle;

import cmoon.test;
import cmoon.geometry;

namespace cmoon::tests::geometry
{
	export
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

	export
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
}