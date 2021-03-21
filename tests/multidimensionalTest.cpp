#include <iostream>

#include "cmoon/multidimensional.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

class multidimensional_constructor_test : public cmoon::test::test_case
{
	public:
		multidimensional_constructor_test()
			: cmoon::test::test_case{"multidimensional_constructor_test"} {}

		void operator()() override
		{
			const cmoon::multidimensional_array<int> m_arr{2, 3};
			cmoon::test::assert_equal(m_arr.dimensions(), std::vector<std::size_t>{2, 3});
			cmoon::test::assert_equal(m_arr.size(), 6);

			cmoon::test::assert_equal(m_arr.get(0, 0), 0);
			cmoon::test::assert_equal(m_arr.get(0, 1), 0);
			cmoon::test::assert_equal(m_arr.get(0, 2), 0);

			cmoon::test::assert_equal(m_arr.get(1, 0), 0);
			cmoon::test::assert_equal(m_arr.get(1, 1), 0);
			cmoon::test::assert_equal(m_arr.get(1, 2), 0);

			const cmoon::multidimensional_array<int> m_arr2{2, 3, 2};
			cmoon::test::assert_equal(m_arr2.dimensions(), std::vector<std::size_t>{2, 3, 2});
			cmoon::test::assert_equal(m_arr2.size(), 12);

			cmoon::test::assert_equal(m_arr2.get(0, 0, 0), 0);
			cmoon::test::assert_equal(m_arr2.get(0, 0, 1), 0);
			cmoon::test::assert_equal(m_arr2.get(0, 1, 0), 0);

			cmoon::test::assert_equal(m_arr2.get(0, 1, 1), 0);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 0), 0);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 1), 0);

			cmoon::test::assert_equal(m_arr2.get(1, 0, 0), 0);
			cmoon::test::assert_equal(m_arr2.get(1, 0, 1), 0);
			cmoon::test::assert_equal(m_arr2.get(1, 1, 0), 0);

			cmoon::test::assert_equal(m_arr2.get(1, 1, 1), 0);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 0), 0);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 1), 0);

			cmoon::fixed_multidimensional_array<int, 2> fixed {2, 2};
			fixed = {
				1, 2,
				3, 4
			};

			constexpr cmoon::constant_multidimensional_array<int, 2, 2> constant = {1, 2, 3, 4};

			cmoon::multidimensional_array<int> goal{2, 2};
			goal = {
				1, 2,
				3, 4
			};

			cmoon::multidimensional_array<int> m_arr3{fixed};
			cmoon::multidimensional_array<int> m_arr4{constant};

			cmoon::test::assert_equal(m_arr3.get(0, 0), goal.get(0, 0));
			cmoon::test::assert_equal(m_arr3.get(0, 1), goal.get(0, 1));
			cmoon::test::assert_equal(m_arr3.get(1, 0), goal.get(1, 0));
			cmoon::test::assert_equal(m_arr3.get(1, 1), goal.get(1, 1));

			cmoon::test::assert_equal(m_arr4.get(0, 0), goal.get(0, 0));
			cmoon::test::assert_equal(m_arr4.get(0, 1), goal.get(0, 1));
			cmoon::test::assert_equal(m_arr4.get(1, 0), goal.get(1, 0));
			cmoon::test::assert_equal(m_arr4.get(1, 1), goal.get(1, 1));

			m_arr3 = constant;
			m_arr4 = fixed;

			cmoon::test::assert_equal(m_arr3.get(0, 0), goal.get(0, 0));
			cmoon::test::assert_equal(m_arr3.get(0, 1), goal.get(0, 1));
			cmoon::test::assert_equal(m_arr3.get(1, 0), goal.get(1, 0));
			cmoon::test::assert_equal(m_arr3.get(1, 1), goal.get(1, 1));

			cmoon::test::assert_equal(m_arr4.get(0, 0), goal.get(0, 0));
			cmoon::test::assert_equal(m_arr4.get(0, 1), goal.get(0, 1));
			cmoon::test::assert_equal(m_arr4.get(1, 0), goal.get(1, 0));
			cmoon::test::assert_equal(m_arr4.get(1, 1), goal.get(1, 1));
		}

	private:
		void static_information()
		{
			const cmoon::multidimensional_array<int> m_arr{2, 3};
			const cmoon::multidimensional_array<int> m_arr2{2, 3, 2};

			constexpr auto m_arr_size = sizeof(m_arr);
			constexpr auto m_arr2_size = sizeof(m_arr2);
		}
};

