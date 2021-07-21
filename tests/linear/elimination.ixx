export module cmoon.tests.linear.elimination;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class gaussian_elimination_test : public cmoon::test::test_case
	{
		public:
			gaussian_elimination_test()
				: cmoon::test::test_case{"gaussian_elimination_test"} {}

			void operator()() override
			{
				constexpr double delta = 0.00000000000001;
				constexpr cmoon::linear::static_matrix<double, 3, 3> starting_const {2, 1, -1,
																					-3, -1, 2,
																					-2, 1, 2};

				constexpr cmoon::linear::static_matrix<double, 3, 1> b_const {8,
																			 -11,
																			 -3};

				cmoon::linear::matrix<double> starting {starting_const};
				cmoon::linear::matrix<double> b {b_const};

				cmoon::linear::static_matrix<double, 3, 3> start2 {starting_const};
				cmoon::linear::static_matrix<double, 3, 1> b2 {b_const};

				cmoon::linear::static_matrix<double, 3, 3> start3 {starting_const};
				cmoon::linear::matrix<double> b3 {b_const};

				cmoon::linear::matrix<double> start4 {starting_const};
				cmoon::linear::static_matrix<double, 3, 1> b4 {b_const};

				const auto result = cmoon::linear::gaussian_elimination(starting, b);
				const auto result2 = cmoon::linear::gaussian_elimination(start2, b2);
				const auto result3 = cmoon::linear::gaussian_elimination(start3, b3);
				const auto result4 = cmoon::linear::gaussian_elimination(start4, b4);

				cmoon::test::assert_almost_equal(result[0], 2, delta);
				cmoon::test::assert_almost_equal(result[1], 3, delta);
				cmoon::test::assert_almost_equal(result[2], -1, delta);

				cmoon::test::assert_equal(result, result2);
				cmoon::test::assert_equal(result2, result3);
				cmoon::test::assert_equal(result3, result4);
			}
	};
}