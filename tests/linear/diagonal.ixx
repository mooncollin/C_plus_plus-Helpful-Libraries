export module cmoon.tests.linear.diagonal;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class diagonal_test : public cmoon::test::test_case
	{
		public:
			diagonal_test()
				: cmoon::test::test_case{"diagonal_test"} {}

			void operator()() override
			{
				cmoon::linear::matrix<int> mat{3, 3};

				mat = {
					6, 1, 1, 
					4, -2, 5, 
					2, 8, 7
				};

				constexpr cmoon::linear::static_matrix<int, 3, 3> mat2 = {
					6, 1, 1, 
					4, -2, 5, 
					2, 8, 7
				};

				auto main_d = cmoon::linear::main_diagonal(mat);
				auto main_d_it = std::begin(main_d);
				auto main_d_copy = main_d; // Making sure this works

				cmoon::test::assert_equal(main_d[0], 6);
				cmoon::test::assert_equal(main_d[1], -2);
				cmoon::test::assert_equal(main_d[2], 7);

				cmoon::test::assert_equal(*main_d_it, 6);
				++main_d_it;
				cmoon::test::assert_equal(*main_d_it, -2);
				++main_d_it;
				cmoon::test::assert_equal(*main_d_it, 7);
				++main_d_it;
				cmoon::test::assert_equal(main_d_it, std::end(main_d));

				constexpr auto main_d2 = cmoon::linear::main_diagonal(mat2);

				cmoon::test::assert_equal(main_d2[0], 6);
				cmoon::test::assert_equal(main_d2[1], -2);
				cmoon::test::assert_equal(main_d2[2], 7);

				static_assert(main_d2[0] == 6);
				static_assert(main_d2[1] == -2);
				static_assert(main_d2[2] == 7);
			}
	};
}