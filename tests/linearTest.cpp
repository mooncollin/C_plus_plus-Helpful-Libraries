#include <numeric>

#include "cmoon/linear/matrix.hpp"
#include "cmoon/linear/algorithms.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"

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

	private:
		void static_information()
		{
			const cmoon::linear::matrix<int> mat{2, 2};
			constexpr auto mat_size = sizeof(mat);
		}
};

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
};

//class fixed_matrix_constructor_test : public cmoon::test::test_case
//{
//	public:
//		fixed_matrix_constructor_test()
//			: cmoon::test::test_case{"fixed_matrix_constructor_test"} {}
//
//		void operator()() override
//		{
//			constexpr cmoon::linear::fixed_matrix<int, 2, 2> mat{1, 2, 3, 4};
//			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat2{1, 2, 3, 4, 5, 6};
//			static_assert((sizeof(int) * 2 * 2) == sizeof(mat));
//			static_assert(mat.size() == 4);
//			static_assert(mat.rows() == 2);
//			static_assert(mat.cols() == 2);
//			static_assert(mat(0, 0) == 1);
//			static_assert(mat(0, 1) == 2);
//			static_assert(mat(1, 0) == 3);
//			static_assert(mat(1, 1) == 4);
//			static_assert(mat2 != mat);
//
//			cmoon::test::assert_equal(mat.size(), 4);
//			cmoon::test::assert_equal(mat.rows(), 2);
//			cmoon::test::assert_equal(mat.cols(), 2);
//
//			cmoon::test::assert_equal(mat(0, 0), 1);
//			cmoon::test::assert_equal(mat(0, 1), 2);
//			cmoon::test::assert_equal(mat(1, 0), 3);
//			cmoon::test::assert_equal(mat(1, 1), 4);
//			cmoon::test::assert_not_equal(mat, mat2);
//		}
//
//	private:
//		void static_information()
//		{
//			constexpr cmoon::linear::fixed_matrix<int, 2, 2> mat;
//			constexpr auto mat_size = sizeof(mat);
//		}
//};

class fixed_matrix_add_test : public cmoon::test::test_case
{
	public:
		fixed_matrix_add_test()
			: cmoon::test::test_case{"fixed_matrix_add_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat1 = {
				1, 3,
				1, 0,
				1, 2
			};
			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat2 = {
				0, 0,
				7, 5,
				2, 1
			};

			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat3 = {
				1, 3,
				8, 5,
				3, 3
			};

			static_assert(mat1 + mat2 == mat3);
			cmoon::test::assert_equal(mat1 + mat2, mat3);
		}
};

class fixed_matrix_minus_test : public cmoon::test::test_case
{
	public:
		fixed_matrix_minus_test()
			: cmoon::test::test_case{"fixed_matrix_minus_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat1 = {
				1, 3,
				1, 0,
				1, 2
			};
			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat2 = {
				0, 0,
				7, 5,
				2, 1
			};

			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat3 = {
				1, 3,
				-6, -5,
				-1, 1
			};

			static_assert(mat1 - mat2 == mat3);
			cmoon::test::assert_equal(mat1 - mat2, mat3);
		}
};

class fixed_matrix_scale_test : public cmoon::test::test_case
{
	public:
		fixed_matrix_scale_test()
			: cmoon::test::test_case{"fixed_matrix_scale_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 2, 3> mat = {
				1, 8, -3,
				4, -2, 5
			};

			constexpr cmoon::linear::fixed_matrix<int, 2, 3> mat2 = {
				2, 16, -6,
				8, -4, 10
			};

			static_assert(mat * 2 == mat2);
			cmoon::test::assert_equal(mat * 2, mat2);
		}
};

class fixed_matrix_transpose_test : public cmoon::test::test_case
{
	public:
		fixed_matrix_transpose_test()
			: cmoon::test::test_case{"fixed_matrix_transpose_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 2, 3> mat = {
				1, 2, 3,
				0, -6, 7
			};

			constexpr cmoon::linear::fixed_matrix<int, 3, 2> mat2 = {
				1, 0,
				2, -6,
				3, 7
			};

			const cmoon::linear::fixed_matrix<int, 2, 3> mat3{mat};
			const cmoon::linear::fixed_matrix<int, 3, 2> mat4{mat2};

			static_assert(cmoon::linear::transpose(mat) == mat2);
			cmoon::test::assert_equal(cmoon::linear::transpose(mat), mat2);

