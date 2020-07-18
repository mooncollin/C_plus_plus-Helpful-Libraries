#include <iostream>

#include "collin/multidimensional.hpp"
#include "collin/test.hpp"

class multidimensional_constructor_test : public collin::test::test_case
{
	public:
		multidimensional_constructor_test()
			: collin::test::test_case{"multidimensional_constructor_test"} {}

		void operator()() override
		{
			const collin::multidimensional::multidimensional_array<int> m_arr{2, 3};
			collin::test::assert_equal(m_arr.dimensions(), std::vector<std::size_t>{2, 3});
			collin::test::assert_equal(m_arr.size(), 6);

			collin::test::assert_equal(m_arr.get(0, 0), 0);
			collin::test::assert_equal(m_arr.get(0, 1), 0);
			collin::test::assert_equal(m_arr.get(0, 2), 0);

			collin::test::assert_equal(m_arr.get(1, 0), 0);
			collin::test::assert_equal(m_arr.get(1, 1), 0);
			collin::test::assert_equal(m_arr.get(1, 2), 0);

			const collin::multidimensional::multidimensional_array<int> m_arr2{2, 3, 2};
			collin::test::assert_equal(m_arr2.dimensions(), std::vector<std::size_t>{2, 3, 2});
			collin::test::assert_equal(m_arr2.size(), 12);

			collin::test::assert_equal(m_arr2.get(0, 0, 0), 0);
			collin::test::assert_equal(m_arr2.get(0, 0, 1), 0);
			collin::test::assert_equal(m_arr2.get(0, 1, 0), 0);

			collin::test::assert_equal(m_arr2.get(0, 1, 1), 0);
			collin::test::assert_equal(m_arr2.get(0, 2, 0), 0);
			collin::test::assert_equal(m_arr2.get(0, 2, 1), 0);

			collin::test::assert_equal(m_arr2.get(1, 0, 0), 0);
			collin::test::assert_equal(m_arr2.get(1, 0, 1), 0);
			collin::test::assert_equal(m_arr2.get(1, 1, 0), 0);

			collin::test::assert_equal(m_arr2.get(1, 1, 1), 0);
			collin::test::assert_equal(m_arr2.get(1, 2, 0), 0);
			collin::test::assert_equal(m_arr2.get(1, 2, 1), 0);

			collin::multidimensional::fixed_multidimensional_array<int, 2> fixed {2, 2};
			fixed = {
				1, 2,
				3, 4
			};

			constexpr collin::multidimensional::constant_multidimensional_array<int, 2, 2> constant = {1, 2, 3, 4};

			collin::multidimensional::multidimensional_array<int> goal{2, 2};
			goal = {
				1, 2,
				3, 4
			};

			collin::multidimensional::multidimensional_array<int> m_arr3{fixed};
			collin::multidimensional::multidimensional_array<int> m_arr4{constant};

			collin::test::assert_equal(m_arr3.get(0, 0), goal.get(0, 0));
			collin::test::assert_equal(m_arr3.get(0, 1), goal.get(0, 1));
			collin::test::assert_equal(m_arr3.get(1, 0), goal.get(1, 0));
			collin::test::assert_equal(m_arr3.get(1, 1), goal.get(1, 1));

			collin::test::assert_equal(m_arr4.get(0, 0), goal.get(0, 0));
			collin::test::assert_equal(m_arr4.get(0, 1), goal.get(0, 1));
			collin::test::assert_equal(m_arr4.get(1, 0), goal.get(1, 0));
			collin::test::assert_equal(m_arr4.get(1, 1), goal.get(1, 1));

			m_arr3 = constant;
			m_arr4 = fixed;

			collin::test::assert_equal(m_arr3.get(0, 0), goal.get(0, 0));
			collin::test::assert_equal(m_arr3.get(0, 1), goal.get(0, 1));
			collin::test::assert_equal(m_arr3.get(1, 0), goal.get(1, 0));
			collin::test::assert_equal(m_arr3.get(1, 1), goal.get(1, 1));

			collin::test::assert_equal(m_arr4.get(0, 0), goal.get(0, 0));
			collin::test::assert_equal(m_arr4.get(0, 1), goal.get(0, 1));
			collin::test::assert_equal(m_arr4.get(1, 0), goal.get(1, 0));
			collin::test::assert_equal(m_arr4.get(1, 1), goal.get(1, 1));
		}

	private:
		void static_information()
		{
			const collin::multidimensional::multidimensional_array<int> m_arr{2, 3};
			const collin::multidimensional::multidimensional_array<int> m_arr2{2, 3, 2};

			constexpr auto m_arr_size = sizeof(m_arr);
			constexpr auto m_arr2_size = sizeof(m_arr2);
		}
};

