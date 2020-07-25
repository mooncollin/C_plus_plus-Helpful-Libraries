#include <numeric>

#include "collin/linear/matrix.hpp"
#include "collin/linear/algorithms.hpp"
#include "collin/test.hpp"

class matrix_constructor_test : public collin::test::test_case
{
	public:
		matrix_constructor_test()
			: collin::test::test_case{"matrix_constructor_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat{2, 2};
			collin::test::assert_equal(mat.size(), 4);
			collin::test::assert_equal(mat.rows(), 2);
			collin::test::assert_equal(mat.cols(), 2);

			collin::test::assert_equal(mat(0, 0), 0);
			collin::test::assert_equal(mat(0, 1), 0);
			collin::test::assert_equal(mat(1, 0), 0);
			collin::test::assert_equal(mat(1, 1), 0);

			mat = {
				1, 2,
				3, 4
			};

			collin::test::assert_equal(mat(0, 0), 1);
			collin::test::assert_equal(mat(0, 1), 2);
			collin::test::assert_equal(mat(1, 0), 3);
			collin::test::assert_equal(mat(1, 1), 4);

			constexpr collin::linear::fixed_matrix<int, 2, 2> goal1 = {1, 2, 3, 4};
			constexpr collin::linear::fixed_matrix<int, 2, 2> goal2 = {5, 6, 7, 8};

			collin::linear::matrix<int> mat2{goal1};
			collin::test::assert_equal(mat2, goal1);

			mat2 = goal2;

			collin::test::assert_equal(mat2, goal2);
		}

	private:
		void static_information()
		{
			const collin::linear::matrix<int> mat{2, 2};
			constexpr auto mat_size = sizeof(mat);
		}
};

class matrix_add_test : public collin::test::test_case
{
	public:
		matrix_add_test()
			: collin::test::test_case{"matrix_add_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat1{3, 2};
			collin::linear::matrix<int> mat2{3, 2};

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

			collin::linear::matrix<int> mat3{3, 2};
			mat3 = {
				1, 3,
				8, 5,
				3, 3
			};

			collin::test::assert_equal(mat1 + mat2, mat3);
		}
};

class matrix_minus_test : public collin::test::test_case
{
public:
	matrix_minus_test()
		: collin::test::test_case{"matrix_minus_test"} {}

	void operator()() override
	{
		collin::linear::matrix<int> mat1{3, 2};
		collin::linear::matrix<int> mat2{3, 2};

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

		collin::linear::matrix<int> mat3{3, 2};
		mat3 = {
			1, 3,
			-6, -5,
			-1, 1
		};

		collin::test::assert_equal(mat1 - mat2, mat3);
	}
};

class matrix_scale_test : public collin::test::test_case
{
	public:
		matrix_scale_test()
			: collin::test::test_case{"matrix_scale_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat{2, 3};

			mat = {
				1, 8, -3,
				4, -2, 5
			};

			collin::linear::matrix<int> mat2{2, 3};

			mat2 = {
				2, 16, -6,
				8, -4, 10
			};

			collin::test::assert_equal(mat * 2, mat2);
		}
};

class matrix_transpose_test : public collin::test::test_case
{
	public:
		matrix_transpose_test()
			: collin::test::test_case{"matrix_transpose_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat{2, 3};

			mat = {
				1, 2, 3,
				0, -6, 7
			};

			collin::linear::matrix<int> mat2{3, 2};

			mat2 = {
				1, 0,
				2, -6,
				3, 7
			};

			collin::test::assert_equal(collin::linear::transpose(mat), mat2);
		}
};

class fixed_matrix_constructor_test : public collin::test::test_case
{
public:
	fixed_matrix_constructor_test()
		: collin::test::test_case{"fixed_matrix_constructor_test"} {}

