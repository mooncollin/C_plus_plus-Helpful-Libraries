#include "collin_math.hpp"
#include "collin_test.hpp"
#include <tuple>
#include <random>
#include <array>
#include <chrono>

void test_is_odd()
{
	const auto is_odd_test_values = collin::make_test_data
	(
		collin::is_odd<int>,
			{
				std::make_pair(std::make_tuple(1), true),
				std::make_pair(std::make_tuple(2), false),
				std::make_pair(std::make_tuple(3), true),
				std::make_pair(std::make_tuple(4), false),
				std::make_pair(std::make_tuple(5), true),
				std::make_pair(std::make_tuple(6), false),
				std::make_pair(std::make_tuple(7), true),
				std::make_pair(std::make_tuple(8), false),
				std::make_pair(std::make_tuple(9), true),
				std::make_pair(std::make_tuple(10), false),
				std::make_pair(std::make_tuple(-3), true),
				std::make_pair(std::make_tuple(-2), false),
				std::make_pair(std::make_tuple(0), false)
			}
	);

	const auto is_odd_float_test_values = collin::make_test_data
	(
		collin::is_odd<float>,
			{
				std::make_pair(std::make_tuple(4.5), false),
				std::make_pair(std::make_tuple(3.3), true),
				std::make_pair(std::make_tuple(-55.3), true),
			}
	);

	const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> distribution;

	std::array<int, 10> random_nums;
	for(std::size_t i = 0; i < random_nums.size(); i++)
	{
		random_nums[i] = distribution(generator);
	}

	const auto is_odd_random_test_values = collin::make_test_data
	(
		collin::is_odd<int>,
			{
				std::make_pair(std::make_tuple(random_nums[0]), random_nums[0] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[1]), random_nums[1] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[2]), random_nums[2] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[3]), random_nums[3] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[4]), random_nums[4] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[5]), random_nums[5] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[6]), random_nums[6] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[7]), random_nums[7] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[8]), random_nums[8] % 2 != 0),
				std::make_pair(std::make_tuple(random_nums[9]), random_nums[9] % 2 != 0),
			}
	);

	collin::assert_test_data(is_odd_test_values, "is_odd Normal");
	collin::assert_test_data(is_odd_float_test_values, "is_odd Float");
	collin::assert_test_data(is_odd_random_test_values, "is_odd Random");
}

void test_is_even()
{
	const auto is_even_test_values = collin::make_test_data
	(
		collin::is_even<int>,
			{
				std::make_pair(std::make_tuple(1), false),
				std::make_pair(std::make_tuple(2), true),
				std::make_pair(std::make_tuple(3), false),
				std::make_pair(std::make_tuple(4), true),
				std::make_pair(std::make_tuple(5), false),
				std::make_pair(std::make_tuple(6), true),
				std::make_pair(std::make_tuple(7), false),
				std::make_pair(std::make_tuple(8), true),
				std::make_pair(std::make_tuple(9), false),
				std::make_pair(std::make_tuple(10), true),
				std::make_pair(std::make_tuple(-3), false),
				std::make_pair(std::make_tuple(-2), true),
				std::make_pair(std::make_tuple(0), true)
			}
	);

	const auto is_even_float_test_values = collin::make_test_data
	(
		collin::is_even<float>,
			{
				std::make_pair(std::make_tuple(4.5), true),
				std::make_pair(std::make_tuple(3.3), false),
				std::make_pair(std::make_tuple(-55.3), false),
			}
	);

	const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> distribution;

	std::array<int, 10> random_nums;
	for(std::size_t i = 0; i < random_nums.size(); i++)
	{
		random_nums[i] = distribution(generator);
	}

	const auto is_even_random_test_values = collin::make_test_data
	(
		collin::is_even<int>,
			{
				std::make_pair(std::make_tuple(random_nums[0]), random_nums[0] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[1]), random_nums[1] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[2]), random_nums[2] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[3]), random_nums[3] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[4]), random_nums[4] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[5]), random_nums[5] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[6]), random_nums[6] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[7]), random_nums[7] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[8]), random_nums[8] % 2 == 0),
				std::make_pair(std::make_tuple(random_nums[9]), random_nums[9] % 2 == 0),
			}
	);

	collin::assert_test_data(is_even_test_values, "is_even Normal");
	collin::assert_test_data(is_even_float_test_values, "is_even Float");
	collin::assert_test_data(is_even_random_test_values, "is_even Random");
}

void test_factorial()
{
	const auto test_values = collin::make_test_data
	(
		collin::factorial,
			{
				std::make_pair(std::make_tuple(1), 1),
				std::make_pair(std::make_tuple(2), 2),
				std::make_pair(std::make_tuple(3), 6),
				std::make_pair(std::make_tuple(4), 24),
				std::make_pair(std::make_tuple(5), 120),
				std::make_pair(std::make_tuple(6), 720),
				std::make_pair(std::make_tuple(7), 5040),
				std::make_pair(std::make_tuple(8), 40320),
				std::make_pair(std::make_tuple(9), 362880),
				std::make_pair(std::make_tuple(10), 3628800)
			}
	);

	collin::assert_test_data(test_values, "factorial Normal");
}

int main()
{
	test_is_odd();
	test_is_even();
	test_factorial();
}