class multidimensional_assignment_test : public collin::test::test_case
{
	public:
		multidimensional_assignment_test()
			: collin::test::test_case{"multidimensional_assignment_test"} {}

		void operator()() override
		{
			collin::multidimensional::multidimensional_array<int> m_arr{3, 2};

			m_arr = {1, 2, 3, 4, 5, 6};

			collin::test::assert_equal(m_arr.get(0, 0), 1);
			collin::test::assert_equal(m_arr.get(0, 1), 2);
			collin::test::assert_equal(m_arr.get(1, 0), 3);

			collin::test::assert_equal(m_arr.get(1, 1), 4);
			collin::test::assert_equal(m_arr.get(2, 0), 5);
			collin::test::assert_equal(m_arr.get(2, 1), 6);

			collin::multidimensional::multidimensional_array<int> m_arr2{2, 3, 2};

			m_arr2 = {
				1, 2, 3,
				4, 5, 6,
					7, 8, 9,
					10, 11, 12
			};

			collin::test::assert_equal(m_arr2.get(0, 0, 0), 1);
			collin::test::assert_equal(m_arr2.get(0, 0, 1), 2);
			collin::test::assert_equal(m_arr2.get(0, 1, 0), 3);

			collin::test::assert_equal(m_arr2.get(0, 1, 1), 4);
			collin::test::assert_equal(m_arr2.get(0, 2, 0), 5);
			collin::test::assert_equal(m_arr2.get(0, 2, 1), 6);

			collin::test::assert_equal(m_arr2.get(1, 0, 0), 7);
			collin::test::assert_equal(m_arr2.get(1, 0, 1), 8);
			collin::test::assert_equal(m_arr2.get(1, 1, 0), 9);

			collin::test::assert_equal(m_arr2.get(1, 1, 1), 10);
			collin::test::assert_equal(m_arr2.get(1, 2, 0), 11);
			collin::test::assert_equal(m_arr2.get(1, 2, 1), 12);
		}
};

class multidimensional_value_test : public collin::test::test_case
{
	public:
		multidimensional_value_test()
			: collin::test::test_case{"multidimensional_value_test"} {}

		void operator()() override
		{
			collin::multidimensional::multidimensional_array<int> m_arr{3, 2};

			m_arr.get(0, 0) = 1;
			m_arr.get(0, 1) = 2;
			m_arr.get(1, 0) = 3;

			m_arr.get(1, 1) = 4;
			m_arr.get(2, 0) = 5;
			m_arr.get(2, 1) = 6;

			collin::test::assert_equal(m_arr.get(0, 0), 1);
			collin::test::assert_equal(m_arr.get(0, 1), 2);
			collin::test::assert_equal(m_arr.get(1, 0), 3);

			collin::test::assert_equal(m_arr.get(1, 1), 4);
			collin::test::assert_equal(m_arr.get(2, 0), 5);
			collin::test::assert_equal(m_arr.get(2, 1), 6);

			collin::multidimensional::multidimensional_array<int> m_arr2{2, 3, 2};

			m_arr2.get(0, 0, 0) = 1;
			m_arr2.get(0, 0, 1) = 2;
			m_arr2.get(0, 1, 0) = 3;

			m_arr2.get(0, 1, 1) = 4;
			m_arr2.get(0, 2, 0) = 5;
			m_arr2.get(0, 2, 1) = 6;

			m_arr2.get(1, 0, 0) = 7;
			m_arr2.get(1, 0, 1) = 8;
			m_arr2.get(1, 1, 0) = 9;

			m_arr2.get(1, 1, 1) = 10;
			m_arr2.get(1, 2, 0) = 11;
			m_arr2.get(1, 2, 1) = 12;

			collin::test::assert_equal(m_arr2.get(0, 0, 0), 1);
			collin::test::assert_equal(m_arr2.get(0, 0, 1), 2);
			collin::test::assert_equal(m_arr2.get(0, 1, 0), 3);

			collin::test::assert_equal(m_arr2.get(0, 1, 1), 4);
			collin::test::assert_equal(m_arr2.get(0, 2, 0), 5);
			collin::test::assert_equal(m_arr2.get(0, 2, 1), 6);

			collin::test::assert_equal(m_arr2.get(1, 0, 0), 7);
			collin::test::assert_equal(m_arr2.get(1, 0, 1), 8);
			collin::test::assert_equal(m_arr2.get(1, 1, 0), 9);

			collin::test::assert_equal(m_arr2.get(1, 1, 1), 10);
			collin::test::assert_equal(m_arr2.get(1, 2, 0), 11);
			collin::test::assert_equal(m_arr2.get(1, 2, 1), 12);
		}
};