	void operator()() override
	{
		constexpr collin::linear::fixed_matrix<int, 2, 2> mat{1, 2, 3, 4};
		constexpr collin::linear::fixed_matrix<int, 3, 2> mat2{1, 2, 3, 4, 5, 6};
		static_assert((sizeof(int) * 2 * 2) == sizeof(mat));
		static_assert(mat.size() == 4);
		static_assert(mat.rows() == 2);
		static_assert(mat.cols() == 2);
		static_assert(mat(0, 0) == 1);
		static_assert(mat(0, 1) == 2);
		static_assert(mat(1, 0) == 3);
		static_assert(mat(1, 1) == 4);
		static_assert(mat2 != mat);

		collin::test::assert_equal(mat.size(), 4);
		collin::test::assert_equal(mat.rows(), 2);
		collin::test::assert_equal(mat.cols(), 2);

		collin::test::assert_equal(mat(0, 0), 1);
		collin::test::assert_equal(mat(0, 1), 2);
		collin::test::assert_equal(mat(1, 0), 3);
		collin::test::assert_equal(mat(1, 1), 4);
		collin::test::assert_not_equal(mat, mat2);
	}

private:
	void static_information()
	{
		constexpr collin::linear::fixed_matrix<int, 2, 2> mat;
		constexpr auto mat_size = sizeof(mat);
	}
};

class fixed_matrix_add_test : public collin::test::test_case
{
	public:
		fixed_matrix_add_test()
			: collin::test::test_case{"fixed_matrix_add_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 2> mat1 = {
				1, 3,
				1, 0,
				1, 2
			};
			constexpr collin::linear::fixed_matrix<int, 3, 2> mat2 = {
				0, 0,
				7, 5,
				2, 1
			};

			constexpr collin::linear::fixed_matrix<int, 3, 2> mat3 = {
				1, 3,
				8, 5,
				3, 3
			};

			static_assert(mat1 + mat2 == mat3);
			collin::test::assert_equal(mat1 + mat2, mat3);
		}
};

class fixed_matrix_minus_test : public collin::test::test_case
{
	public:
		fixed_matrix_minus_test()
			: collin::test::test_case{"fixed_matrix_minus_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 2> mat1 = {
				1, 3,
				1, 0,
				1, 2
			};
			constexpr collin::linear::fixed_matrix<int, 3, 2> mat2 = {
				0, 0,
				7, 5,
				2, 1
			};

			constexpr collin::linear::fixed_matrix<int, 3, 2> mat3 = {
				1, 3,
				-6, -5,
				-1, 1
			};

			static_assert(mat1 - mat2 == mat3);
			collin::test::assert_equal(mat1 - mat2, mat3);
		}
};

class fixed_matrix_scale_test : public collin::test::test_case
{
	public:
		fixed_matrix_scale_test()
			: collin::test::test_case{"fixed_matrix_scale_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 2, 3> mat = {
				1, 8, -3,
				4, -2, 5
			};

			constexpr collin::linear::fixed_matrix<int, 2, 3> mat2 = {
				2, 16, -6,
				8, -4, 10
			};

			static_assert(mat * 2 == mat2);
			collin::test::assert_equal(mat * 2, mat2);
		}
};

class fixed_matrix_transpose_test : public collin::test::test_case
{
	public:
		fixed_matrix_transpose_test()
			: collin::test::test_case{"fixed_matrix_transpose_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 2, 3> mat = {
				1, 2, 3,
				0, -6, 7
			};

			constexpr collin::linear::fixed_matrix<int, 3, 2> mat2 = {
				1, 0,
				2, -6,
				3, 7
			};

			static_assert(collin::linear::transpose(mat) == mat2);
			collin::test::assert_equal(collin::linear::transpose(mat), mat2);
		}
};

class mixed_matrix_equal_test : public collin::test::test_case
{
	public:
		mixed_matrix_equal_test()
			: collin::test::test_case{"mixed_matrix_equal_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 2, 3> mat = {
				1, 2, 3,
				0, -6, 7
			};

			collin::linear::matrix<int> mat2{2, 3};

			mat2 = {
				1, 2, 3,
				0, -6, 7
			};

			constexpr collin::linear::fixed_matrix<int, 2, 3> mat3 = {
				1, 10000, 3,
				0, -6, 7
			};

