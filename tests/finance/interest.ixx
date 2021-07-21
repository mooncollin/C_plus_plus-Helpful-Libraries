export module cmoon.tests.finance.interest;

import <chrono>;

import cmoon.test;
import cmoon.finance;

namespace cmoon::tests::finance
{
	export
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
				constexpr auto months1 = std::chrono::months{48};
				constexpr auto goal2 = cmoon::finance::decimal<6>{18000.09};

				constexpr auto principal4 = cmoon::finance::decimal<6>{100};
				constexpr auto rate4 = cmoon::finance::decimal<6>{.06};
				constexpr auto years2 = cmoon::finance::years365{2};
				constexpr auto goal4 = cmoon::finance::decimal<6>{12};

				cmoon::test::assert_equal(cmoon::finance::simple_interest(principal1, rate1, years1), goal1);
				static_assert(cmoon::finance::simple_interest(principal1, rate1, years1) == goal1);

				cmoon::test::assert_equal(cmoon::finance::simple_interest(principal2, rate2, months1), goal2);
				static_assert(cmoon::finance::simple_interest(principal2, rate2, months1) == goal2);

				cmoon::test::assert_equal(cmoon::finance::simple_interest(principal4, rate4, years2), goal4);
				constexpr auto t = cmoon::finance::simple_interest(principal4, rate4, years2);
				static_assert(cmoon::finance::simple_interest(principal4, rate4, years2) == goal4);
			}
	};

	export
	class compound_interest_test : public cmoon::test::test_case
	{
		public:
			compound_interest_test()
				: cmoon::test::test_case{"compound_interest_test"} {}

			void operator()() override
			{
				constexpr auto principal1 = cmoon::finance::decimal<6>{4000};
				constexpr auto rate1 = cmoon::finance::decimal<6>{.07};
				constexpr auto years1 = std::chrono::years{5};
				constexpr auto days1 = std::chrono::days{1825};
				constexpr auto num_compounds1 = cmoon::finance::compound_yearly<6>;
				constexpr auto goal1 = cmoon::finance::decimal<6>{1610.208};

				constexpr auto principal2 = cmoon::finance::decimal<6>{1000};
				constexpr auto rate2 = cmoon::finance::decimal<6>{.09};
				constexpr auto months1 = std::chrono::months{18};
				constexpr auto num_compounds2 = cmoon::finance::compound_monthly<6>;
				constexpr auto goal2 = cmoon::finance::decimal<6>{143.960};

				constexpr auto principal3 = cmoon::finance::decimal<6>{1000};
				constexpr auto rate3 = cmoon::finance::decimal<6>{.05};
				constexpr auto years2 = std::chrono::years{5};
				constexpr auto goal3 = cmoon::finance::decimal<6>{284.025};

				cmoon::test::assert_equal(cmoon::finance::compound_interest(principal1, rate1, num_compounds1, years1), goal1);
				cmoon::test::assert_equal(cmoon::finance::compound_interest(principal2, rate2, num_compounds2, months1), goal2);
				cmoon::test::assert_equal(cmoon::finance::compound_interest(principal1, rate1, num_compounds1, days1), goal1);

				cmoon::test::assert_equal(cmoon::finance::compound_interest_continously(principal3, rate3, years2), goal3);
			}
	};
}