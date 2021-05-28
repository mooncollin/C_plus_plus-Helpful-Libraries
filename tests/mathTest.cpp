import <iostream>;

import cmoon.test;
import cmoon.math;

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
			cmoon::test::assert_equal(cmoon::pow(2, 0), 1);
			cmoon::test::assert_equal(cmoon::pow(2, 1), 2);
			cmoon::test::assert_equal(cmoon::pow(2, 2), 4);
			cmoon::test::assert_equal(cmoon::pow(2, 3), 8);
			cmoon::test::assert_equal(cmoon::pow(34, 3), 39304);
			cmoon::test::assert_equal(cmoon::pow(9, 5), 59049);
			cmoon::test::assert_almost_equal(cmoon::pow(1.5, 5), 7.59375, 0.000001);
			cmoon::test::assert_almost_equal(cmoon::pow(9, 0.5), 3.0, 0.01);

			static_assert(cmoon::pow(2, 0) == 1);
			static_assert(cmoon::pow(2, 1) == 2);
			static_assert(cmoon::pow(2, 2) == 4);
			static_assert(cmoon::pow(2, 3) == 8);
			static_assert(cmoon::pow(34, 3) == 39304);
			static_assert(cmoon::pow(9, 5) == 59049);
			static_assert(cmoon::abs(cmoon::pow(1.5, 5) - 7.59375) <= 0.000001);
		}
};

class static_pow_test : public cmoon::test::test_case
{
	public:
		static_pow_test()
			: cmoon::test::test_case{"static_pow_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::static_pow<0>(2), 1);
			cmoon::test::assert_equal(cmoon::static_pow<1>(2), 2);
			cmoon::test::assert_equal(cmoon::static_pow<2>(2), 4);
			cmoon::test::assert_equal(cmoon::static_pow<3>(2), 8);
			cmoon::test::assert_equal(cmoon::static_pow<3>(34), 39304);
			cmoon::test::assert_equal(cmoon::static_pow<5>(9), 59049);

			static_assert(cmoon::static_pow<0>(2) == 1);
			static_assert(cmoon::static_pow<1>(2) == 2);
			static_assert(cmoon::static_pow<2>(2) == 4);
			static_assert(cmoon::static_pow<3>(2) == 8);
			static_assert(cmoon::static_pow<3>(34) == 39304);
			static_assert(cmoon::static_pow<5>(9) == 59049);
		}
};

class abs_test : public cmoon::test::test_case
{
	public:
		abs_test()
			: cmoon::test::test_case{"abs_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::abs(1), 1);
			cmoon::test::assert_equal(cmoon::abs(2), 2);
			cmoon::test::assert_equal(cmoon::abs(3), 3);
			cmoon::test::assert_equal(cmoon::abs(4), 4);
			cmoon::test::assert_equal(cmoon::abs(5), 5);
			cmoon::test::assert_equal(cmoon::abs(6), 6);
			cmoon::test::assert_equal(cmoon::abs(7), 7);
			cmoon::test::assert_equal(cmoon::abs(8), 8);
			cmoon::test::assert_equal(cmoon::abs(9), 9);
			cmoon::test::assert_equal(cmoon::abs(10), 10);

			cmoon::test::assert_equal(cmoon::abs(-1), 1);
			cmoon::test::assert_equal(cmoon::abs(-2), 2);
			cmoon::test::assert_equal(cmoon::abs(-3), 3);
			cmoon::test::assert_equal(cmoon::abs(-4), 4);
			cmoon::test::assert_equal(cmoon::abs(-5), 5);
			cmoon::test::assert_equal(cmoon::abs(-6), 6);
			cmoon::test::assert_equal(cmoon::abs(-7), 7);
			cmoon::test::assert_equal(cmoon::abs(-8), 8);
			cmoon::test::assert_equal(cmoon::abs(-9), 9);
			cmoon::test::assert_equal(cmoon::abs(-10), 10);

			cmoon::test::assert_equal(cmoon::abs(1.0), 1.0);
			cmoon::test::assert_equal(cmoon::abs(2.0), 2.0);
			cmoon::test::assert_equal(cmoon::abs(3.0), 3.0);
			cmoon::test::assert_equal(cmoon::abs(4.0), 4.0);
			cmoon::test::assert_equal(cmoon::abs(5.0), 5.0);
			cmoon::test::assert_equal(cmoon::abs(6.0), 6.0);
			cmoon::test::assert_equal(cmoon::abs(7.0), 7.0);
			cmoon::test::assert_equal(cmoon::abs(8.0), 8.0);
			cmoon::test::assert_equal(cmoon::abs(9.0), 9.0);
			cmoon::test::assert_equal(cmoon::abs(10.0), 10.0);

			cmoon::test::assert_equal(cmoon::abs(-1.0), 1.0);
			cmoon::test::assert_equal(cmoon::abs(-2.0), 2.0);
			cmoon::test::assert_equal(cmoon::abs(-3.0), 3.0);
			cmoon::test::assert_equal(cmoon::abs(-4.0), 4.0);
			cmoon::test::assert_equal(cmoon::abs(-5.0), 5.0);
			cmoon::test::assert_equal(cmoon::abs(-6.0), 6.0);
			cmoon::test::assert_equal(cmoon::abs(-7.0), 7.0);
			cmoon::test::assert_equal(cmoon::abs(-8.0), 8.0);
			cmoon::test::assert_equal(cmoon::abs(-9.0), 9.0);
			cmoon::test::assert_equal(cmoon::abs(-10.0), 10.0);

			static_assert(cmoon::abs(1) == 1);
			static_assert(cmoon::abs(2) == 2);
			static_assert(cmoon::abs(3) == 3);
			static_assert(cmoon::abs(4) == 4);
			static_assert(cmoon::abs(5) == 5);
			static_assert(cmoon::abs(6) == 6);
			static_assert(cmoon::abs(7) == 7);
			static_assert(cmoon::abs(8) == 8);
			static_assert(cmoon::abs(9) == 9);
			static_assert(cmoon::abs(10) == 10);

			static_assert(cmoon::abs(-1) == 1);
			static_assert(cmoon::abs(-2) == 2);
			static_assert(cmoon::abs(-3) == 3);
			static_assert(cmoon::abs(-4) == 4);
			static_assert(cmoon::abs(-5) == 5);
			static_assert(cmoon::abs(-6) == 6);
			static_assert(cmoon::abs(-7) == 7);
			static_assert(cmoon::abs(-8) == 8);
			static_assert(cmoon::abs(-9) == 9);
			static_assert(cmoon::abs(-10) == 10);

			static_assert(cmoon::abs(1.0) == 1.0);
			static_assert(cmoon::abs(2.0) == 2.0);
			static_assert(cmoon::abs(3.0) == 3.0);
			static_assert(cmoon::abs(4.0) == 4.0);
			static_assert(cmoon::abs(5.0) == 5.0);
			static_assert(cmoon::abs(6.0) == 6.0);
			static_assert(cmoon::abs(7.0) == 7.0);
			static_assert(cmoon::abs(8.0) == 8.0);
			static_assert(cmoon::abs(9.0) == 9.0);
			static_assert(cmoon::abs(10.0) == 10.0);

			static_assert(cmoon::abs(-1.0) == 1.0);
			static_assert(cmoon::abs(-2.0) == 2.0);
			static_assert(cmoon::abs(-3.0) == 3.0);
			static_assert(cmoon::abs(-4.0) == 4.0);
			static_assert(cmoon::abs(-5.0) == 5.0);
			static_assert(cmoon::abs(-6.0) == 6.0);
			static_assert(cmoon::abs(-7.0) == 7.0);
			static_assert(cmoon::abs(-8.0) == 8.0);
			static_assert(cmoon::abs(-9.0) == 9.0);
			static_assert(cmoon::abs(-10.0) == 10.0);
		}
};

