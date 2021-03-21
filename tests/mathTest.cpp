#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

import cmoon.core;

class is_odd_test : public cmoon::test::test_case
{
	public:
		is_odd_test()
			: cmoon::test::test_case{"is_odd_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::is_odd(1), true);
			cmoon::test::assert_equal(cmoon::is_odd(2), false);
			cmoon::test::assert_equal(cmoon::is_odd(3), true);
			cmoon::test::assert_equal(cmoon::is_odd(4), false);
			cmoon::test::assert_equal(cmoon::is_odd(5), true);
			cmoon::test::assert_equal(cmoon::is_odd(6), false);
			cmoon::test::assert_equal(cmoon::is_odd(7), true);
			cmoon::test::assert_equal(cmoon::is_odd(8), false);
			cmoon::test::assert_equal(cmoon::is_odd(9), true);
			cmoon::test::assert_equal(cmoon::is_odd(10), false);
			cmoon::test::assert_equal(cmoon::is_odd(-3), true);
			cmoon::test::assert_equal(cmoon::is_odd(-2), false);
			cmoon::test::assert_equal(cmoon::is_odd(0), false);

			static_assert(cmoon::is_odd(1) == true);
			static_assert(cmoon::is_odd(2) == false);
			static_assert(cmoon::is_odd(3) == true);
			static_assert(cmoon::is_odd(4) == false);
			static_assert(cmoon::is_odd(5) == true);
			static_assert(cmoon::is_odd(6) == false);
			static_assert(cmoon::is_odd(7) == true);
			static_assert(cmoon::is_odd(8) == false);
			static_assert(cmoon::is_odd(9) == true);
			static_assert(cmoon::is_odd(10) == false);
			static_assert(cmoon::is_odd(-3) == true);
			static_assert(cmoon::is_odd(-2) == false);
			static_assert(cmoon::is_odd(0) == false);

		}
};

class is_even_test : public cmoon::test::test_case
{
public:
	is_even_test()
		: cmoon::test::test_case{"is_odd_test"} {}

	void operator()() override
	{
		cmoon::test::assert_equal(cmoon::is_even(1), false);
		cmoon::test::assert_equal(cmoon::is_even(2), true);
		cmoon::test::assert_equal(cmoon::is_even(3), false);
		cmoon::test::assert_equal(cmoon::is_even(4), true);
		cmoon::test::assert_equal(cmoon::is_even(5), false);
		cmoon::test::assert_equal(cmoon::is_even(6), true);
		cmoon::test::assert_equal(cmoon::is_even(7), false);
		cmoon::test::assert_equal(cmoon::is_even(8), true);
		cmoon::test::assert_equal(cmoon::is_even(9), false);
		cmoon::test::assert_equal(cmoon::is_even(10), true);
		cmoon::test::assert_equal(cmoon::is_even(-3), false);
		cmoon::test::assert_equal(cmoon::is_even(-2), true);
		cmoon::test::assert_equal(cmoon::is_even(0), true);

		static_assert(cmoon::is_even(1) == false);
		static_assert(cmoon::is_even(2) == true);
		static_assert(cmoon::is_even(3) == false);
		static_assert(cmoon::is_even(4) == true);
		static_assert(cmoon::is_even(5) == false);
		static_assert(cmoon::is_even(6) == true);
		static_assert(cmoon::is_even(7) == false);
		static_assert(cmoon::is_even(8) == true);
		static_assert(cmoon::is_even(9) == false);
		static_assert(cmoon::is_even(10) == true);
		static_assert(cmoon::is_even(-3) == false);
		static_assert(cmoon::is_even(-2) == true);
		static_assert(cmoon::is_even(0) == true);

	}
};

