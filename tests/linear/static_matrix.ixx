export module cmoon.tests.linear.static_matrix;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class static_matrix_constructor_test : public cmoon::test::test_case
	{
		public:
			static_matrix_constructor_test()
				: cmoon::test::test_case{"static_matrix_constructor_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 2, 2> mat{1, 2, 3, 4};
				constexpr cmoon::linear::static_matrix<int, 3, 2> mat2{1, 2, 3, 4, 5, 6};
				static_assert((sizeof(int) * 2 * 2) == sizeof(mat));
				static_assert(mat.size() == 4);
				static_assert(mat.rows() == 2);
				static_assert(mat.cols() == 2);
				static_assert(mat(0, 0) == 1);
				static_assert(mat(0, 1) == 2);
				static_assert(mat(1, 0) == 3);
				static_assert(mat(1, 1) == 4);
				static_assert(mat2 != mat);

				cmoon::test::assert_equal(mat.size(), 4);
				cmoon::test::assert_equal(mat.rows(), 2);
				cmoon::test::assert_equal(mat.cols(), 2);

				cmoon::test::assert_equal(mat(0, 0), 1);
				cmoon::test::assert_equal(mat(0, 1), 2);
				cmoon::test::assert_equal(mat(1, 0), 3);
				cmoon::test::assert_equal(mat(1, 1), 4);
				cmoon::test::assert_not_equal(mat, mat2);
			}

		private:
			void static_information()
			{
				constexpr cmoon::linear::static_matrix<int, 2, 2> mat;
				constexpr auto mat_size = sizeof(mat);
			}
	};

	export
	class static_matrix_add_test : public cmoon::test::test_case
	{
		public:
			static_matrix_add_test()
				: cmoon::test::test_case{"static_matrix_add_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 3, 2> mat1 = {
					1, 3,
					1, 0,
					1, 2
				};
				constexpr cmoon::linear::static_matrix<int, 3, 2> mat2 = {
					0, 0,
					7, 5,
					2, 1
				};

				constexpr cmoon::linear::static_matrix<int, 3, 2> mat3 = {
					1, 3,
					8, 5,
					3, 3
				};

				static_assert(mat1 + mat2 == mat3);
				cmoon::test::assert_equal(mat1 + mat2, mat3);
			}
	};

	export
	class static_matrix_minus_test : public cmoon::test::test_case
	{
		public:
			static_matrix_minus_test()
				: cmoon::test::test_case{"static_matrix_minus_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 3, 2> mat1 = {
					1, 3,
					1, 0,
					1, 2
				};
				constexpr cmoon::linear::static_matrix<int, 3, 2> mat2 = {
					0, 0,
					7, 5,
					2, 1
				};

				constexpr cmoon::linear::static_matrix<int, 3, 2> mat3 = {
					1, 3,
					-6, -5,
					-1, 1
				};

				static_assert(mat1 - mat2 == mat3);
				cmoon::test::assert_equal(mat1 - mat2, mat3);
			}
	};

	export
	class static_matrix_scale_test : public cmoon::test::test_case
	{
		public:
			static_matrix_scale_test()
				: cmoon::test::test_case{"static_matrix_scale_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 2, 3> mat = {
					1, 8, -3,
					4, -2, 5
				};

				constexpr cmoon::linear::static_matrix<int, 2, 3> mat2 = {
					2, 16, -6,
					8, -4, 10
				};

				static_assert(mat * 2 == mat2);
				cmoon::test::assert_equal(mat * 2, mat2);
			}
	};
}