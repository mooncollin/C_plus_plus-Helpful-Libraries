#include "collin/math.hpp"
#include "collin/test.hpp"
#include <tuple>
#include <random>
#include <array>
#include <chrono>

void test_is_odd()
{
	const auto is_odd_test_values = collin::test::make_test_data
	(
		collin::math::is_odd<int>,
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

	const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> distribution;

	std::array<int, 10> random_nums;
	for(std::size_t i = 0; i < random_nums.size(); i++)
	{
		random_nums[i] = distribution(generator);
	}

	const auto is_odd_random_test_values = collin::test::make_test_data
	(
		collin::math::is_odd<int>,
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

	collin::test::assert_test_data(is_odd_test_values, "is_odd Normal");
	collin::test::assert_test_data(is_odd_random_test_values, "is_odd Random");

	static_assert(collin::math::is_odd(1) == true);
	static_assert(collin::math::is_odd(2) == false);
	static_assert(collin::math::is_odd(3) == true);
	static_assert(collin::math::is_odd(4) == false);
	static_assert(collin::math::is_odd(5) == true);
	static_assert(collin::math::is_odd(6) == false);
	static_assert(collin::math::is_odd(7) == true);
	static_assert(collin::math::is_odd(8) == false);
	static_assert(collin::math::is_odd(9) == true);
	static_assert(collin::math::is_odd(10) == false);
	static_assert(collin::math::is_odd(-3) == true);
	static_assert(collin::math::is_odd(-2) == false);
	static_assert(collin::math::is_odd(0) == false);
}

void test_is_even()
{
	const auto is_even_test_values = collin::test::make_test_data
	(
		collin::math::is_even<int>,
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

	const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_int_distribution<int> distribution;

	std::array<int, 10> random_nums;
	for(std::size_t i = 0; i < random_nums.size(); i++)
	{
		random_nums[i] = distribution(generator);
	}

	const auto is_even_random_test_values = collin::test::make_test_data
	(
		collin::math::is_even<int>,
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

	collin::test::assert_test_data(is_even_test_values, "is_even Normal");
	collin::test::assert_test_data(is_even_random_test_values, "is_even Random");

	static_assert(collin::math::is_even(1) == false);
	static_assert(collin::math::is_even(2) == true);
	static_assert(collin::math::is_even(3) == false);
	static_assert(collin::math::is_even(4) == true);
	static_assert(collin::math::is_even(5) == false);
	static_assert(collin::math::is_even(6) == true);
	static_assert(collin::math::is_even(7) == false);
	static_assert(collin::math::is_even(8) == true);
	static_assert(collin::math::is_even(9) == false);
	static_assert(collin::math::is_even(10) == true);
	static_assert(collin::math::is_even(-3) == false);
	static_assert(collin::math::is_even(-2) == true);
	static_assert(collin::math::is_even(0) == true);
}

void test_factorial()
{
	const auto test_values = collin::test::make_test_data
	(
		collin::math::factorial,
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

	collin::test::assert_test_data(test_values, "factorial Normal");

	static_assert(collin::math::factorial(1) == 1);
	static_assert(collin::math::factorial(2) == 2);
	static_assert(collin::math::factorial(3) == 6);
	static_assert(collin::math::factorial(4) == 24);
	static_assert(collin::math::factorial(5) == 120);
	static_assert(collin::math::factorial(6) == 720);
	static_assert(collin::math::factorial(7) == 5040);
	static_assert(collin::math::factorial(8) == 40320);
	static_assert(collin::math::factorial(9) == 362880);
	static_assert(collin::math::factorial(10) == 3628800);
}

void test_rational()
{
	static_assert(collin::math::Rational(1) == 1);
	static_assert(collin::math::Rational(1, 2) == 0.5);
	static_assert(collin::math::Rational(1, 4) == 0.25);
	static_assert(collin::math::Rational(1, 2) + collin::math::Rational(1, 2) == 1);
	static_assert(collin::math::Rational(1, 3) + collin::math::Rational(1, 3) + collin::math::Rational(1, 3) == 1);
	static_assert(collin::math::Rational(1, 3) * 3 == 1);
	static_assert(collin::math::Rational(12, 3) == 4);
	static_assert(collin::math::Rational(3, 1) == 3);
	static_assert(collin::math::Rational(-3, 1) == -3);
	static_assert(collin::math::Rational(3, -1) == -3);
	static_assert(collin::math::mult_inverse(collin::math::Rational(1, -3)) == -3);
	static_assert(collin::math::add_inverse(collin::math::Rational(-5, 1)) == 5);
	static_assert(collin::math::add_inverse(collin::math::Rational(5, 1)) == -5);
	static_assert(collin::math::canonical(collin::math::Rational(12, 3)) == collin::math::Rational(4, 1));
}

int main()
{
	test_is_odd();
	test_is_even();
	test_factorial();
	test_rational();
}