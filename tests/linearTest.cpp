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

			collin::test::assert_equal(mat.transpose(), mat2);
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

			static_assert(mat.transpose() == mat2);
			collin::test::assert_equal(mat.transpose(), mat2);
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

class determinant_test : public collin::test::test_case
{
	public:
		determinant_test()
			: collin::test::test_case{"determinant_test"} {}

		void operator()() override
		{
			collin::linear::matrix<int> mat1 {1, 1};
			collin::linear::matrix<int> mat2 {2, 2};
			collin::linear::matrix<int> mat3 {3, 3};

			constexpr collin::linear::fixed_matrix<int, 1, 1> mat4 {6};
			constexpr collin::linear::fixed_matrix<int, 2, 2> mat5 {5, 10, 3, 9};
			constexpr collin::linear::fixed_matrix<int, 3, 3> mat6 {6, 1, 1, 4, -2, 5, 2, 8, 7};

			mat1 = {
				6
			};
			
			mat2 = {
				5, 10,
				3, 9
			};

			mat3 = {
				6, 1, 1,
				4, -2, 5,
				2, 8, 7
			};

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

			static_assert(collin::linear::identity_matrix<int, 1>() == goal1);
			static_assert(collin::linear::identity_matrix<int, 2>() == goal2);
			static_assert(collin::linear::identity_matrix<int, 3>() == goal3);
			static_assert(collin::linear::identity_matrix<int, 4>() == goal4);
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
	suite.add_test_case<determinant_test>();
	suite.add_test_case<row_test>();
	suite.add_test_case<column_test>();
	suite.add_test_case<fixed_row_test>();

	collin::test::text_test_runner runner{std::cout};
	return !runner.run(suite);
}