#include <tuple>
#include <utility>
#include <numeric>
#include <vector>
#include <functional>

#include "collin/algorithm.hpp"
#include "collin/test.hpp"
#include "collin/utility.hpp"
#include "collin/math.hpp"

class apply_range_test : public collin::test::test_case
{
	public:
		apply_range_test()
			: collin::test::test_case{"apply_range_test"} {}

		void operator()()
		{
			const std::array<std::function<void(int&)>, 4> functions = {
				std::function([](int& i){ i *= 2;}),
				std::function([](int& i) {i *= 2;}),
				std::function([](int& i) {i *= 2;}),
				std::function([](int& i) {i *= 2;})
			};

			const std::array<void(*)(int&), 4> functions2 = {
				[](int& i){ i += 2;},
				[](int& i) {i += 2;},
				[](int& i) {i += 2;},
				[](int& i) {i += 2;}
			};

			int value = 1;
			int value2 = 10;

			collin::algorithms::apply_range(std::begin(functions), std::end(functions), value);
			collin::algorithms::apply_range(std::begin(functions2), std::end(functions2), value2);

			collin::test::assert_equal(value, 16);
			collin::test::assert_equal(value2, 18);
		}
};

class remove_until_test : public collin::test::test_case
{
	public:
		remove_until_test()
			: collin::test::test_case{"remove_until_test"} {}

		void operator()() override
		{
			const std::vector<int> goal {6, 7, 8, 9, 10};

			std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
			collin::algorithms::remove_until(values, [](auto i){return i < 5;});

			collin::test::assert_equal(values, goal);
		}
};

int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<apply_range_test>();
	suite.add_test_case<remove_until_test>();

	collin::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}