class factorial_test : public cmoon::test::test_case
{
	public:
		factorial_test()
			: cmoon::test::test_case{"factorial_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::factorial(1), 1);
			cmoon::test::assert_equal(cmoon::factorial(2), 2);
			cmoon::test::assert_equal(cmoon::factorial(3), 6);
			cmoon::test::assert_equal(cmoon::factorial(4), 24);
			cmoon::test::assert_equal(cmoon::factorial(5), 120);
			cmoon::test::assert_equal(cmoon::factorial(6), 720);
			cmoon::test::assert_equal(cmoon::factorial(7), 5040);
			cmoon::test::assert_equal(cmoon::factorial(8), 40320);
			cmoon::test::assert_equal(cmoon::factorial(9), 362880);
			cmoon::test::assert_equal(cmoon::factorial(10), 3628800);

			static_assert(cmoon::factorial(1) == 1);
			static_assert(cmoon::factorial(2) == 2);
			static_assert(cmoon::factorial(3) == 6);
			static_assert(cmoon::factorial(4) == 24);
			static_assert(cmoon::factorial(5) == 120);
			static_assert(cmoon::factorial(6) == 720);
			static_assert(cmoon::factorial(7) == 5040);
			static_assert(cmoon::factorial(8) == 40320);
			static_assert(cmoon::factorial(9) == 362880);
			static_assert(cmoon::factorial(10) == 3628800);

		}
};

class rational_test : public cmoon::test::test_case
{
	public:
		rational_test()
			: cmoon::test::test_case{"rational_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::rational(1), 1);
			cmoon::test::assert_equal(cmoon::rational(1, 2), 0.5);
			cmoon::test::assert_equal(cmoon::rational(1, 4), 0.25);
			cmoon::test::assert_equal(cmoon::rational(1, 2) + cmoon::rational(1, 2), 1);
			cmoon::test::assert_equal(cmoon::rational(1, 3) + cmoon::rational(1, 3) + cmoon::rational(1, 3), 1);
			cmoon::test::assert_equal(cmoon::rational(1, 3) * 3, 1);
			cmoon::test::assert_equal(cmoon::rational(12, 3), 4);
			cmoon::test::assert_equal(cmoon::rational(3, 1), 3);
			cmoon::test::assert_equal(cmoon::rational(-3, 1), -3);
			cmoon::test::assert_equal(cmoon::rational(3, -1), -3);
			cmoon::test::assert_equal(cmoon::mult_inverse(cmoon::rational(1, -3)), -3);
			cmoon::test::assert_equal(cmoon::add_inverse(cmoon::rational(-5, 1)), 5);
			cmoon::test::assert_equal(cmoon::add_inverse(cmoon::rational(5, 1)), -5);
			cmoon::test::assert_equal(cmoon::canonical(cmoon::rational(12, 3)), cmoon::rational(4, 1));

			static_assert(cmoon::rational(1) == 1);
			static_assert(cmoon::rational(1, 3) < cmoon::rational(1, 2));
			static_assert(cmoon::rational(1) > cmoon::rational(1, 5));
			static_assert(cmoon::rational(2, 5) >= cmoon::rational(20, 50));
			static_assert(cmoon::rational(1, 2) == 0.5);
			static_assert(cmoon::rational(1, 4) == 0.25);
			static_assert(cmoon::rational(1, 2) + cmoon::rational(1, 2) == 1);
			static_assert(cmoon::rational(1, 3) + cmoon::rational(1, 3) + cmoon::rational(1, 3) == 1);
			static_assert(cmoon::rational(1, 3) * 3 == 1);
			static_assert(cmoon::rational(12, 3) == 4);
			static_assert(cmoon::rational(3, 1) == 3);
			static_assert(cmoon::rational(-3, 1) == -3);
			static_assert(cmoon::rational(3, -1) == -3);
			static_assert(cmoon::mult_inverse(cmoon::rational(1, -3)) == -3);
			static_assert(cmoon::add_inverse(cmoon::rational(-5, 1)) == 5);
			static_assert(cmoon::add_inverse(cmoon::rational(5, 1)) == -5);
			static_assert(cmoon::canonical(cmoon::rational(12, 3)) == cmoon::rational(4, 1));
		}
};

class pow_test : public cmoon::test::test_case
{
	public:
		pow_test()
			: cmoon::test::test_case{"pow_test"} {}

		void operator()() override
		{

		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<is_odd_test>();
	suite.add_test_case<is_even_test>();
	suite.add_test_case<factorial_test>();
	suite.add_test_case<rational_test>();
	suite.add_test_case<pow_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}