			static_assert(cmoon::linear::transpose(mat2) == mat);
			cmoon::test::assert_equal(cmoon::linear::transpose(mat2), mat);

			cmoon::test::assert_equal(cmoon::linear::transpose(mat3), mat4);

			cmoon::test::assert_equal(cmoon::linear::transpose(mat4), mat3);
		}
};

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

class row_test : public cmoon::test::test_case
{
	public:
		row_test()
			: cmoon::test::test_case{"row_test"} {}

		void operator()() override
		{
			cmoon::linear::matrix<int> mat3 {3, 3};
			mat3 = {
				6, 1, 1,
				4, -2, 5,
				2, 8, 7
			};

			auto mat4 = mat3;
			auto mat5 = mat3;
			auto mat6 = mat3;
			auto mat7 = mat3;

			auto first_row = mat3.get_row(0);
			cmoon::test::assert_equal(first_row[0], 6);
			cmoon::test::assert_equal(first_row[1], 1);
			cmoon::test::assert_equal(first_row[2], 1);

			first_row[0] = 10;
			first_row[1] = 15;
			first_row[2] = 20;

			cmoon::test::assert_equal(first_row[0], 10);
			cmoon::test::assert_equal(first_row[1], 15);
			cmoon::test::assert_equal(first_row[2], 20);

			const auto second_row = mat3.get_row(1);
			const auto third_row = mat3.get_row(2);

			cmoon::test::assert_equal(second_row[0], 4);
			cmoon::test::assert_equal(second_row[1], -2);
			cmoon::test::assert_equal(second_row[2], 5);

			cmoon::test::assert_equal(third_row[0], 2);
			cmoon::test::assert_equal(third_row[1], 8);
			cmoon::test::assert_equal(third_row[2], 7);

			cmoon::test::assert_equal(std::accumulate(std::begin(third_row), std::end(third_row), 0), 17);

			auto row1 = mat4.get_row(0);
			row1 += mat4.get_row(1);
			cmoon::test::assert_equal(row1[0], 10);
			cmoon::test::assert_equal(row1[1], -1);
			cmoon::test::assert_equal(row1[2], 6);

			auto row2 = mat5.get_row(0);
			row2 -= mat5.get_row(1);
			cmoon::test::assert_equal(row2[0], 2);
			cmoon::test::assert_equal(row2[1], 3);
			cmoon::test::assert_equal(row2[2], -4);

			auto row3 = mat6.get_row(0);
			row3 *= 5;
			cmoon::test::assert_equal(row3[0], 30);
			cmoon::test::assert_equal(row3[1], 5);
			cmoon::test::assert_equal(row3[2], 5);

			auto row4 = mat7.get_row(0);
			row4 /= 2;
			cmoon::test::assert_equal(row4[0], 3);
			cmoon::test::assert_equal(row4[1], 0);
			cmoon::test::assert_equal(row4[2], 0);
		}

	private:
		void static_information()
		{
			cmoon::linear::matrix<int> mat3 {3, 3};
			const auto r = mat3.get_row(0);
			constexpr auto row_size = sizeof(r);
		}
};

class column_test : public cmoon::test::test_case
{
	public:
		column_test()
			: cmoon::test::test_case{"column_test"} {}

		void operator()() override
		{
			cmoon::linear::matrix<int> mat3 {3, 3};
			mat3 = {
				6, 1, 1,
				4, -2, 5,
				2, 8, 7
			};

			auto first_column = mat3.get_column(0);
			cmoon::test::assert_equal(first_column[0], 6);
			cmoon::test::assert_equal(first_column[1], 4);
			cmoon::test::assert_equal(first_column[2], 2);

			first_column[0] = 10;
			first_column[1] = 15;
			first_column[2] = 20;

			cmoon::test::assert_equal(first_column[0], 10);
			cmoon::test::assert_equal(first_column[1], 15);
			cmoon::test::assert_equal(first_column[2], 20);

			const auto second_column = mat3.get_column(1);
			const auto third_column = mat3.get_column(2);

			cmoon::test::assert_equal(second_column[0], 1);
			cmoon::test::assert_equal(second_column[1], -2);
			cmoon::test::assert_equal(second_column[2], 8);

			cmoon::test::assert_equal(third_column[0], 1);
			cmoon::test::assert_equal(third_column[1], 5);
			cmoon::test::assert_equal(third_column[2], 7);

			cmoon::test::assert_equal(std::accumulate(std::begin(third_column), std::end(third_column), 0), 13);

			static_information();
		}
	private:
		void static_information()
		{
			cmoon::linear::matrix<int> mat3 {3, 3};
			cmoon::linear::fixed_matrix<int, 3, 3> mat4;
			cmoon::linear::fixed_matrix<int, 3, 4> mat5;
			const auto c = mat3.get_column(0);
			constexpr auto c2 = mat4.get_column(0);
			constexpr auto c3 = mat5.get_column(0);
			constexpr auto column_size = sizeof(c);
			constexpr auto column_size_constant = sizeof(c2);
			constexpr auto column_size_constant2 = sizeof(c3);
		}
};

