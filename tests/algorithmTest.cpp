#include "collin_algorithm.hpp"
#include "collin_test.hpp"
#include <tuple>
#include <utility>

void for_each_test()
{
	const auto sum_f = [] (auto container) {
		typename decltype(container)::value_type sum {};

		collin::for_each(container, [&](const auto& val) {
			sum += val;
		});

		return sum;
	};

	const auto test_values = collin::make_test_data
	(
		sum_f,
			{
				std::make_pair(std::make_tuple(std::vector<int> {1, 2, 3, 4, 5}), 15)
			}
	);

	collin::assert_test_data(test_values, "for_each Sum");
}

int main()
{
	for_each_test();
}