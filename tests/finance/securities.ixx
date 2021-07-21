export module cmoon.tests.finance.securities;

import cmoon.test;
import cmoon.finance;

namespace cmoon::tests::finance
{
	export
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
}