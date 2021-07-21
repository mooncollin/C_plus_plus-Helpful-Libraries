export module cmoon.tests.linear.identity_matrix;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class identity_matrix_test : public cmoon::test::test_case
	{
		public:
			identity_matrix_test()
				: cmoon::test::test_case{"identity_matrix_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::square_matrix<int, 1> goal1 = {1};
				constexpr cmoon::linear::square_matrix<int, 2> goal2 = {
					1, 0,
					0, 1
				};
				constexpr cmoon::linear::square_matrix<int, 3> goal3 = {
					1, 0, 0,
					0, 1, 0,
					0, 0, 1
				};
				constexpr cmoon::linear::square_matrix<int, 4> goal4 = {
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
				};

				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int, 1>(), goal1);
				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int, 2>(), goal2);
				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int, 3>(), goal3);
				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int, 4>(), goal4);

				static_assert(cmoon::linear::identity_matrix<int, 1>() == goal1);
				static_assert(cmoon::linear::identity_matrix<int, 2>() == goal2);
				static_assert(cmoon::linear::identity_matrix<int, 3>() == goal3);
				static_assert(cmoon::linear::identity_matrix<int, 4>() == goal4);

				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int>(1), goal1);
				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int>(2), goal2);
				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int>(3), goal3);
				cmoon::test::assert_equal(cmoon::linear::identity_matrix<int>(4), goal4);
			}
	};
}