class fixed_multidimensional_constructor_test : public collin::test::test_case
{
	public:
		fixed_multidimensional_constructor_test()
			: collin::test::test_case{"fixed_multidimensional_constructor_test"} {}

		void operator()() override
		{
			const collin::multidimensional::fixed_multidimensional_array<int, 2> m_arr{2, 3};
			collin::test::assert_equal(m_arr.dimensions(), std::array<std::size_t, 2>{2, 3});
			collin::test::assert_equal(m_arr.size(), 6);

			collin::test::assert_equal(m_arr.get(0, 0), 0);
			collin::test::assert_equal(m_arr.get(0, 1), 0);
			collin::test::assert_equal(m_arr.get(0, 2), 0);

			collin::test::assert_equal(m_arr.get(1, 0), 0);
			collin::test::assert_equal(m_arr.get(1, 1), 0);
			collin::test::assert_equal(m_arr.get(1, 2), 0);

			constexpr collin::multidimensional::constant_multidimensional_array<int, 2, 2> constant = {1, 2, 3, 4};

			collin::multidimensional::fixed_multidimensional_array<int, 2> goal{2, 2};
			goal = {
				1, 2,
				3, 4
			};

			collin::multidimensional::fixed_multidimensional_array<int, 2> m_arr2{constant};

			collin::test::assert_equal(m_arr2.get(0, 0), goal.get(0, 0));
			collin::test::assert_equal(m_arr2.get(0, 1), goal.get(0, 1));
			collin::test::assert_equal(m_arr2.get(1, 0), goal.get(1, 0));
			collin::test::assert_equal(m_arr2.get(1, 1), goal.get(1, 1));

			m_arr2 = constant;

			collin::test::assert_equal(m_arr2.get(0, 0), goal.get(0, 0));
			collin::test::assert_equal(m_arr2.get(0, 1), goal.get(0, 1));
			collin::test::assert_equal(m_arr2.get(1, 0), goal.get(1, 0));
			collin::test::assert_equal(m_arr2.get(1, 1), goal.get(1, 1));
		}

	private:
		void static_information()
		{
			const collin::multidimensional::fixed_multidimensional_array<int, 2> m_arr{2, 3};
			const collin::multidimensional::fixed_multidimensional_array<int, 3> m_arr2{2, 3, 2};

			constexpr auto m_arr_size = sizeof(m_arr);
			constexpr auto m_arr2_size = sizeof(m_arr2);
		}
};

class fixed_multidimensional_value_test : public collin::test::test_case
{
	public:
		fixed_multidimensional_value_test()
			: collin::test::test_case{"fixed_multidimensional_value_test"} {}

		void operator()() override
		{
			collin::multidimensional::fixed_multidimensional_array<int, 2> m_arr{3, 2};

			m_arr.get(0, 0) = 1;
			m_arr.get(0, 1) = 2;
			m_arr.get(1, 0) = 3;

			m_arr.get(1, 1) = 4;
			m_arr.get(2, 0) = 5;
			m_arr.get(2, 1) = 6;

			collin::test::assert_equal(m_arr.get(0, 0), 1);
			collin::test::assert_equal(m_arr.get(0, 1), 2);
			collin::test::assert_equal(m_arr.get(1, 0), 3);

			collin::test::assert_equal(m_arr.get(1, 1), 4);
			collin::test::assert_equal(m_arr.get(2, 0), 5);
			collin::test::assert_equal(m_arr.get(2, 1), 6);

			collin::multidimensional::fixed_multidimensional_array<int, 3> m_arr2{2, 3, 2};

			m_arr2.get(0, 0, 0) = 1;
			m_arr2.get(0, 0, 1) = 2;
			m_arr2.get(0, 1, 0) = 3;

			m_arr2.get(0, 1, 1) = 4;
			m_arr2.get(0, 2, 0) = 5;
			m_arr2.get(0, 2, 1) = 6;

			m_arr2.get(1, 0, 0) = 7;
			m_arr2.get(1, 0, 1) = 8;
			m_arr2.get(1, 1, 0) = 9;

			m_arr2.get(1, 1, 1) = 10;
			m_arr2.get(1, 2, 0) = 11;
			m_arr2.get(1, 2, 1) = 12;

			collin::test::assert_equal(m_arr2.get(0, 0, 0), 1);
			collin::test::assert_equal(m_arr2.get(0, 0, 1), 2);
			collin::test::assert_equal(m_arr2.get(0, 1, 0), 3);

			collin::test::assert_equal(m_arr2.get(0, 1, 1), 4);
			collin::test::assert_equal(m_arr2.get(0, 2, 0), 5);
			collin::test::assert_equal(m_arr2.get(0, 2, 1), 6);

			collin::test::assert_equal(m_arr2.get(1, 0, 0), 7);
			collin::test::assert_equal(m_arr2.get(1, 0, 1), 8);
			collin::test::assert_equal(m_arr2.get(1, 1, 0), 9);

			collin::test::assert_equal(m_arr2.get(1, 1, 1), 10);
			collin::test::assert_equal(m_arr2.get(1, 2, 0), 11);
			collin::test::assert_equal(m_arr2.get(1, 2, 1), 12);
		}
};

