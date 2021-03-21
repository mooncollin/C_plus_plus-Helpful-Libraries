#include <iostream>
#include <vector>
#include <list>

#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"

#include "cmoon/statistics.hpp"

class average_test : public cmoon::test::test_case
{
	public:
		average_test()
			: cmoon::test::test_case{"average_test"} {}

		void operator()() override
		{
			constexpr auto goal = 22.142857142857;

			const std::vector values = {10, 2, 38, 23, 38, 23, 21};
			const std::list values2 = {10, 2, 38, 23, 38, 23, 21};

			cmoon::test::assert_almost_equal(cmoon::average(values), goal, .0000001);
			cmoon::test::assert_almost_equal(cmoon::average({10, 2, 38, 23, 38, 23, 21}), goal, .0000001);
			cmoon::test::assert_almost_equal(cmoon::average(values2), goal, .0000001);
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<average_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}