class multidimensional_assignment_test : public cmoon::test::test_case
{
	public:
		multidimensional_assignment_test()
			: cmoon::test::test_case{"multidimensional_assignment_test"} {}

		void operator()() override
		{
			cmoon::multidimensional_array<int> m_arr{3, 2};

			m_arr = {1, 2, 3, 4, 5, 6};

			cmoon::test::assert_equal(m_arr.get(0, 0), 1);
			cmoon::test::assert_equal(m_arr.get(0, 1), 2);
			cmoon::test::assert_equal(m_arr.get(1, 0), 3);

			cmoon::test::assert_equal(m_arr.get(1, 1), 4);
			cmoon::test::assert_equal(m_arr.get(2, 0), 5);
			cmoon::test::assert_equal(m_arr.get(2, 1), 6);

			cmoon::multidimensional_array<int> m_arr2{2, 3, 2};

			m_arr2 = {
				1, 2, 3,
				4, 5, 6,
					7, 8, 9,
					10, 11, 12
			};

			cmoon::test::assert_equal(m_arr2.get(0, 0, 0), 1);
			cmoon::test::assert_equal(m_arr2.get(0, 0, 1), 2);
			cmoon::test::assert_equal(m_arr2.get(0, 1, 0), 3);

			cmoon::test::assert_equal(m_arr2.get(0, 1, 1), 4);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 0), 5);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 1), 6);

			cmoon::test::assert_equal(m_arr2.get(1, 0, 0), 7);
			cmoon::test::assert_equal(m_arr2.get(1, 0, 1), 8);
			cmoon::test::assert_equal(m_arr2.get(1, 1, 0), 9);

			cmoon::test::assert_equal(m_arr2.get(1, 1, 1), 10);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 0), 11);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 1), 12);
		}
};

class multidimensional_value_test : public cmoon::test::test_case
{
	public:
		multidimensional_value_test()
			: cmoon::test::test_case{"multidimensional_value_test"} {}

		void operator()() override
		{
			cmoon::multidimensional_array<int> m_arr{3, 2};

			m_arr.get(0, 0) = 1;
			m_arr.get(0, 1) = 2;
			m_arr.get(1, 0) = 3;

			m_arr.get(1, 1) = 4;
			m_arr.get(2, 0) = 5;
			m_arr.get(2, 1) = 6;

			cmoon::test::assert_equal(m_arr.get(0, 0), 1);
			cmoon::test::assert_equal(m_arr.get(0, 1), 2);
			cmoon::test::assert_equal(m_arr.get(1, 0), 3);

			cmoon::test::assert_equal(m_arr.get(1, 1), 4);
			cmoon::test::assert_equal(m_arr.get(2, 0), 5);
			cmoon::test::assert_equal(m_arr.get(2, 1), 6);

			cmoon::multidimensional_array<int> m_arr2{2, 3, 2};

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

			cmoon::test::assert_equal(m_arr2.get(0, 0, 0), 1);
			cmoon::test::assert_equal(m_arr2.get(0, 0, 1), 2);
			cmoon::test::assert_equal(m_arr2.get(0, 1, 0), 3);

			cmoon::test::assert_equal(m_arr2.get(0, 1, 1), 4);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 0), 5);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 1), 6);

			cmoon::test::assert_equal(m_arr2.get(1, 0, 0), 7);
			cmoon::test::assert_equal(m_arr2.get(1, 0, 1), 8);
			cmoon::test::assert_equal(m_arr2.get(1, 1, 0), 9);

			cmoon::test::assert_equal(m_arr2.get(1, 1, 1), 10);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 0), 11);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 1), 12);
		}
};