class fixed_row_test : public cmoon::test::test_case
{
	public:
		fixed_row_test()
			: cmoon::test::test_case{"fixed_row_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 3, 3> mat3 = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			constexpr auto first_row = mat3.get_row(0);
			static_assert(first_row[0] == 6);
			static_assert(first_row[1] == 1);
			static_assert(first_row[2] == 1);
			cmoon::test::assert_equal(first_row[0], 6);
			cmoon::test::assert_equal(first_row[1], 1);
			cmoon::test::assert_equal(first_row[2], 1);

			constexpr auto second_row = mat3.get_row(1);
			static_assert(second_row[0] == 4);
			static_assert(second_row[1] == -2);
			static_assert(second_row[2] == 5);
			cmoon::test::assert_equal(second_row[0], 4);
			cmoon::test::assert_equal(second_row[1], -2);
			cmoon::test::assert_equal(second_row[2], 5);

			constexpr auto third_row = mat3.get_row(2);
			static_assert(third_row[0] == 2);
			static_assert(third_row[1] == 8);
			static_assert(third_row[2] == 7);
			cmoon::test::assert_equal(third_row[0], 2);
			cmoon::test::assert_equal(third_row[1], 8);
			cmoon::test::assert_equal(third_row[2], 7);

			static_assert(std::accumulate(std::begin(first_row), std::end(first_row), 0) == 8);
		}

	private:
		void static_information()
		{
			constexpr cmoon::linear::fixed_matrix<int, 3, 3> mat3;
			constexpr auto r = mat3.get_row(0);
			constexpr auto row_size = sizeof(r);
		}
};

class fixed_column_test : public cmoon::test::test_case
{
	public:
		fixed_column_test()
			: cmoon::test::test_case{"fixed_column_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 3, 3> mat3 = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			constexpr auto first_column = mat3.get_column(0);
			static_assert(first_column[0] == 6);
			static_assert(first_column[1] == 4);
			static_assert(first_column[2] == 2);
			cmoon::test::assert_equal(first_column[0], 6);
			cmoon::test::assert_equal(first_column[1], 4);
			cmoon::test::assert_equal(first_column[2], 2);

			constexpr auto second_column = mat3.get_column(1);
			static_assert(second_column[0] == 1);
			static_assert(second_column[1] == -2);
			static_assert(second_column[2] == 8);
			cmoon::test::assert_equal(second_column[0], 1);
			cmoon::test::assert_equal(second_column[1], -2);
			cmoon::test::assert_equal(second_column[2], 8);

			constexpr auto third_column = mat3.get_column(2);
			static_assert(third_column[0] == 1);
			static_assert(third_column[1] == 5);
			static_assert(third_column[2] == 7);
			cmoon::test::assert_equal(third_column[0], 1);
			cmoon::test::assert_equal(third_column[1], 6);
			cmoon::test::assert_equal(third_column[2], 7);

			static_assert(std::accumulate(std::begin(first_column), std::end(first_column), 0) == 12);
		}

	private:
		void static_information()
		{
			constexpr cmoon::linear::fixed_matrix<int, 3, 3> mat3;
			constexpr auto c = mat3.get_column(0);
			constexpr auto column_size = sizeof(c);
		}
};

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

			constexpr cmoon::linear::fixed_matrix<int, 3, 3> mat2 = {
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
	private:
		void static_information()
		{
			cmoon::linear::matrix<int> mat {3, 3};
			constexpr cmoon::linear::fixed_matrix<int, 3, 3> fixed_mat;
			const auto diag1 = cmoon::linear::main_diagonal(mat);
			constexpr auto diag2 = cmoon::linear::main_diagonal(fixed_mat);

			constexpr auto diag_size = sizeof(diag1);
			constexpr auto fixed_diag_size = sizeof(diag2);
			constexpr auto diag_it = sizeof(std::begin(diag1));
			constexpr auto fixed_diag_it = sizeof(std::begin(diag2));
		}
};

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