			collin::test::assert_equal(mat, mat2);
			collin::test::assert_not_equal(mat2, mat3);
		}
};

class mixed_matrix_add_test : public collin::test::test_case
{
	public:
		mixed_matrix_add_test()
			: collin::test::test_case{"mixed_matrix_add_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 2> mat1 = {
				1, 3,
				1, 0,
				1, 2
			};

			collin::linear::matrix<int> mat2 {3, 2};

			mat2 = {
				0, 0,
				7, 5,
				2, 1
			};

			constexpr collin::linear::fixed_matrix<int, 3, 2> mat3 = {
				1, 3,
				8, 5,
				3, 3
			};

			collin::test::assert_equal(mat1 + mat2, mat3);
		}
};

class mixed_matrix_minus_test : public collin::test::test_case
{
	public:
		mixed_matrix_minus_test()
			: collin::test::test_case{"mixed_matrix_minus_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 2> mat1 = {
				1, 3,
				1, 0,
				1, 2
			};

			collin::linear::matrix<int> mat2{3, 2};

			mat2 = {
				0, 0,
				7, 5,
				2, 1
			};

			constexpr collin::linear::fixed_matrix<int, 3, 2> mat3 = {
				1, 3,
				-6, -5,
				-1, 1
			};

			collin::test::assert_equal(mat1 - mat2, mat3);
		}
};

class mixed_matrix_multiply_test : public collin::test::test_case
{
	public:
		mixed_matrix_multiply_test()
			: collin::test::test_case{"mixed_matrix_multiply_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 2, 2> goal = {
				3, 2340,
				0, 1000
			};

			collin::linear::matrix<int> mat1{2, 3};
			collin::linear::matrix<int> mat2{3, 2};

			mat1 = {
				2, 3, 4,
				1, 0, 0
			};

			mat2 = {
				0, 1000,
				1, 100,
				0, 10
			};

			constexpr collin::linear::fixed_matrix<int, 2, 3> mat3 = {
				2, 3, 4,
				1, 0, 0
			};

			constexpr collin::linear::fixed_matrix<int, 3, 2> mat4 = {
				0, 1000,
				1, 100,
				0, 10
			};

			collin::test::assert_equal(mat1 * mat2, goal);
			collin::test::assert_equal(mat1 * mat4, goal);

			static_assert(mat3 * mat4 == goal);
			collin::test::assert_equal(mat3 * mat4, goal);
		}
};

class cofactor_test : public collin::test::test_case
{
	public:
		cofactor_test()
			: collin::test::test_case{"cofactor_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 3> fixed_input_data = 
			{
				3, 0, 2,
				2, 0, -2,
				0, 1, 1
			};

			const collin::linear::matrix<int> input_data{fixed_input_data};

			constexpr collin::linear::fixed_matrix<int, 2, 2> goal1 =
			{
				0, -2,
				1, 1
			};

			constexpr collin::linear::fixed_matrix<int, 2, 2> goal2 =
			{
				2, -2,
				0, 1
			};

			constexpr collin::linear::fixed_matrix<int, 2, 2> goal3 =
			{
				3, 2,
				2, -2
			};

			constexpr collin::linear::fixed_matrix<int, 2, 2> goal4 =
			{
				3, 0,
				2, 0
			};

			static_assert(collin::linear::cofactor(fixed_input_data, 0, 0) == goal1);
			static_assert(collin::linear::cofactor(fixed_input_data, 0, 1) == goal2);
			static_assert(collin::linear::cofactor(fixed_input_data, 2, 1) == goal3);
			static_assert(collin::linear::cofactor(fixed_input_data, 2, 2) == goal4);

			collin::test::assert_equal(collin::linear::cofactor(fixed_input_data, 0, 0), goal1);
			collin::test::assert_equal(collin::linear::cofactor(fixed_input_data, 0, 1), goal2);
			collin::test::assert_equal(collin::linear::cofactor(fixed_input_data, 2, 1), goal3);
			collin::test::assert_equal(collin::linear::cofactor(fixed_input_data, 2, 2), goal4);

			collin::test::assert_equal(collin::linear::cofactor(input_data, 0, 0), goal1);
			collin::test::assert_equal(collin::linear::cofactor(input_data, 0, 1), goal2);
			collin::test::assert_equal(collin::linear::cofactor(input_data, 2, 1), goal3);
			collin::test::assert_equal(collin::linear::cofactor(input_data, 2, 2), goal4);
		}
};