class fixed_multidimensional_constructor_test : public cmoon::test::test_case
{
	public:
		fixed_multidimensional_constructor_test()
			: cmoon::test::test_case{"fixed_multidimensional_constructor_test"} {}

		void operator()() override
		{
			const cmoon::fixed_multidimensional_array<int, 2> m_arr{2, 3};
			cmoon::test::assert_equal(m_arr.dimensions(), std::array<std::size_t, 2>{2, 3});
			cmoon::test::assert_equal(m_arr.size(), 6);

			cmoon::test::assert_equal(m_arr.get(0, 0), 0);
			cmoon::test::assert_equal(m_arr.get(0, 1), 0);
			cmoon::test::assert_equal(m_arr.get(0, 2), 0);

			cmoon::test::assert_equal(m_arr.get(1, 0), 0);
			cmoon::test::assert_equal(m_arr.get(1, 1), 0);
			cmoon::test::assert_equal(m_arr.get(1, 2), 0);

			constexpr cmoon::constant_multidimensional_array<int, 2, 2> constant = {1, 2, 3, 4};

			cmoon::fixed_multidimensional_array<int, 2> goal{2, 2};
			goal = {
				1, 2,
				3, 4
			};

			cmoon::fixed_multidimensional_array<int, 2> m_arr2{constant};

			cmoon::test::assert_equal(m_arr2.get(0, 0), goal.get(0, 0));
			cmoon::test::assert_equal(m_arr2.get(0, 1), goal.get(0, 1));
			cmoon::test::assert_equal(m_arr2.get(1, 0), goal.get(1, 0));
			cmoon::test::assert_equal(m_arr2.get(1, 1), goal.get(1, 1));

			m_arr2 = constant;

			cmoon::test::assert_equal(m_arr2.get(0, 0), goal.get(0, 0));
			cmoon::test::assert_equal(m_arr2.get(0, 1), goal.get(0, 1));
			cmoon::test::assert_equal(m_arr2.get(1, 0), goal.get(1, 0));
			cmoon::test::assert_equal(m_arr2.get(1, 1), goal.get(1, 1));
		}

	private:
		void static_information()
		{
			const cmoon::fixed_multidimensional_array<int, 1> m_arr{2};
			const cmoon::fixed_multidimensional_array<int, 2> m_arr2{2, 3};
			const cmoon::fixed_multidimensional_array<int, 3> m_arr3{2, 3, 2};

			constexpr auto m_arr_size = sizeof(m_arr);
			constexpr auto m_arr2_size = sizeof(m_arr2);
			constexpr auto m_arr3_size = sizeof(m_arr3);
		}
};

class fixed_multidimensional_value_test : public cmoon::test::test_case
{
	public:
		fixed_multidimensional_value_test()
			: cmoon::test::test_case{"fixed_multidimensional_value_test"} {}

