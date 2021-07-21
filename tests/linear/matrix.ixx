export module cmoon.tests.linear.matrix;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
	class matrix_constructor_test : public cmoon::test::test_case
	{
		public:
			matrix_constructor_test()
				: cmoon::test::test_case{"matrix_constructor_test"} {}

			void operator()() override
			{
				cmoon::linear::matrix<int> mat{2, 2};
				cmoon::test::assert_equal(mat.size(), 4);
				cmoon::test::assert_equal(mat.rows(), 2);
				cmoon::test::assert_equal(mat.cols(), 2);

				cmoon::test::assert_equal(mat(0, 0), 0);
				cmoon::test::assert_equal(mat(0, 1), 0);
				cmoon::test::assert_equal(mat(1, 0), 0);
				cmoon::test::assert_equal(mat(1, 1), 0);

				mat = {
					1, 2,
					3, 4
				};

				cmoon::test::assert_equal(mat(0, 0), 1);
				cmoon::test::assert_equal(mat(0, 1), 2);
				cmoon::test::assert_equal(mat(1, 0), 3);
				cmoon::test::assert_equal(mat(1, 1), 4);

				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal1 = {1, 2, 3, 4};
				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal2 = {5, 6, 7, 8};

				cmoon::linear::matrix<int> mat2{goal1};
				cmoon::test::assert_equal(mat2, goal1);

				mat2 = goal2;

				cmoon::test::assert_equal(mat2, goal2);
			}
	};

	export
	class matrix_add_test : public cmoon::test::test_case
	{
		public:
			matrix_add_test()
				: cmoon::test::test_case{"matrix_add_test"} {}

			void operator()() override
			{
				cmoon::linear::matrix<int> mat1{3, 2};
				cmoon::linear::matrix<int> mat2{3, 2};

				mat1 = {
					1, 3,
					1, 0,
					1, 2
				};

				mat2 = {
					0, 0,
					7, 5,
					2, 1
				};

				cmoon::linear::matrix<int> mat3{3, 2};
				mat3 = {
					1, 3,
					8, 5,
					3, 3
				};

				cmoon::test::assert_equal(mat1 + mat2, mat3);
			}
	};

	export
	class matrix_minus_test : public cmoon::test::test_case
	{
		public:	
			matrix_minus_test()
				: cmoon::test::test_case{"matrix_minus_test"} {}

			void operator()() override
			{
				cmoon::linear::matrix<int> mat1{3, 2};
				cmoon::linear::matrix<int> mat2{3, 2};

				mat1 = {
					1, 3,
					1, 0,
					1, 2
				};

				mat2 = {
					0, 0,
					7, 5,
					2, 1
				};

				cmoon::linear::matrix<int> mat3{3, 2};
				mat3 = {
					1, 3,
					-6, -5,
					-1, 1
				};

				cmoon::test::assert_equal(mat1 - mat2, mat3);
			}
	};

	export
	class matrix_scale_test : public cmoon::test::test_case
	{
		public:
			matrix_scale_test()
				: cmoon::test::test_case{"matrix_scale_test"} {}

			void operator()() override
			{
				cmoon::linear::matrix<int> mat{2, 3};

				mat = {
					1, 8, -3,
					4, -2, 5
				};

				cmoon::linear::matrix<int> mat2{2, 3};

				mat2 = {
					2, 16, -6,
					8, -4, 10
				};

				cmoon::test::assert_equal(mat * 2, mat2);
			}
	};

	

	export
	class mixed_matrix_equal_test : public cmoon::test::test_case
	{
		public:
			mixed_matrix_equal_test()
				: cmoon::test::test_case{"mixed_matrix_equal_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::fixed_matrix<int, 2, 3> mat = {
					1, 2, 3,
					0, -6, 7
				};

				cmoon::linear::matrix<int> mat2{2, 3};

				mat2 = {
					1, 2, 3,
					0, -6, 7
				};

				constexpr cmoon::linear::fixed_matrix<int, 2, 3> mat3 = {
					1, 10000, 3,
					0, -6, 7
				};

				cmoon::test::assert_equal(mat, mat2);
				cmoon::test::assert_not_equal(mat2, mat3);
			}
	};

	export
	class mixed_matrix_add_test : public cmoon::test::test_case
	{
		public:
			mixed_matrix_add_test()
				: cmoon::test::test_case{"mixed_matrix_add_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat1 = {
					1, 3,
					1, 0,
					1, 2
				};

				cmoon::linear::matrix<int> mat2 {3, 2};

				mat2 = {
					0, 0,
					7, 5,
					2, 1
				};

				constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat3 = {
					1, 3,
					8, 5,
					3, 3
				};

				cmoon::test::assert_equal(mat1 + mat2, mat3);
			}
	};

	export
	class mixed_matrix_minus_test : public cmoon::test::test_case
	{
		public:
			mixed_matrix_minus_test()
				: cmoon::test::test_case{"mixed_matrix_minus_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat1 = {
					1, 3,
					1, 0,
					1, 2
				};

				cmoon::linear::matrix<int> mat2{3, 2};

				mat2 = {
					0, 0,
					7, 5,
					2, 1
				};

				constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat3 = {
					1, 3,
					-6, -5,
					-1, 1
				};

				cmoon::test::assert_equal(mat1 - mat2, mat3);
			}
	};

	export
	class mixed_matrix_multiply_test : public cmoon::test::test_case
	{
		public:
			mixed_matrix_multiply_test()
				: cmoon::test::test_case{"mixed_matrix_multiply_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal = {
					3, 2340,
					0, 1000
				};

				cmoon::linear::matrix<int> mat1{2, 3};
				cmoon::linear::matrix<int> mat2{3, 2};

				mat1 = {
					2, 3, 4,
					1, 0, 0
				};

				mat2 = {
					0, 1000,
					1, 100,
					0, 10
				};

				constexpr cmoon::linear::fixed_matrix<int, 2, 3> mat3 = {
					2, 3, 4,
					1, 0, 0
				};

				constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat4 = {
					0, 1000,
					1, 100,
					0, 10
				};

				cmoon::test::assert_equal(mat1 * mat2, goal);
				cmoon::test::assert_equal(mat1 * mat4, goal);

				static_assert(mat3 * mat4 == goal);
				cmoon::test::assert_equal(mat3 * mat4, goal);
			}
	};
}