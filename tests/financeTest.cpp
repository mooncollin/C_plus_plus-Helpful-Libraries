#include <iostream>

#include "cmoon/test/test_case.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/finance/formulas.hpp"
#include "cmoon/finance/time.hpp"
#include "cmoon/finance/decimal.hpp"
#include "cmoon/measures/time.hpp"
#include "cmoon/finance/money.hpp"

class decimal_zero_constructor_test : public cmoon::test::test_case
{
	public:
		decimal_zero_constructor_test()
			: cmoon::test::test_case{"decimal_zero_constructor_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<0> zero;
			constexpr cmoon::finance::decimal<6> zero2;

			cmoon::test::assert_almost_equal(static_cast<double>(zero), 0.0, 0);
			static_assert(static_cast<double>(zero) == 0.0);

			cmoon::test::assert_almost_equal(static_cast<double>(zero2), 0.0, 0.000001);
			static_assert(static_cast<double>(zero2) == 0.0);
		}

	private:
		void static_information()
		{
			constexpr auto size = sizeof(cmoon::finance::decimal<6>);
		}
};

class decimal_integer_constructor_test : public cmoon::test::test_case
{
	public:
		decimal_integer_constructor_test()
			: cmoon::test::test_case{"decimal_integer_constructor_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<0> value1{50};
			constexpr cmoon::finance::decimal<6> value2{75};

			cmoon::test::assert_almost_equal(static_cast<double>(value1), 50.0, 0);
			static_assert(static_cast<double>(value1) == 50.0);

			cmoon::test::assert_almost_equal(static_cast<double>(value2), 75.0, 0.000001);
			static_assert(static_cast<double>(value2) == 75.0);
		}
};

class decimal_float_constructor_test : public cmoon::test::test_case
{
	public:
		decimal_float_constructor_test()
			: cmoon::test::test_case{"decimal_float_constructor_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<0> value1{0.02};
			constexpr cmoon::finance::decimal<2> value2{0.02};
			constexpr cmoon::finance::decimal<6> value3{53.12223452525};

			constexpr auto goal1 = 0.0;
			constexpr auto goal2 = 0.02;
			constexpr auto goal3 = 53.122235;

			cmoon::test::assert_almost_equal(static_cast<double>(value1), goal1, 0);
			static_assert(static_cast<double>(value1) == goal1);

			cmoon::test::assert_almost_equal(static_cast<double>(value2), goal2, 0.01);
			static_assert(static_cast<double>(value2) == goal2);

			cmoon::test::assert_almost_equal(static_cast<double>(value3), goal3, 0.000001);
			static_assert(static_cast<double>(value3) == goal3);
		}
};

class decimal_add_test : public cmoon::test::test_case
{
	public:
		decimal_add_test()
			: cmoon::test::test_case{"decimal_add_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<6> value1{40.444};
			constexpr cmoon::finance::decimal<6> value2{30.555};
			constexpr cmoon::finance::decimal<3> value3{30.555};
			constexpr cmoon::finance::decimal<6> goal1{70.999};
			constexpr cmoon::finance::decimal<3> goal2{70.999};

			cmoon::test::assert_equal(value1 + value2, goal1);
			static_assert(value1 + value2 == goal1);

			cmoon::test::assert_equal(value1 + value3, goal1);
			static_assert(value1 + value3 == goal1);

			cmoon::test::assert_equal(value1 + value3, static_cast<cmoon::finance::decimal<6>>(goal2));
			static_assert(value1 + value3 == static_cast<cmoon::finance::decimal<6>>(goal2));

			cmoon::test::assert_equal(static_cast<cmoon::finance::decimal<3>>(value1 + value3), goal2);
			static_assert(static_cast<cmoon::finance::decimal<3>>(value1 + value3) == goal2);
		}
};

