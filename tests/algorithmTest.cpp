#include "collin/algorithm.hpp"
#include "collin/test.hpp"
#include "collin/utility.hpp"
#include "collin/math.hpp"
#include <tuple>
#include <utility>
#include <numeric>
#include <vector>
#include <functional>

void apply_range_test()
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

	std::vector<std::pair<int, int>> test_values;
	test_values.emplace_back(value, 16);
	test_values.emplace_back(value2, 18);

	collin::test::assert_test_data(test_values, "apply_range");
}

void remove_until_test()
{
	std::vector<int> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	collin::algorithms::remove_until(values, [](auto i){return i < 5;});

	std::vector<std::pair<std::vector<int>, std::vector<int>>> test_values;
	test_values.emplace_back(values, std::vector<int>{6, 7, 8, 9, 10});

	collin::test::assert_test_data(test_values, "remove_until");
}

int main()
{
	apply_range_test();
	remove_until_test();
}