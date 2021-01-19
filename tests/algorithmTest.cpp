#include <vector>
#include <iostream>
#include <array>

#include "cmoon/algorithm.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"

class repeat_test : public cmoon::test::test_case
{
	public:
		repeat_test()
			: cmoon::test::test_case{"repeat_test"} {}

		void operator()() override
		{
			constexpr int goal1 {5};
			int test1 {0};
			const auto func = [&test1](){ test1++; };

			cmoon::repeat(5, func);

			cmoon::test::assert_equal(test1, goal1);
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<repeat_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}