class decimal_minus_test : public cmoon::test::test_case
{
	public:
		decimal_minus_test()
			: cmoon::test::test_case{"decimal_minus_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<6> value1{40.444};
			constexpr cmoon::finance::decimal<6> value2{30.555};
			constexpr cmoon::finance::decimal<3> value3{30.555};
			constexpr cmoon::finance::decimal<6> goal1{9.889};
			constexpr cmoon::finance::decimal<3> goal2{9.889};

			cmoon::test::assert_equal(value1 - value2, goal1);
			static_assert(value1 - value2 == goal1);

			cmoon::test::assert_equal(value1 - value3, goal1);
			static_assert(value1 - value3 == goal1);

			cmoon::test::assert_equal(value1 - value3, static_cast<cmoon::finance::decimal<6>>(goal2));
			static_assert(value1 - value3 == static_cast<cmoon::finance::decimal<6>>(goal2));

			cmoon::test::assert_equal(static_cast<cmoon::finance::decimal<3>>(value1 - value3), goal2);
			static_assert(static_cast<cmoon::finance::decimal<3>>(value1 - value3) == goal2);
		}
};

class decimal_multiply_test : public cmoon::test::test_case
{
	public:
		decimal_multiply_test()
			: cmoon::test::test_case{"decimal_multiply_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<6> value1{40.444};
			constexpr cmoon::finance::decimal<6> value2{30.555};
			constexpr cmoon::finance::decimal<3> value3{30.555};
			constexpr cmoon::finance::decimal<6> goal1{1235.76642};
			constexpr cmoon::finance::decimal<3> goal2{1235.76642};

			cmoon::test::assert_equal(value1 * value2, goal1);
			static_assert(value1 * value2 == goal1);

			cmoon::test::assert_equal(value1 * value3, goal1);
			static_assert(value1 * value3 == goal1);

			cmoon::test::assert_equal(static_cast<cmoon::finance::decimal<3>>(value1 * value3), goal2);
			static_assert(static_cast<cmoon::finance::decimal<3>>(value1 * value3) == goal2);
		}
};

class decimal_divide_test : public cmoon::test::test_case
{
	public:
		decimal_divide_test()
			: cmoon::test::test_case{ "decimal_divide_test" } {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<6> value1{40.444};
			constexpr cmoon::finance::decimal<6> value2{30.555};
			constexpr cmoon::finance::decimal<3> value3{30.555};
			constexpr cmoon::finance::decimal<6> goal1{1.32364588};
			constexpr cmoon::finance::decimal<3> goal2{1.32364588};

			cmoon::test::assert_equal(value1 / value2, goal1);
			static_assert(value1 / value2 == goal1);

			cmoon::test::assert_equal(value1 / value3, goal1);
			static_assert(value1 / value3 == goal1);

			cmoon::test::assert_equal(static_cast<cmoon::finance::decimal<3>>(value1 / value3), goal2);
			static_assert(static_cast<cmoon::finance::decimal<3>>(value1 / value3) == goal2);
		}
};

class decimal_change_precision_test : public cmoon::test::test_case
{
	public:
		decimal_change_precision_test()
			: cmoon::test::test_case{"decimal_change_precision_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<6> value1{3.141592};
			constexpr cmoon::finance::decimal<9> big_goal{3.141592000};
			constexpr cmoon::finance::decimal<3> small_goal{3.142};

			cmoon::test::assert_equal(static_cast<cmoon::finance::decimal<9>>(value1), big_goal);
			static_assert(static_cast<cmoon::finance::decimal<9>>(value1) == big_goal);

			cmoon::test::assert_equal(static_cast<cmoon::finance::decimal<3>>(value1), small_goal);
			static_assert(static_cast<cmoon::finance::decimal<3>>(value1) == small_goal);
		}
};

class decimal_abs_test : public cmoon::test::test_case
{
	public:
		decimal_abs_test()
			: cmoon::test::test_case{"decimal_abs_test"} {}

		void operator()() override
		{
			constexpr cmoon::finance::decimal<6> value1{3.141592};
			constexpr cmoon::finance::decimal<6> value2{-3.141592};
			constexpr cmoon::finance::decimal<6> goal{3.141592};

			cmoon::test::assert_equal(cmoon::finance::abs(value1), goal);
			static_assert(cmoon::finance::abs(value1) == goal);

			cmoon::test::assert_equal(cmoon::finance::abs(value2), goal);
			static_assert(cmoon::finance::abs(value2) == goal);
		}
};

class decimal_pow_test : public cmoon::test::test_case
{
	public:
		decimal_pow_test()
			: cmoon::test::test_case{"decimal_pow_test"} {}

