#include <iostream>
#include <cstdio>

#include "cmoon/scope.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"

class unique_resource_initialization_test : public cmoon::test::test_case
{
	public:
		unique_resource_initialization_test()
			: cmoon::test::test_case {"unique_resource_initialization_test"} {}

		void operator()() override
		{
			auto file = cmoon::make_unique_resource_checked(
				std::fopen("potentially_nonexistent_file.txt", "r"),
				nullptr,
				[](std::FILE* fptr) { std::fclose(fptr); }
			);

			cmoon::test::assert_equal(file.get(), nullptr);

			constexpr int goal = 1;
			int value = 0;

			{
				auto test = cmoon::make_unique_resource_checked(
					value, // Dummy value
					10, // Dummy value
					[&value](int) { value++; }
				);

				cmoon::test::assert_equal(test.get(), value);
			}

			cmoon::test::assert_equal(value, goal);
		}
};

class scope_exit_test : public cmoon::test::test_case
{
	public:
		scope_exit_test()
			: cmoon::test::test_case{"scope_exit_test"} {}

		void operator()() override
		{
			constexpr int goal1 = 1;
			int value1 = 0;

			{
				cmoon::scope_exit se {[&value1](){ value1++; }};
			}

			cmoon::test::assert_equal(value1, goal1);

			constexpr int goal2 = 0;
			int value2 = 0;

			{
				cmoon::scope_exit se {[&value2](){ value2++; }};
				se.release();
			}

			cmoon::test::assert_equal(value2, goal2);
		}
};

class scope_success_test : public cmoon::test::test_case
{
	public:
		scope_success_test()
			: cmoon::test::test_case{"scope_success_test"} {}

		void operator()() override
		{
			constexpr int goal1 = 1;
			int value1 = 0;

			{
				cmoon::scope_success sc {[&value1](){ value1++; }};
			}

			cmoon::test::assert_equal(value1, goal1);

			constexpr int goal2 = 0;
			int value2 = 0;

			try
			{
				{
					cmoon::scope_success sc {[&value2](){ value2++; }};
					throw "";
				}
			}
			catch (...)
			{
			}

			cmoon::test::assert_equal(value2, goal2);

			constexpr int goal3 = 0;
			int value3 = 0;

			{
				cmoon::scope_success sc {[&value1](){ value1++; }};
				sc.release();
			}

			cmoon::test::assert_equal(value3, goal3);
		}
};

class scope_fail_test : public cmoon::test::test_case
{
	public:
		scope_fail_test()
			: cmoon::test::test_case{"scope_fail_test"} {}

		void operator()() override
		{
			constexpr int goal1 = 1;
			int value1 = 0;

			try
			{
				{
					cmoon::scope_fail sf {[&value1](){ value1++; }};
					throw "";
				}
			}
			catch (...)
			{
			}

			cmoon::test::assert_equal(value1, goal1);

			constexpr int goal2 = 0;
			int value2 = 0;

			{
				cmoon::scope_fail sf {[&value2](){ value2++; }};
			}

			cmoon::test::assert_equal(value2, goal2);

			constexpr int goal3 = 0;
			int value3 = 0;

			try
			{
				{
					cmoon::scope_fail sf {[&value3](){ value3++; }};
					sf.release();
					throw "";
				}
			}
			catch (...)
			{
			}

			cmoon::test::assert_equal(value3, goal3);
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<unique_resource_initialization_test>();
	suite.add_test_case<scope_exit_test>();
	suite.add_test_case<scope_success_test>();
	suite.add_test_case<scope_fail_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}