export module cmoon.tests.linear.transpose;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class matrix_transpose_test : public cmoon::test::test_case
	{
		public:
			matrix_transpose_test()
				: cmoon::test::test_case{"matrix_transpose_test"} {}

			void operator()() override
			{
				cmoon::linear::matrix<int> mat{2, 3};

				mat = {
					1, 2, 3,
					0, -6, 7
				};

				cmoon::linear::matrix<int> mat2{3, 2};

				mat2 = {
					1, 0,
					2, -6,
					3, 7
				};

				cmoon::test::assert_equal(cmoon::linear::transpose(mat), mat2);
				cmoon::test::assert_equal(cmoon::linear::transpose(mat2), mat);
			}
	}

	export
	class static_matrix_transpose_test : public cmoon::test::test_case
	{
		public:
			static_matrix_transpose_test()
				: cmoon::test::test_case{"static_matrix_transpose_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 2, 3> mat = {
					1, 2, 3,
					0, -6, 7
				};

				constexpr cmoon::linear::static_matrix<int, 3, 2> mat2 = {
					1, 0,
					2, -6,
					3, 7
				};

				const cmoon::linear::static_matrix<int, 2, 3> mat3{mat};
				const cmoon::linear::static_matrix<int, 3, 2> mat4{mat2};

				static_assert(cmoon::linear::transpose(mat) == mat2);
				cmoon::test::assert_equal(cmoon::linear::transpose(mat), mat2);

				static_assert(cmoon::linear::transpose(mat2) == mat);
				cmoon::test::assert_equal(cmoon::linear::transpose(mat2), mat);

				cmoon::test::assert_equal(cmoon::linear::transpose(mat3), mat4);

				cmoon::test::assert_equal(cmoon::linear::transpose(mat4), mat3);
			}
	};
}