class determinant_test : public collin::test::test_case
{
	public:
		determinant_test()
			: collin::test::test_case{"determinant_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 1, 1> mat4 {6};
			constexpr collin::linear::fixed_matrix<int, 2, 2> mat5 {5, 10, 3, 9};
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat6 {6, 1, 1, 4, -2, 5, 2, 8, 7};

			const collin::linear::matrix<int> mat1 {mat4};
			const collin::linear::matrix<int> mat2 {mat5};
			const collin::linear::matrix<int> mat3 {mat6};

			collin::test::assert_throws(std::invalid_argument{"can only find the determinant of a square matrix"}, []() {
				collin::linear::matrix<int> wrong_mat {2, 3};
				collin::linear::determinant(wrong_mat);
			});

			collin::test::assert_equal(collin::linear::determinant(mat1), 6);
			collin::test::assert_equal(collin::linear::determinant(mat2), 15);
			collin::test::assert_equal(collin::linear::determinant(mat3), -306);

			static_assert(collin::linear::determinant(mat4) == 6);
			static_assert(collin::linear::determinant(mat5) == 15);
			static_assert(collin::linear::determinant(mat6) == -306);

			collin::test::assert_equal(collin::linear::determinant(mat4), 6);
			collin::test::assert_equal(collin::linear::determinant(mat5), 15);
			collin::test::assert_equal(collin::linear::determinant(mat6), -306);
		}
};

class row_test : public collin::test::test_case
{
	public:
		row_test()
			: collin::test::test_case{"row_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat3 {3, 3};
			mat3 = {
				6, 1, 1,
				4, -2, 5,
				2, 8, 7
			};

			auto first_row = mat3.get_row(0);
			collin::test::assert_equal(first_row[0], 6);
			collin::test::assert_equal(first_row[1], 1);
			collin::test::assert_equal(first_row[2], 1);

			first_row[0] = 10;
			first_row[1] = 15;
			first_row[2] = 20;

			collin::test::assert_equal(first_row[0], 10);
			collin::test::assert_equal(first_row[1], 15);
			collin::test::assert_equal(first_row[2], 20);

			const auto second_row = mat3.get_row(1);
			const auto third_row = mat3.get_row(2);

			collin::test::assert_equal(second_row[0], 4);
			collin::test::assert_equal(second_row[1], -2);
			collin::test::assert_equal(second_row[2], 5);

			collin::test::assert_equal(third_row[0], 2);
			collin::test::assert_equal(third_row[1], 8);
			collin::test::assert_equal(third_row[2], 7);

			collin::test::assert_equal(std::accumulate(std::begin(third_row), std::end(third_row), 0), 17);
		}

	private:
		void static_information()
		{
			collin::linear::matrix<int> mat3 {3, 3};
			const auto r = mat3.get_row(0);
			constexpr auto row_size = sizeof(r);
		}
};

class column_test : public collin::test::test_case
{
	public:
		column_test()
			: collin::test::test_case{"column_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat3 {3, 3};
			mat3 = {
				6, 1, 1,
				4, -2, 5,
				2, 8, 7
			};

			auto first_column = mat3.get_column(0);
			collin::test::assert_equal(first_column[0], 6);
			collin::test::assert_equal(first_column[1], 4);
			collin::test::assert_equal(first_column[2], 2);

			first_column[0] = 10;
			first_column[1] = 15;
			first_column[2] = 20;

			collin::test::assert_equal(first_column[0], 10);
			collin::test::assert_equal(first_column[1], 15);
			collin::test::assert_equal(first_column[2], 20);

			const auto second_column = mat3.get_column(1);
			const auto third_column = mat3.get_column(2);

			collin::test::assert_equal(second_column[0], 1);
			collin::test::assert_equal(second_column[1], -2);
			collin::test::assert_equal(second_column[2], 8);

			collin::test::assert_equal(third_column[0], 1);
			collin::test::assert_equal(third_column[1], 5);
			collin::test::assert_equal(third_column[2], 7);

			collin::test::assert_equal(std::accumulate(std::begin(third_column), std::end(third_column), 0), 13);
		}
	private:
		void static_information()
		{
			collin::linear::matrix<int> mat3 {3, 3};
			const auto c = mat3.get_column(0);
			constexpr auto column_size = sizeof(c);
		}
};