class adjugate_test : public cmoon::test::test_case
{
	public:
		adjugate_test()
			: cmoon::test::test_case{"adjugate_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<int, 2, 2> fixed_input_data =
			{
				10, 4,
				-2, 54
			};

			constexpr cmoon::linear::fixed_matrix<int, 4, 4> fixed_input_data2 =
			{
				5, -2, 2, 7,
				1, 0, 0, 3,
				-3, 1, 5, 0,
				3, -1, -9, 4
			};

			const cmoon::linear::matrix<int> input_data{fixed_input_data};
			const cmoon::linear::matrix<int> input_data2{fixed_input_data2};

			constexpr cmoon::linear::fixed_matrix<int, 2, 2> goal = 
			{
				54, -4,
				2, 10
			};

			constexpr cmoon::linear::fixed_matrix<int, 4, 4> goal2 =
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

class inverse_test : public cmoon::test::test_case
{
	public:
		inverse_test()
			: cmoon::test::test_case{"inverse_test"} {}

		void operator()() override
		{
			constexpr cmoon::linear::fixed_matrix<double, 4, 4> fixed_input_data = 
			{
				5, -2, 2, 7,
				1, 0, 0, 3,
				-3, 1, 5, 0,
				3, -1, -9, 4
			};

			const cmoon::linear::matrix<double> input_data {fixed_input_data};

			constexpr cmoon::linear::fixed_matrix<double, 4, 4> goal = 
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

class elimination_test : public cmoon::test::test_case
{
	public:
		elimination_test()
			: cmoon::test::test_case{"elimination_test"} {}

		void operator()() override
		{
			constexpr double delta = 0.00000000000001;
			constexpr cmoon::linear::fixed_matrix<double, 3, 3> starting_const {2, 1, -1,
																			    -3, -1, 2,
																			    -2, 1, 2};

			constexpr cmoon::linear::fixed_matrix<double, 3, 1> b_const {8,
																		 -11,
																		 -3};

			cmoon::linear::matrix<double> starting {starting_const};
			cmoon::linear::matrix<double> b {b_const};

			cmoon::linear::fixed_matrix<double, 3, 3> start2 {starting_const};
			cmoon::linear::fixed_matrix<double, 3, 1> b2 {b_const};

			cmoon::linear::fixed_matrix<double, 3, 3> start3 {starting_const};
			cmoon::linear::matrix<double> b3 {b_const};

			cmoon::linear::matrix<double> start4 {starting_const};
			cmoon::linear::fixed_matrix<double, 3, 1> b4 {b_const};

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

int main()
{
	cmoon::test::test_suite suite;

	suite.add_test_case<matrix_constructor_test>();
	suite.add_test_case<matrix_add_test>();
	suite.add_test_case<matrix_minus_test>();
	suite.add_test_case<matrix_scale_test>();
	suite.add_test_case<matrix_transpose_test>();
	//suite.add_test_case<fixed_matrix_constructor_test>();
	suite.add_test_case<fixed_matrix_add_test>();
	suite.add_test_case<fixed_matrix_minus_test>();
	suite.add_test_case<fixed_matrix_scale_test>();
	suite.add_test_case<mixed_matrix_equal_test>();
	suite.add_test_case<mixed_matrix_add_test>();
	suite.add_test_case<mixed_matrix_minus_test>();
	suite.add_test_case<mixed_matrix_multiply_test>();
	suite.add_test_case<sub_matrix_test>();
	suite.add_test_case<determinant_test>();
	suite.add_test_case<row_test>();
	suite.add_test_case<column_test>();
	suite.add_test_case<fixed_row_test>();
	suite.add_test_case<diagonal_test>();
	suite.add_test_case<identity_matrix_test>();
	suite.add_test_case<adjugate_test>();
	suite.add_test_case<elimination_test>();
	suite.add_test_case<inverse_test>();

	cmoon::test::text_test_runner runner{std::cout};
	return !runner.run(suite);
}