		void operator()() override
		{
			cmoon::fixed_multidimensional_array<int, 2> m_arr{3, 2};

			m_arr.get(0, 0) = 1;
			m_arr.get(0, 1) = 2;
			m_arr.get(1, 0) = 3;

			m_arr.get(1, 1) = 4;
			m_arr.get(2, 0) = 5;
			m_arr.get(2, 1) = 6;

			cmoon::test::assert_equal(m_arr.get(0, 0), 1);
			cmoon::test::assert_equal(m_arr.get(0, 1), 2);
			cmoon::test::assert_equal(m_arr.get(1, 0), 3);

			cmoon::test::assert_equal(m_arr.get(1, 1), 4);
			cmoon::test::assert_equal(m_arr.get(2, 0), 5);
			cmoon::test::assert_equal(m_arr.get(2, 1), 6);

			cmoon::fixed_multidimensional_array<int, 3> m_arr2{2, 3, 2};

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

			cmoon::test::assert_equal(m_arr2.get(0, 0, 0), 1);
			cmoon::test::assert_equal(m_arr2.get(0, 0, 1), 2);
			cmoon::test::assert_equal(m_arr2.get(0, 1, 0), 3);

			cmoon::test::assert_equal(m_arr2.get(0, 1, 1), 4);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 0), 5);
			cmoon::test::assert_equal(m_arr2.get(0, 2, 1), 6);

			cmoon::test::assert_equal(m_arr2.get(1, 0, 0), 7);
			cmoon::test::assert_equal(m_arr2.get(1, 0, 1), 8);
			cmoon::test::assert_equal(m_arr2.get(1, 1, 0), 9);

			cmoon::test::assert_equal(m_arr2.get(1, 1, 1), 10);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 0), 11);
			cmoon::test::assert_equal(m_arr2.get(1, 2, 1), 12);
		}
};

class constant_multidimensional_constructor_test : public cmoon::test::test_case
{
	public:
		constant_multidimensional_constructor_test()
			: cmoon::test::test_case{"constant_multidimensional_constructor_test"} {}

		void operator()() override
		{
			constexpr cmoon::constant_multidimensional_array<int, 2, 3, 2> m_arr
			{
				1, 2, 3,
				4, 5, 6,
					7, 8, 9,
					10, 11, 12
			};
			static_assert((sizeof(int) * 2 * 3 * 2) == sizeof(m_arr));
			static_assert(m_arr.dimensions() == std::array<std::size_t, 3>{2, 3, 2});
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

			constexpr cmoon::constant_multidimensional_array<int, 2, 3> m_arr2
			{
				1, 2, 3,
				4, 5, 6
			};
			static_assert((sizeof(int) * 2 * 3) == sizeof(m_arr2));
			static_assert(m_arr2.dimensions() == std::array<std::size_t, 2>{2, 3});
			static_assert(m_arr2.size() == 6);

			static_assert(m_arr2.get(0, 0) == 1);
			static_assert(m_arr2.get(0, 1) == 2);
			static_assert(m_arr2.get(0, 2) == 3);

			static_assert(m_arr2.get(1, 0) == 4);
			static_assert(m_arr2.get(1, 1) == 5);
			static_assert(m_arr2.get(1, 2) == 6);

			constexpr cmoon::constant_multidimensional_array<int, 3, 1> m_arr3
			{
				4,
				1,
				8
			};
			static_assert((sizeof(int) * 3 * 1) == sizeof(m_arr3));
			static_assert(m_arr3.dimensions() == std::array<std::size_t, 2>{3, 1});
			static_assert(m_arr3.size() == 3);

			static_assert(m_arr3.get(0, 0) == 4);
			static_assert(m_arr3.get(1, 0) == 1);
			static_assert(m_arr3.get(2, 0) == 8);
		}

	private:
		void static_information()
		{
			constexpr cmoon::constant_multidimensional_array<int, 2, 3, 2> m_arr2
			{
				1, 2, 3,
				4, 5, 6,
				7, 8, 9,
				10, 11, 12
			};

			constexpr cmoon::constant_multidimensional_array<int, 2, 3> m_arr
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
	cmoon::test::test_suite suite;

	suite.add_test_case<multidimensional_constructor_test>();
	suite.add_test_case<constant_multidimensional_constructor_test>();
	suite.add_test_case<multidimensional_value_test>();
	suite.add_test_case<multidimensional_assignment_test>();
	suite.add_test_case<fixed_multidimensional_value_test>();
	suite.add_test_case<fixed_multidimensional_constructor_test>();

	cmoon::test::text_test_runner runner(std::cout);

	return !runner.run(suite);
}