class fixed_row_test : public collin::test::test_case
{
	public:
		fixed_row_test()
			: collin::test::test_case{"fixed_row_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat3 = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			constexpr auto first_row = mat3.get_row(0);
			static_assert(first_row[0] == 6);
			static_assert(first_row[1] == 1);
			static_assert(first_row[2] == 1);
			collin::test::assert_equal(first_row[0], 6);
			collin::test::assert_equal(first_row[1], 1);
			collin::test::assert_equal(first_row[2], 1);

			constexpr auto second_row = mat3.get_row(1);
			static_assert(second_row[0] == 4);
			static_assert(second_row[1] == -2);
			static_assert(second_row[2] == 5);
			collin::test::assert_equal(second_row[0], 4);
			collin::test::assert_equal(second_row[1], -2);
			collin::test::assert_equal(second_row[2], 5);

			constexpr auto third_row = mat3.get_row(2);
			static_assert(third_row[0] == 2);
			static_assert(third_row[1] == 8);
			static_assert(third_row[2] == 7);
			collin::test::assert_equal(third_row[0], 2);
			collin::test::assert_equal(third_row[1], 8);
			collin::test::assert_equal(third_row[2], 7);

			static_assert(std::accumulate(std::begin(first_row), std::end(first_row), 0) == 8);
		}

	private:
		void static_information()
		{
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat3;
			constexpr auto r = mat3.get_row(0);
			constexpr auto row_size = sizeof(r);
		}
};

class fixed_column_test : public collin::test::test_case
{
	public:
		fixed_column_test()
			: collin::test::test_case{"fixed_column_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat3 = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			constexpr auto first_column = mat3.get_column(0);
			static_assert(first_column[0] == 6);
			static_assert(first_column[1] == 4);
			static_assert(first_column[2] == 2);
			collin::test::assert_equal(first_column[0], 6);
			collin::test::assert_equal(first_column[1], 4);
			collin::test::assert_equal(first_column[2], 2);

			constexpr auto second_column = mat3.get_column(1);
			static_assert(second_column[0] == 1);
			static_assert(second_column[1] == -2);
			static_assert(second_column[2] == 8);
			collin::test::assert_equal(second_column[0], 1);
			collin::test::assert_equal(second_column[1], -2);
			collin::test::assert_equal(second_column[2], 8);

			constexpr auto third_column = mat3.get_column(2);
			static_assert(third_column[0] == 1);
			static_assert(third_column[1] == 5);
			static_assert(third_column[2] == 7);
			collin::test::assert_equal(third_column[0], 1);
			collin::test::assert_equal(third_column[1], 6);
			collin::test::assert_equal(third_column[2], 7);

			static_assert(std::accumulate(std::begin(first_column), std::end(first_column), 0) == 12);
		}

	private:
		void static_information()
		{
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat3;
			constexpr auto c = mat3.get_column(0);
			constexpr auto column_size = sizeof(c);
		}
};

