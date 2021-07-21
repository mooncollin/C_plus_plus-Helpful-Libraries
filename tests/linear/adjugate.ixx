export module cmoon.tests.linear.adjugate;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class adjugate_test : public cmoon::test::test_case
	{
		public:
			adjugate_test()
				: cmoon::test::test_case{"adjugate_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 2, 2> fixed_input_data =
				{
					10, 4,
					-2, 54
				};

				constexpr cmoon::linear::static_matrix<int, 4, 4> fixed_input_data2 =
				{
					5, -2, 2, 7,
					1, 0, 0, 3,
					-3, 1, 5, 0,
					3, -1, -9, 4
				};

				const cmoon::linear::matrix<int> input_data{fixed_input_data};
				const cmoon::linear::matrix<int> input_data2{fixed_input_data2};

				constexpr cmoon::linear::static_matrix<int, 2, 2> goal = 
				{
					54, -4,
					2, 10
				};

				constexpr cmoon::linear::static_matrix<int, 4, 4> goal2 =
				{
					-12, 76, -60, -36,
					-56, 208, -82, -58,
					4, 4, -2, -10,
					4, 4, 20, 12
				};

				static_assert(cmoon::linear::adjugate(fixed_input_data) == goal);
				static_assert(cmoon::linear::adjugate(fixed_input_data2) == goal2);
			
				cmoon::test::assert_equal(cmoon::linear::adjugate(fixed_input_data), goal);
				cmoon::test::assert_equal(cmoon::linear::adjugate(input_data), goal);

				cmoon::test::assert_equal(cmoon::linear::adjugate(fixed_input_data2), goal2);
				cmoon::test::assert_equal(cmoon::linear::adjugate(input_data2), goal2);
			}
	};
}