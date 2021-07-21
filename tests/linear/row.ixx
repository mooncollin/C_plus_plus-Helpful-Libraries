export module cmoon.tests.linear.row;

import <numeric>;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
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
	};

	export
	class static_row_test : public cmoon::test::test_case
	{
		public:
			static_row_test()
				: cmoon::test::test_case{"static_row_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 3, 3> mat3 = {
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
	};
}