class ceil_test : public cmoon::test::test_case
{
	public:
		ceil_test()
			: cmoon::test::test_case{"ceil_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::ceil(5), 5);
			cmoon::test::assert_equal(cmoon::ceil(5.6), 6);
			cmoon::test::assert_equal(cmoon::ceil(-1), -1);
			cmoon::test::assert_equal(cmoon::ceil(-1.2), -1);

			static_assert(cmoon::ceil(5) == 5);
			static_assert(cmoon::ceil(5.6) == 6);
			static_assert(cmoon::ceil(-1) == -1);
			static_assert(cmoon::ceil(-1.2) == -1);
		}
};

class floor_test : public cmoon::test::test_case
{
	public:
		floor_test()
			: cmoon::test::test_case{"floor_test"} {}

		void operator()()
		{
			cmoon::test::assert_equal(cmoon::floor(5), 5);
			cmoon::test::assert_equal(cmoon::floor(5.6), 5);
			cmoon::test::assert_equal(cmoon::floor(-1), -1);
			cmoon::test::assert_equal(cmoon::floor(-1.2), -2);

			static_assert(cmoon::floor(5) == 5);
			static_assert(cmoon::floor(5.6) == 5);
			static_assert(cmoon::floor(-1) == -1);
			static_assert(cmoon::floor(-1.2) == -2);
		}
};

class cot_test : public cmoon::test::test_case
{
	public:
		cot_test()
			: cmoon::test::test_case{"cot_test"} {}

		void operator()() override
		{
			cmoon::test::assert_almost_equal(cmoon::cot(0.523599), 1.73205, 0.00001);
			cmoon::test::assert_almost_equal(cmoon::cot(1.5708), 0, 0.1);
		}
};

class sign_test : public cmoon::test::test_case
{
	public:
		sign_test()
			: cmoon::test::test_case{"sign_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::sign(0), 1);
			cmoon::test::assert_equal(cmoon::sign(1), 1);
			cmoon::test::assert_equal(cmoon::sign(-1), -1);

			static_assert(cmoon::sign(0) == 1);
			static_assert(cmoon::sign(1) == 1);
			static_assert(cmoon::sign(-1) == -1);

			cmoon::test::assert_true(cmoon::is_positive(0));
			cmoon::test::assert_true(cmoon::is_positive(1));
			cmoon::test::assert_false(cmoon::is_positive(-1));

			static_assert(cmoon::is_positive(0));
			static_assert(cmoon::is_positive(1));
			static_assert(!cmoon::is_positive(-1));

			cmoon::test::assert_false(cmoon::is_negative(0));
			cmoon::test::assert_false(cmoon::is_negative(1));
			cmoon::test::assert_true(cmoon::is_negative(-1));

			static_assert(!cmoon::is_negative(0));
			static_assert(!cmoon::is_negative(1));
			static_assert(cmoon::is_negative(-1));
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
	suite.add_test_case<static_pow_test>();
	suite.add_test_case<abs_test>();
	suite.add_test_case<ceil_test>();
	suite.add_test_case<floor_test>();
	suite.add_test_case<cot_test>();
	suite.add_test_case<sign_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}