		void operator()() override
		{
			constexpr auto value1 = cmoon::finance::decimal<6>{5};
			constexpr auto exponent1 = 2;
			constexpr auto goal1 = cmoon::finance::decimal<6>{25};

			constexpr auto value2 = cmoon::finance::decimal<6>{5};
			constexpr auto exponent2 = 5;
			constexpr auto goal2 = cmoon::finance::decimal<6>{3125};

			constexpr auto value3 = cmoon::finance::decimal<6>{77};
			constexpr auto exponent3 = 0;
			constexpr auto goal3 = cmoon::finance::decimal<6>{1};

			constexpr auto value4 = cmoon::finance::decimal<6>{5};
			constexpr auto exponent4 = -2;
			constexpr auto goal4 = cmoon::finance::decimal<6>{0.04};

			constexpr auto value5 = cmoon::finance::decimal<6>{100};
			constexpr auto exponent5 = 1;
			constexpr auto goal5 = cmoon::finance::decimal<6>{100};

			cmoon::test::assert_equal(cmoon::finance::pow(value1, exponent1), goal1);
			static_assert(cmoon::finance::pow(value1, exponent1) == goal1);

			cmoon::test::assert_equal(cmoon::finance::pow(value2, exponent2), goal2);

			constexpr double d = cmoon::finance::pow(value2, exponent2);
			static_assert(cmoon::finance::pow(value2, exponent2) == goal2);

			cmoon::test::assert_equal(cmoon::finance::pow(value3, exponent3), goal3);
			static_assert(cmoon::finance::pow(value3, exponent3) == goal3);

			cmoon::test::assert_equal(cmoon::finance::pow(value4, exponent4), goal4);
			static_assert(cmoon::finance::pow(value4, exponent4) == goal4);

			cmoon::test::assert_equal(cmoon::finance::pow(value5, exponent5), goal5);
			static_assert(cmoon::finance::pow(value5, exponent5) == goal5);
		}
};

class dividend_yield_test : public cmoon::test::test_case
{
	public:
		dividend_yield_test()
			: cmoon::test::test_case{"dividend_yield_test"} {}

		void operator()() override
		{
			constexpr auto stockprice1 = cmoon::finance::decimal<6>{100.0};
			constexpr auto annual_div1 = cmoon::finance::decimal<6>{5.0};
			constexpr auto goal1 = cmoon::finance::decimal<6>{0.05};

			cmoon::test::assert_equal(cmoon::finance::dividend_yield(stockprice1, annual_div1), goal1);
			static_assert(cmoon::finance::dividend_yield(stockprice1, annual_div1) == goal1);
		}
};

class simple_interest_test : public cmoon::test::test_case
{
	public:
		simple_interest_test()
			: cmoon::test::test_case{"simple_interest_test"} {}

		void operator()() override
		{
			constexpr auto principal1 = cmoon::finance::decimal<6>{350.0};
			constexpr auto rate1 = cmoon::finance::decimal<6>{0.025};
			constexpr auto years1 = cmoon::finance::years365{3};
			constexpr auto goal1 = cmoon::finance::decimal<6>{26.25};

			constexpr auto principal2 = cmoon::finance::decimal<6>{50000.25};
			constexpr auto rate2 = cmoon::finance::decimal<6>{0.09};
			constexpr auto months1 = cmoon::measures::months{48};
			constexpr auto goal2 = cmoon::finance::decimal<6>{18000.09};

			constexpr auto principal3 = cmoon::finance::decimal<6>{50.77};
			constexpr auto rate3 = cmoon::finance::decimal<6>{0.09};
			constexpr auto days1 = cmoon::measures::basic_days<double>{90};
			constexpr auto goal3 = cmoon::finance::decimal<6>{1.126675};

			constexpr auto principal4 = cmoon::finance::decimal<6>{100};
			constexpr auto rate4 = cmoon::finance::decimal<6>{.06};
			constexpr auto years2 = cmoon::finance::years365{2};
			constexpr auto goal4 = cmoon::finance::decimal<6>{12};

			cmoon::test::assert_equal(cmoon::finance::simple_interest(principal1, rate1, years1), goal1);
			static_assert(cmoon::finance::simple_interest(principal1, rate1, years1) == goal1);

			cmoon::test::assert_equal(cmoon::finance::simple_interest(principal2, rate2, months1), goal2);
			static_assert(cmoon::finance::simple_interest(principal2, rate2, months1) == goal2);

			cmoon::test::assert_equal(cmoon::finance::simple_interest(principal3, rate3, days1), goal3);
			static_assert(cmoon::finance::simple_interest(principal3, rate3, days1) == goal3);

			cmoon::test::assert_equal(cmoon::finance::simple_interest(principal4, rate4, years2), goal4);
			constexpr auto t = cmoon::finance::simple_interest(principal4, rate4, years2);
			static_assert(cmoon::finance::simple_interest(principal4, rate4, years2) == goal4);
		}
};