class diagonal_test : public collin::test::test_case
{
	public:
		diagonal_test()
			: collin::test::test_case{"diagonal_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat{3, 3};

			mat = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			constexpr collin::linear::fixed_matrix<int, 3, 3> mat2 = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			auto main_d = collin::linear::main_diagonal(mat);
			auto main_d_it = std::begin(main_d);

			collin::test::assert_equal(main_d[0], 6);
			collin::test::assert_equal(main_d[1], -2);
			collin::test::assert_equal(main_d[2], 7);

			collin::test::assert_equal(*main_d_it, 6);
			++main_d_it;
			collin::test::assert_equal(*main_d_it, -2);
			++main_d_it;
			collin::test::assert_equal(*main_d_it, 7);
			++main_d_it;
			collin::test::assert_equal(main_d_it, std::end(main_d));

			constexpr auto main_d2 = collin::linear::main_diagonal(mat2);

			collin::test::assert_equal(main_d2[0], 6);
			collin::test::assert_equal(main_d2[1], -2);
			collin::test::assert_equal(main_d2[2], 7);

			static_assert(main_d2[0] == 6);
			static_assert(main_d2[1] == -2);
			static_assert(main_d2[2] == 7);
		}
	private:
		void static_information()
		{
			collin::linear::matrix<int> mat {3, 3};
			constexpr collin::linear::fixed_matrix<int, 3, 3> fixed_mat;
			const auto diag1 = collin::linear::main_diagonal(mat);
			constexpr auto diag2 = collin::linear::main_diagonal(fixed_mat);

			constexpr auto diag_size = sizeof(diag1);
			constexpr auto fixed_diag_size = sizeof(diag2);
			constexpr auto diag_it = sizeof(std::begin(diag1));
			constexpr auto fixed_diag_it = sizeof(std::begin(diag2));
		}
};

class identity_matrix_test : public collin::test::test_case
{
	public:
		identity_matrix_test()
			: collin::test::test_case{"identity_matrix_test"} {}

		void operator()() override
		{
			constexpr collin::linear::square_matrix<int, 1> goal1 = {1};
			constexpr collin::linear::square_matrix<int, 2> goal2 = {
				1, 0,
				0, 1
			};
			constexpr collin::linear::square_matrix<int, 3> goal3 = {
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			};
			constexpr collin::linear::square_matrix<int, 4> goal4 = {
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			};

			collin::test::assert_equal(collin::linear::identity_matrix<int, 1>(), goal1);
			collin::test::assert_equal(collin::linear::identity_matrix<int, 2>(), goal2);
			collin::test::assert_equal(collin::linear::identity_matrix<int, 3>(), goal3);
			collin::test::assert_equal(collin::linear::identity_matrix<int, 4>(), goal4);

			static_assert(collin::linear::identity_matrix<int, 1>() == goal1);
			static_assert(collin::linear::identity_matrix<int, 2>() == goal2);
			static_assert(collin::linear::identity_matrix<int, 3>() == goal3);
			static_assert(collin::linear::identity_matrix<int, 4>() == goal4);

			collin::test::assert_equal(collin::linear::identity_matrix<int>(1), goal1);
			collin::test::assert_equal(collin::linear::identity_matrix<int>(2), goal2);
			collin::test::assert_equal(collin::linear::identity_matrix<int>(3), goal3);
			collin::test::assert_equal(collin::linear::identity_matrix<int>(4), goal4);
		}
};

class pivot_matrix_test : public collin::test::test_case
{
	public:
		pivot_matrix_test()
			: collin::test::test_case{"pivot_matrix_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat = {
				6, 1, 1, 
				4, -2, 5, 
				2, 8, 7
			};

			constexpr auto pivot = collin::linear::pivot_matrix(mat);
		}
};

class lu_decomposition_test : public collin::test::test_case
{
	public:
		lu_decomposition_test()
			: collin::test::test_case{"lu_decomposition_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<double, 4, 4> mat = {
				7, 3, -1, 2,
				3, 8, 1, -4,
				-1, 1, 4, -1,
				2, -4, -1, 6
			};

			constexpr auto decomp = collin::linear::lu_decomposition(mat);

			constexpr auto p = std::get<0>(decomp);
			constexpr auto l = std::get<1>(decomp);
			constexpr auto u = std::get<2>(decomp);
		}
};

