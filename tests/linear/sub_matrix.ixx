export module cmoon.tests.linear.sub_matrix;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class sub_matrix_test : public cmoon::test::test_case
	{
		public:
			sub_matrix_test()
				: cmoon::test::test_case{"sub_matrix_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::fixed_matrix<int, 3, 3> fixed_input_data = 
				{
					3, 0, 2,
					2, 0, -2,
					0, 1, 1
				};

				const cmoon::linear::matrix<int> input_data{fixed_input_data};

				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal1 =
				{
					0, -2,
					1, 1
				};

				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal2 =
				{
					2, -2,
					0, 1
				};

				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal3 =
				{
					3, 2,
					2, -2
				};

				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal4 =
				{
					3, 0,
					2, 0
				};

				static_assert(cmoon::linear::sub_matrix(fixed_input_data, 0, 0) == goal1);
				static_assert(cmoon::linear::sub_matrix(fixed_input_data, 0, 1) == goal2);
				static_assert(cmoon::linear::sub_matrix(fixed_input_data, 2, 1) == goal3);
				static_assert(cmoon::linear::sub_matrix(fixed_input_data, 2, 2) == goal4);

				cmoon::test::assert_equal(cmoon::linear::sub_matrix(fixed_input_data, 0, 0), goal1);
				cmoon::test::assert_equal(cmoon::linear::sub_matrix(fixed_input_data, 0, 1), goal2);
				cmoon::test::assert_equal(cmoon::linear::sub_matrix(fixed_input_data, 2, 1), goal3);
				cmoon::test::assert_equal(cmoon::linear::sub_matrix(fixed_input_data, 2, 2), goal4);

				cmoon::test::assert_equal(cmoon::linear::sub_matrix(input_data, 0, 0), goal1);
				cmoon::test::assert_equal(cmoon::linear::sub_matrix(input_data, 0, 1), goal2);
				cmoon::test::assert_equal(cmoon::linear::sub_matrix(input_data, 2, 1), goal3);
				cmoon::test::assert_equal(cmoon::linear::sub_matrix(input_data, 2, 2), goal4);
			}
	};
}