class compound_interest_test : public cmoon::test::test_case
{
	public:
		compound_interest_test()
			: cmoon::test::test_case{"compound_interest_test"} {}

		void operator()() override
		{
			constexpr auto principal1 = cmoon::finance::decimal<6>{4000};
			constexpr auto rate1 = cmoon::finance::decimal<6>{.07};
			constexpr auto years1 = cmoon::measures::years{5};
			constexpr auto days1 = cmoon::measures::days{1825};
			constexpr auto num_compounds1 = cmoon::finance::compound_yearly<6>;
			constexpr auto goal1 = cmoon::finance::decimal<6>{1610.208};

			constexpr auto principal2 = cmoon::finance::decimal<6>{1000};
			constexpr auto rate2 = cmoon::finance::decimal<6>{.09};
			constexpr auto months1 = cmoon::measures::months{18};
			constexpr auto num_compounds2 = cmoon::finance::compound_monthly<6>;
			constexpr auto goal2 = cmoon::finance::decimal<6>{143.960};

			constexpr auto principal3 = cmoon::finance::decimal<6>{1000};
			constexpr auto rate3 = cmoon::finance::decimal<6>{.05};
			constexpr auto years2 = cmoon::measures::years{5};
			constexpr auto goal3 = cmoon::finance::decimal<6>{284.025};

			cmoon::test::assert_equal(cmoon::finance::compound_interest(principal1, rate1, num_compounds1, years1), goal1);
			cmoon::test::assert_equal(cmoon::finance::compound_interest(principal2, rate2, num_compounds2, months1), goal2);
			cmoon::test::assert_equal(cmoon::finance::compound_interest(principal1, rate1, num_compounds1, days1), goal1);

			cmoon::test::assert_equal(cmoon::finance::compound_interest_continously(principal3, rate3, years2), goal3);
		}
};

class currency_code_constructor_test : public cmoon::test::test_case
{
	public:
		currency_code_constructor_test()
			: cmoon::test::test_case{"currency_code_test"} {}

		void operator()() override
		{

		}

	private:
		constexpr void static_information()
		{
			constexpr auto size = sizeof(cmoon::finance::currency_code);
		}
};

class currency_constructor_test : public cmoon::test::test_case
{
	public:
		currency_constructor_test()
			: cmoon::test::test_case{"currency_test"} {}

		void operator()() override
		{
			// Making sure I can access members
			static_assert(cmoon::finance::currencies::albanian_lek.currency_code().code() == "ALL");
			static_assert(cmoon::finance::currencies::albanian_lek.currency_number() == 8);
		}

	private:
		constexpr void static_information()
		{
			constexpr auto size = sizeof(cmoon::finance::currency);
		}
};

class money_constructor_test : public cmoon::test::test_case
{
	public:
		money_constructor_test()
			: cmoon::test::test_case{"money_constructor_test"} {}

		void operator()() override
		{
			
		}

	private:
		constexpr void static_information()
		{
			constexpr auto size = sizeof(cmoon::finance::money<6>);
		}
};

int main()
{
	cmoon::test::test_suite suite;

	suite.add_test_case<decimal_zero_constructor_test>();
	suite.add_test_case<decimal_integer_constructor_test>();
	suite.add_test_case<decimal_add_test>();
	suite.add_test_case<decimal_minus_test>();
	suite.add_test_case<decimal_multiply_test>();
	suite.add_test_case<decimal_divide_test>();
	suite.add_test_case<decimal_change_precision_test>();
	suite.add_test_case<decimal_abs_test>();
	suite.add_test_case<dividend_yield_test>();
	suite.add_test_case<simple_interest_test>();
	suite.add_test_case<compound_interest_test>();
	suite.add_test_case<currency_code_constructor_test>();
	suite.add_test_case<currency_constructor_test>();
	suite.add_test_case<money_constructor_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}