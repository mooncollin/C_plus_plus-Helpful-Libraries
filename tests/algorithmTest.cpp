#include "collin_algorithm.hpp"
#include "collin_test.hpp"
#include "collin_utility.hpp"
#include "collin_math.hpp"
#include <tuple>
#include <utility>
#include <numeric>
#include <array>

void for_each_test()
{
	const auto sum_f = [] (auto container) {
		auto sum = collin::construct_value(container);

		collin::algorithms::for_each(container, [&sum](const auto& value) {
			sum += value;
		});

		return sum;
	};

	const auto test_values = collin::test::make_test_data
	(
		sum_f,
			{
				std::make_pair(std::make_tuple(std::vector {1, 2, 3, 4, 5}), 15)
			}
	);

	collin::test::assert_test_data(test_values, "for_each Sum");
}

void for_each_n_test()
{
	const auto sum_f = [] ( auto container) {
		auto sum = collin::construct_value(container);
		
		collin::algorithms::for_each_n(container, 3, [&sum](const auto _, const auto& value) {
			sum += value;
		});

		return sum;
	};

	const auto test_values = collin::test::make_test_data
	(
		sum_f,
			{
				std::make_pair(std::make_tuple(std::vector {1, 2, 3, 4, 5}), 6)
			}
	);

	collin::test::assert_test_data(test_values, "for_each_n Sum");
}

void any_of_test()
{
	const auto f = [] (auto container, auto pred)
	{
		return collin::algorithms::any_of(container, pred);
	};

	const auto test_values = collin::test::make_test_data
	(
		f,
		{
			std::make_pair(std::make_tuple(std::vector {true, false, true}, [](const auto& item){return item;}), true)
		}
	);

	const auto test_values_2 = collin::test::make_test_data
	(
		f,
		{
			std::make_pair(std::make_tuple(std::vector {1, 2, 3}, collin::math::is_even<int>), true),
			std::make_pair(std::make_tuple(std::vector {1, 3, 5, 9}, collin::math::is_even<int>), false)
		}
	);

	collin::test::assert_test_data(test_values, "any_of bool");
	collin::test::assert_test_data(test_values_2, "any_of even");
}

int main()
{
	for_each_test();
	for_each_n_test();
	any_of_test();
}