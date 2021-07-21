export module cmoon.tests.linear.inverse;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class inverse_test : public cmoon::test::test_case
	{
		public:
			inverse_test()
				: cmoon::test::test_case{"inverse_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<double, 4, 4> fixed_input_data = 
				{
					5, -2, 2, 7,
					1, 0, 0, 3,
					-3, 1, 5, 0,
					3, -1, -9, 4
				};

				const cmoon::linear::matrix<double> input_data {fixed_input_data};

				constexpr cmoon::linear::static_matrix<double, 4, 4> goal = 
				{
					-0.136364, 0.863636, -0.681818, -0.409091, 
					-0.636364, 2.36364, -0.931818, -0.659091, 
					0.0454545, 0.0454545, -0.0227273, -0.113636, 
					0.0454545, 0.0454545, 0.227273, 0.136364
				};

				constexpr double delta = 0.00001;

				auto constexpr_test = cmoon::linear::inverse(fixed_input_data);
				const auto const_test2 = cmoon::linear::inverse(fixed_input_data);
				const auto test = cmoon::linear::inverse(input_data);

				cmoon::test::assert_sequence_almost_equal(std::begin(test), std::end(test), std::begin(goal), delta);
				cmoon::test::assert_sequence_almost_equal(std::begin(const_test2), std::end(const_test2), std::begin(goal), delta);
				cmoon::test::assert_sequence_almost_equal(std::begin(constexpr_test), std::end(constexpr_test), std::begin(goal), delta);
			}
	};
}