class constant_multidimensional_constructor_test : public collin::test::test_case
{
	public:
		constant_multidimensional_constructor_test()
			: collin::test::test_case{"constant_multidimensional_constructor_test"} {}

		void operator()() override
		{
			constexpr collin::multidimensional::constant_multidimensional_array<int, 2, 3, 2> m_arr
			{
				1, 2, 3,
				4, 5, 6,
					7, 8, 9,
					10, 11, 12
			};
			static_assert((sizeof(int) * 2 * 3 * 2) == sizeof(m_arr));
			static_assert(m_arr.dimensions() == std::tuple{2, 3, 2});
			static_assert(m_arr.size() == 12);

			static_assert(m_arr.get(0, 0, 0) == 1);
			static_assert(m_arr.get(0, 0, 1) == 2);
			static_assert(m_arr.get(0, 1, 0) == 3);

			static_assert(m_arr.get(0, 1, 1) == 4);
			static_assert(m_arr.get(0, 2, 0) == 5);
			static_assert(m_arr.get(0, 2, 1) == 6);

			static_assert(m_arr.get(1, 0, 0) == 7);
			static_assert(m_arr.get(1, 0, 1) == 8);
			static_assert(m_arr.get(1, 1, 0) == 9);

			static_assert(m_arr.get(1, 1, 1) == 10);
			static_assert(m_arr.get(1, 2, 0) == 11);
			static_assert(m_arr.get(1, 2, 1) == 12);

			constexpr collin::multidimensional::constant_multidimensional_array<int, 2, 3> m_arr2
			{
				1, 2, 3,
				4, 5, 6
			};
			static_assert((sizeof(int) * 2 * 3) == sizeof(m_arr2));
			static_assert(m_arr2.dimensions() == std::tuple{2, 3});
			static_assert(m_arr2.size() == 6);

			static_assert(m_arr2.get(0, 0) == 1);
			static_assert(m_arr2.get(0, 1) == 2);
			static_assert(m_arr2.get(0, 2) == 3);

			static_assert(m_arr2.get(1, 0) == 4);
			static_assert(m_arr2.get(1, 1) == 5);
			static_assert(m_arr2.get(1, 2) == 6);

			constexpr collin::multidimensional::constant_multidimensional_array<int, 3, 1> m_arr3
			{
				4,
				1,
				8
			};
			static_assert((sizeof(int) * 3 * 1) == sizeof(m_arr3));
			static_assert(m_arr3.dimensions() == std::tuple{3, 1});
			static_assert(m_arr3.size() == 3);

			static_assert(m_arr3.get(0, 0) == 4);
			static_assert(m_arr3.get(1, 0) == 1);
			static_assert(m_arr3.get(2, 0) == 8);
		}

	private:
		void static_information()
		{
			constexpr collin::multidimensional::constant_multidimensional_array<int, 2, 3, 2> m_arr2
			{
				1, 2, 3,
				4, 5, 6,
				7, 8, 9,
				10, 11, 12
			};

			constexpr collin::multidimensional::constant_multidimensional_array<int, 2, 3> m_arr
			{
				1, 2, 3,
				4, 5, 6
			};

			constexpr auto m_arr_size = sizeof(m_arr);
			constexpr auto m_arr2_size = sizeof(m_arr2);
		}
};

int main()
{
	collin::test::test_suite suite;

	suite.add_test_case<multidimensional_constructor_test>();
	suite.add_test_case<constant_multidimensional_constructor_test>();
	suite.add_test_case<multidimensional_value_test>();
	suite.add_test_case<multidimensional_assignment_test>();
	suite.add_test_case<fixed_multidimensional_value_test>();
	suite.add_test_case<fixed_multidimensional_constructor_test>();

	collin::test::text_test_runner runner(std::cout);

	return !runner.run(suite);
}