class adjugate_test : public collin::test::test_case
{
	public:
		adjugate_test()
			: collin::test::test_case{"adjugate_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<int, 2, 2> fixed_input_data =
			{
				10, 4,
				-2, 54
			};

			constexpr collin::linear::fixed_matrix<int, 4, 4> fixed_input_data2 =
			{
				5, -2, 2, 7,
				1, 0, 0, 3,
				-3, 1, 5, 0,
				3, -1, -9, 4
			};

			const collin::linear::matrix<int> input_data{fixed_input_data};
			const collin::linear::matrix<int> input_data2{fixed_input_data2};

			constexpr collin::linear::fixed_matrix<int, 2, 2> goal = 
			{
				54, -4,
				2, 10
			};

			constexpr collin::linear::fixed_matrix<int, 4, 4> goal2 =
			{
				-12, 76, -60, -36,
				-56, 208, -82, -58,
				4, 4, -2, -10,
				4, 4, 20, 12
			};

			static_assert(collin::linear::adjugate(fixed_input_data) == goal);
			static_assert(collin::linear::adjugate(fixed_input_data2) == goal2);
			
			collin::test::assert_equal(collin::linear::adjugate(fixed_input_data), goal);
			collin::test::assert_equal(collin::linear::adjugate(input_data), goal);

			collin::test::assert_equal(collin::linear::adjugate(fixed_input_data2), goal2);
			collin::test::assert_equal(collin::linear::adjugate(input_data2), goal2);
		}
};

class inverse_test : public collin::test::test_case
{
	public:
		inverse_test()
			: collin::test::test_case{"inverse_test"} {}

		void operator()() override
		{
			constexpr collin::linear::fixed_matrix<double, 4, 4> fixed_input_data = 
			{
				5, -2, 2, 7,
				1, 0, 0, 3,
				-3, 1, 5, 0,
				3, -1, -9, 4
			};

			const collin::linear::matrix<double> input_data {fixed_input_data};

			constexpr collin::linear::fixed_matrix<double, 4, 4> goal = 
			{
				-0.136364, 0.863636, -0.681818, -0.409091, 
				-0.636364, 2.36364, -0.931818, -0.659091, 
				0.0454545, 0.0454545, -0.0227273, -0.113636, 
				0.0454545, 0.0454545, 0.227273, 0.136364
			};

			constexpr auto constexpr_test = collin::linear::inverse(fixed_input_data);
			const auto test = collin::linear::inverse(input_data);

			auto i_it = std::begin(test);
			auto fixed_i_it = std::begin(constexpr_test);
			auto goal_it = std::begin(goal);

			for(; fixed_i_it != std::end(constexpr_test); ++fixed_i_it, ++goal_it, ++i_it)
			{
				collin::test::assert_almost_equal(*i_it, *goal_it, 0.00001);
				collin::test::assert_almost_equal(*fixed_i_it, *goal_it, 0.00001);
			}
		}
};

int main()
{
	collin::test::test_suite suite;

	suite.add_test_case<matrix_constructor_test>();
	suite.add_test_case<matrix_add_test>();
	suite.add_test_case<matrix_minus_test>();
	suite.add_test_case<matrix_scale_test>();
	suite.add_test_case<matrix_transpose_test>();
	suite.add_test_case<fixed_matrix_constructor_test>();
	suite.add_test_case<fixed_matrix_add_test>();
	suite.add_test_case<fixed_matrix_minus_test>();
	suite.add_test_case<fixed_matrix_scale_test>();
	suite.add_test_case<mixed_matrix_equal_test>();
	suite.add_test_case<mixed_matrix_add_test>();
	suite.add_test_case<mixed_matrix_minus_test>();
	suite.add_test_case<mixed_matrix_multiply_test>();
	suite.add_test_case<cofactor_test>();
	suite.add_test_case<determinant_test>();
	suite.add_test_case<row_test>();
	suite.add_test_case<column_test>();
	suite.add_test_case<fixed_row_test>();
	suite.add_test_case<diagonal_test>();
	suite.add_test_case<identity_matrix_test>();
	suite.add_test_case<adjugate_test>();
	suite.add_test_case<inverse_test>();

	collin::test::text_test_runner runner{std::cout};
	return !runner.run(suite);
}