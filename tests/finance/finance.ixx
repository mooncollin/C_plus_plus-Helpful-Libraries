export module cmoon.tests.finance;
export import cmoon.tests.finance.currency;
export import cmoon.tests.finance.decimal;
export import cmoon.tests.finance.securities;
export import cmoon.tests.finance.interest;
export import cmoon.tests.finance.money;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::finance>()
	{
		cmoon::test::test_suite suite{"finance library tests"};
		suite.add_test_case<finance::currency_code_constructor_test>();
		suite.add_test_case<finance::currency_constructor_test>();
		suite.add_test_case<finance::decimal_zero_constructor_test>();
		suite.add_test_case<finance::decimal_integer_constructor_test>();
		suite.add_test_case<finance::decimal_float_constructor_test>();
		suite.add_test_case<finance::decimal_add_test>();
		suite.add_test_case<finance::decimal_minus_test>();
		suite.add_test_case<finance::decimal_multiply_test>();
		suite.add_test_case<finance::decimal_divide_test>();
		suite.add_test_case<finance::decimal_change_precision_test>();
		suite.add_test_case<finance::decimal_abs_test>();
		suite.add_test_case<finance::decimal_pow_test>();
		suite.add_test_case<finance::dividend_yield_test>();
		suite.add_test_case<finance::simple_interest_test>();
		suite.add_test_case<finance::compound_interest_test>();
		suite.add_test_case<finance::money_constructor_test>();

		return std::move(suite);
	}
}