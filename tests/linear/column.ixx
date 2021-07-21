export module cmoon.tests.linear.column;

import <numeric>;

import cmoon.test;
import cmoon.linear;

namespace cmoon::tests::linear
{
	export
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
			}
	};

	export
	class static_column_test : public cmoon::test::test_case
	{
		public:
			static_column_test()
				: cmoon::test::test_case{"static_column_test"} {}

			void operator()() override
			{
				constexpr cmoon::linear::static_matrix<int, 3, 3> mat3 = {
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
	};
}