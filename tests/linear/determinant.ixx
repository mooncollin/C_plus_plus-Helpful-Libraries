export module cmoon.tests.linear.determinant;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class determinant_test : public cmoon::test::test_case
	{
		public:
			determinant_test()
				: cmoon::test::test_case{"determinant_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::fixed_matrix<int, 1, 1> mat4 {6};
				constexpr cmoon::linear::fixed_matrix<int, 2, 2> mat5 {5, 10, 3, 9};
				constexpr cmoon::linear::fixed_matrix<int, 3, 3> mat6 {6, 1, 1, 4, -2, 5, 2, 8, 7};

				const cmoon::linear::matrix<int> mat1 {mat4};
				const cmoon::linear::matrix<int> mat2 {mat5};
				const cmoon::linear::matrix<int> mat3 {mat6};

				cmoon::test::assert_throws(std::invalid_argument{"can only find the determinant of a square matrix"}, []() {
					cmoon::linear::matrix<int> wrong_mat {2, 3};
					(void) cmoon::linear::determinant(wrong_mat);
				});

				cmoon::test::assert_equal(cmoon::linear::determinant(mat1), 6);
				cmoon::test::assert_equal(cmoon::linear::determinant(mat2), 15);
				cmoon::test::assert_equal(cmoon::linear::determinant(mat3), -306);

				static_assert(cmoon::linear::determinant(mat4) == 6);
				static_assert(cmoon::linear::determinant(mat5) == 15);
				static_assert(cmoon::linear::determinant(mat6) == -306);

				cmoon::test::assert_equal(cmoon::linear::determinant(mat4), 6);
				cmoon::test::assert_equal(cmoon::linear::determinant(mat5), 15);
				cmoon::test::assert_equal(cmoon::linear::determinant(mat6), -306);
			}
	};
}