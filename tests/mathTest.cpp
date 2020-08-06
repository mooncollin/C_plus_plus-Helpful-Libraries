#include <tuple>
#include <random>
#include <array>
#include <chrono>

#include "collin/math.hpp"
#include "collin/test.hpp"

class is_odd_test : public collin::test::test_case
{
	public:
		is_odd_test()
			: collin::test::test_case{"is_odd_test"} {}

		void operator()() override
		{
			collin::test::assert_equal(collin::math::is_odd(1), true);
			collin::test::assert_equal(collin::math::is_odd(2), false);
			collin::test::assert_equal(collin::math::is_odd(3), true);
			collin::test::assert_equal(collin::math::is_odd(4), false);
			collin::test::assert_equal(collin::math::is_odd(5), true);
			collin::test::assert_equal(collin::math::is_odd(6), false);
			collin::test::assert_equal(collin::math::is_odd(7), true);
			collin::test::assert_equal(collin::math::is_odd(8), false);
			collin::test::assert_equal(collin::math::is_odd(9), true);
			collin::test::assert_equal(collin::math::is_odd(10), false);
			collin::test::assert_equal(collin::math::is_odd(-3), true);
			collin::test::assert_equal(collin::math::is_odd(-2), false);
			collin::test::assert_equal(collin::math::is_odd(0), false);

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
};

class is_even_test : public collin::test::test_case
{
public:
	is_even_test()
		: collin::test::test_case{"is_odd_test"} {}

	void operator()() override
	{
		collin::test::assert_equal(collin::math::is_even(1), false);
		collin::test::assert_equal(collin::math::is_even(2), true);
		collin::test::assert_equal(collin::math::is_even(3), false);
		collin::test::assert_equal(collin::math::is_even(4), true);
		collin::test::assert_equal(collin::math::is_even(5), false);
		collin::test::assert_equal(collin::math::is_even(6), true);
		collin::test::assert_equal(collin::math::is_even(7), false);
		collin::test::assert_equal(collin::math::is_even(8), true);
		collin::test::assert_equal(collin::math::is_even(9), false);
		collin::test::assert_equal(collin::math::is_even(10), true);
		collin::test::assert_equal(collin::math::is_even(-3), false);
		collin::test::assert_equal(collin::math::is_even(-2), true);
		collin::test::assert_equal(collin::math::is_even(0), true);

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
};

class factorial_test : public collin::test::test_case
{
	public:
		factorial_test()
			: collin::test::test_case{"factorial_test"} {}

		void operator()() override
		{
			collin::test::assert_equal(collin::math::factorial(1), 1);
			collin::test::assert_equal(collin::math::factorial(2), 2);
			collin::test::assert_equal(collin::math::factorial(3), 6);
			collin::test::assert_equal(collin::math::factorial(4), 24);
			collin::test::assert_equal(collin::math::factorial(5), 120);
			collin::test::assert_equal(collin::math::factorial(6), 720);
			collin::test::assert_equal(collin::math::factorial(7), 5040);
			collin::test::assert_equal(collin::math::factorial(8), 40320);
			collin::test::assert_equal(collin::math::factorial(9), 362880);
			collin::test::assert_equal(collin::math::factorial(10), 3628800);

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
};

class rational_test : public collin::test::test_case
{
	public:
		rational_test()
			: collin::test::test_case{"rational_test"} {}

		void operator()() override
		{
			collin::test::assert_equal(collin::math::rational(1), 1);
			collin::test::assert_equal(collin::math::rational(1, 2), 0.5);
			collin::test::assert_equal(collin::math::rational(1, 4), 0.25);
			collin::test::assert_equal(collin::math::rational(1, 2) + collin::math::rational(1, 2), 1);
			collin::test::assert_equal(collin::math::rational(1, 3) + collin::math::rational(1, 3) + collin::math::rational(1, 3), 1);
			collin::test::assert_equal(collin::math::rational(1, 3) * 3, 1);
			collin::test::assert_equal(collin::math::rational(12, 3), 4);
			collin::test::assert_equal(collin::math::rational(3, 1), 3);
			collin::test::assert_equal(collin::math::rational(-3, 1), -3);
			collin::test::assert_equal(collin::math::rational(3, -1), -3);
			collin::test::assert_equal(collin::math::mult_inverse(collin::math::rational(1, -3)), -3);
			collin::test::assert_equal(collin::math::add_inverse(collin::math::rational(-5, 1)), 5);
			collin::test::assert_equal(collin::math::add_inverse(collin::math::rational(5, 1)), -5);
			collin::test::assert_equal(collin::math::canonical(collin::math::rational(12, 3)), collin::math::rational(4, 1));


			static_assert(collin::math::rational(1) == 1);
			static_assert(collin::math::rational(1, 2) == 0.5);
			static_assert(collin::math::rational(1, 4) == 0.25);
			static_assert(collin::math::rational(1, 2) + collin::math::rational(1, 2) == 1);
			static_assert(collin::math::rational(1, 3) + collin::math::rational(1, 3) + collin::math::rational(1, 3) == 1);
			static_assert(collin::math::rational(1, 3) * 3 == 1);
			static_assert(collin::math::rational(12, 3) == 4);
			static_assert(collin::math::rational(3, 1) == 3);
			static_assert(collin::math::rational(-3, 1) == -3);
			static_assert(collin::math::rational(3, -1) == -3);
			static_assert(collin::math::mult_inverse(collin::math::rational(1, -3)) == -3);
			static_assert(collin::math::add_inverse(collin::math::rational(-5, 1)) == 5);
			static_assert(collin::math::add_inverse(collin::math::rational(5, 1)) == -5);
			static_assert(collin::math::canonical(collin::math::rational(12, 3)) == collin::math::rational(4, 1));
		}
};

class pow_test : public collin::test::test_case
{
	public:
		pow_test()
			: collin::test::test_case{"pow_test"} {}

		void operator()() override
		{

		}
};

int main()
{
	collin::test::test_suite suite;
	suite.add_test_case<is_odd_test>();
	suite.add_test_case<is_even_test>();
	suite.add_test_case<factorial_test>();
	suite.add_test_case<rational_test>();
	suite.add_test_case<pow_test>();

	collin::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}