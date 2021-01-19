#include <iostream>

#include "cmoon/datetime.hpp"
#include "cmoon/measures/time.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"
#include "cmoon/test/runner.hpp"

class is_am_test : public cmoon::test::test_case
{
	public:
		is_am_test()
			: cmoon::test::test_case{"is_am_test"} {}

		void operator()() override
		{
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{0}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{1}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{2}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{3}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{4}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{5}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{6}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{7}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{8}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{9}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{10}));
			cmoon::test::assert_true(cmoon::datetime::is_am(cmoon::measures::hours{11}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{12}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{13}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{14}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{15}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{16}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{17}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{18}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{19}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{20}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{21}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{22}));
			cmoon::test::assert_false(cmoon::datetime::is_am(cmoon::measures::hours{23}));

			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{0}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{1}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{2}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{3}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{4}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{5}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{6}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{7}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{8}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{9}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{10}));
			static_assert(cmoon::datetime::is_am(cmoon::measures::hours{11}));

			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{12}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{13}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{14}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{15}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{16}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{17}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{18}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{19}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{20}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{21}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{22}));
			static_assert(!cmoon::datetime::is_am(cmoon::measures::hours{23}));
		}
};

class is_pm_test : public cmoon::test::test_case
{
	public:
		is_pm_test()
			: cmoon::test::test_case{"is_pm_test"} {}

		void operator()() override
		{
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{0}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{1}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{2}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{3}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{4}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{5}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{6}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{7}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{8}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{9}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{10}));
			cmoon::test::assert_false(cmoon::datetime::is_pm(cmoon::measures::hours{11}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{12}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{13}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{14}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{15}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{16}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{17}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{18}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{19}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{20}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{21}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{22}));
			cmoon::test::assert_true(cmoon::datetime::is_pm(cmoon::measures::hours{23}));

			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{0}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{1}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{2}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{3}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{4}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{5}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{6}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{7}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{8}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{9}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{10}));
			static_assert(!cmoon::datetime::is_pm(cmoon::measures::hours{11}));

			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{12}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{13}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{14}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{15}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{16}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{17}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{18}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{19}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{20}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{21}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{22}));
			static_assert(cmoon::datetime::is_pm(cmoon::measures::hours{23}));
		}
};

class make12_test : public cmoon::test::test_case
{
	public:
		make12_test()
			: cmoon::test::test_case{"make12_test"} {}

		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{0}), cmoon::measures::hours{12});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{1}), cmoon::measures::hours{1});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{2}), cmoon::measures::hours{2});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{3}), cmoon::measures::hours{3});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{4}), cmoon::measures::hours{4});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{5}), cmoon::measures::hours{5});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{6}), cmoon::measures::hours{6});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{7}), cmoon::measures::hours{7});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{8}), cmoon::measures::hours{8});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{9}), cmoon::measures::hours{9});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{10}), cmoon::measures::hours{10});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{11}), cmoon::measures::hours{11});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{12}), cmoon::measures::hours{12});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{13}), cmoon::measures::hours{1});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{14}), cmoon::measures::hours{2});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{15}), cmoon::measures::hours{3});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{16}), cmoon::measures::hours{4});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{17}), cmoon::measures::hours{5});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{18}), cmoon::measures::hours{6});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{19}), cmoon::measures::hours{7});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{20}), cmoon::measures::hours{8});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{21}), cmoon::measures::hours{9});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{22}), cmoon::measures::hours{10});
			cmoon::test::assert_equal(cmoon::datetime::make12(cmoon::measures::hours{23}), cmoon::measures::hours{11});

			static_assert(cmoon::datetime::make12(cmoon::measures::hours{0}) == cmoon::measures::hours{12});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{1}) == cmoon::measures::hours{1});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{2}) == cmoon::measures::hours{2});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{3}) == cmoon::measures::hours{3});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{4}) == cmoon::measures::hours{4});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{5}) == cmoon::measures::hours{5});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{6}) == cmoon::measures::hours{6});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{7}) == cmoon::measures::hours{7});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{8}) == cmoon::measures::hours{8});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{9}) == cmoon::measures::hours{9});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{10}) == cmoon::measures::hours{10});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{11}) == cmoon::measures::hours{11});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{12}) == cmoon::measures::hours{12});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{13}) == cmoon::measures::hours{1});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{14}) == cmoon::measures::hours{2});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{15}) == cmoon::measures::hours{3});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{16}) == cmoon::measures::hours{4});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{17}) == cmoon::measures::hours{5});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{18}) == cmoon::measures::hours{6});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{19}) == cmoon::measures::hours{7});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{20}) == cmoon::measures::hours{8});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{21}) == cmoon::measures::hours{9});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{22}) == cmoon::measures::hours{10});
			static_assert(cmoon::datetime::make12(cmoon::measures::hours{23}) == cmoon::measures::hours{11});
		}
};

class make24_test : public cmoon::test::test_case
{
	public:
		make24_test()
			: cmoon::test::test_case{"make24_test"} {}
		
		void operator()() override
		{
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{1}, false), cmoon::measures::hours{1});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{2}, false), cmoon::measures::hours{2});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{3}, false), cmoon::measures::hours{3});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{4}, false), cmoon::measures::hours{4});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{5}, false), cmoon::measures::hours{5});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{6}, false), cmoon::measures::hours{6});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{7}, false), cmoon::measures::hours{7});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{8}, false), cmoon::measures::hours{8});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{9}, false), cmoon::measures::hours{9});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{10}, false), cmoon::measures::hours{10});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{11}, false), cmoon::measures::hours{11});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{12}, false), cmoon::measures::hours{0});

			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{1}, true), cmoon::measures::hours{13});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{2}, true), cmoon::measures::hours{14});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{3}, true), cmoon::measures::hours{15});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{4}, true), cmoon::measures::hours{16});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{5}, true), cmoon::measures::hours{17});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{6}, true), cmoon::measures::hours{18});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{7}, true), cmoon::measures::hours{19});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{8}, true), cmoon::measures::hours{20});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{9}, true), cmoon::measures::hours{21});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{10}, true), cmoon::measures::hours{22});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{11}, true), cmoon::measures::hours{23});
			cmoon::test::assert_equal(cmoon::datetime::make24(cmoon::measures::hours{12}, true), cmoon::measures::hours{12});

			static_assert(cmoon::datetime::make24(cmoon::measures::hours{1}, false) == cmoon::measures::hours{1});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{2}, false) == cmoon::measures::hours{2});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{3}, false) == cmoon::measures::hours{3});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{4}, false) == cmoon::measures::hours{4});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{5}, false) == cmoon::measures::hours{5});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{6}, false) == cmoon::measures::hours{6});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{7}, false) == cmoon::measures::hours{7});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{8}, false) == cmoon::measures::hours{8});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{9}, false) == cmoon::measures::hours{9});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{10}, false) == cmoon::measures::hours{10});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{11}, false) == cmoon::measures::hours{11});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{12}, false) == cmoon::measures::hours{0});

			static_assert(cmoon::datetime::make24(cmoon::measures::hours{1}, true) == cmoon::measures::hours{13});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{2}, true) == cmoon::measures::hours{14});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{3}, true) == cmoon::measures::hours{15});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{4}, true) == cmoon::measures::hours{16});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{5}, true) == cmoon::measures::hours{17});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{6}, true) == cmoon::measures::hours{18});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{7}, true) == cmoon::measures::hours{19});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{8}, true) == cmoon::measures::hours{20});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{9}, true) == cmoon::measures::hours{21});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{10}, true) == cmoon::measures::hours{22});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{11}, true) == cmoon::measures::hours{23});
			static_assert(cmoon::datetime::make24(cmoon::measures::hours{12}, true) == cmoon::measures::hours{12});
		}
};

class hh_mm_ss_hours_test : public cmoon::test::test_case
{
	public:
		hh_mm_ss_hours_test()
			: cmoon::test::test_case{"hh_mm_ss_hours_test"} {}
		
		void operator()() override
		{
			constexpr cmoon::measures::hours hours_expected{5};
			constexpr cmoon::datetime::hh_mm_ss<cmoon::measures::hours> time {hours_expected};

			cmoon::test::assert_equal(time.hours(), hours_expected);
			cmoon::test::assert_equal(time.to_duration(), hours_expected);
		}
};

class hh_mm_ss_minutes_test : public cmoon::test::test_case
{
	public:
		hh_mm_ss_minutes_test()
			: cmoon::test::test_case{"hh_mm_ss_minutes_test"} {}

		void operator()() override
		{
			constexpr cmoon::measures::hours hours_expected {5};
			constexpr cmoon::measures::minutes minutes_expected {15};
			constexpr cmoon::measures::minutes overall_expected {cmoon::measures::unit_cast<cmoon::measures::minutes>(hours_expected) + minutes_expected};

			constexpr cmoon::datetime::hh_mm_ss<cmoon::measures::minutes> time{overall_expected};

			cmoon::test::assert_equal(time.hours(), hours_expected);
			cmoon::test::assert_equal(time.minutes(), minutes_expected);
			cmoon::test::assert_equal(time.to_duration(), overall_expected);
		}
};

class hh_mm_ss_seconds_test : public cmoon::test::test_case
{
	public:
		hh_mm_ss_seconds_test()
			: cmoon::test::test_case{"hh_mm_ss_seconds_test"} {}

		void operator()() override
		{
			constexpr cmoon::measures::hours hours_expected{5};
			constexpr cmoon::measures::minutes minutes_expected{15};
			constexpr cmoon::measures::seconds seconds_expected{26};

			constexpr cmoon::measures::seconds overall_expected{
				cmoon::measures::unit_cast<cmoon::measures::seconds>(hours_expected) +
				cmoon::measures::unit_cast<cmoon::measures::seconds>(minutes_expected) +
				seconds_expected};

			constexpr cmoon::datetime::hh_mm_ss<cmoon::measures::seconds> time{overall_expected};

			cmoon::test::assert_equal(time.hours(), hours_expected);
			cmoon::test::assert_equal(time.minutes(), minutes_expected);
			cmoon::test::assert_equal(time.seconds(), seconds_expected);

			cmoon::test::assert_equal(time.to_duration(), overall_expected);
		}
};

class month_test : public cmoon::test::test_case
{
	public:
		month_test()
			: cmoon::test::test_case{"month_test"} {}

		void operator()() override
		{
			cmoon::test::assert_true(cmoon::datetime::month{1}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{2}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{3}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{4}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{5}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{6}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{7}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{8}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{9}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{10}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{11}.ok());
			cmoon::test::assert_true(cmoon::datetime::month{12}.ok());
			cmoon::test::assert_false(cmoon::datetime::month{13}.ok());
			cmoon::test::assert_false(cmoon::datetime::month{0}.ok());

			cmoon::test::assert_equal(cmoon::datetime::month{1}, cmoon::datetime::month{1});
			cmoon::test::assert_less(cmoon::datetime::month{1}, cmoon::datetime::month{2});
			cmoon::test::assert_less_equal(cmoon::datetime::month{1}, cmoon::datetime::month{5});
			cmoon::test::assert_greater(cmoon::datetime::month{3}, cmoon::datetime::month{1});
			cmoon::test::assert_greater_equal(cmoon::datetime::month{1}, cmoon::datetime::month{1});
			cmoon::test::assert_not_equal(cmoon::datetime::month{1}, cmoon::datetime::month{2});

			cmoon::test::assert_equal(cmoon::datetime::month{1} + cmoon::measures::months{1}, cmoon::datetime::month{2});
			cmoon::test::assert_equal(cmoon::measures::months{3} + cmoon::datetime::month{10}, cmoon::datetime::month{1});
			cmoon::test::assert_equal(cmoon::datetime::july - cmoon::measures::months{4}, cmoon::datetime::march);
			cmoon::test::assert_equal(cmoon::datetime::july - cmoon::datetime::october, -cmoon::measures::months{3});
			cmoon::test::assert_equal(cmoon::datetime::july - cmoon::datetime::january, cmoon::measures::months{6});
		}
};

class year_test : public cmoon::test::test_case
{
	public:
		year_test()
			: cmoon::test::test_case{"year_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::year y {2020};
			cmoon::test::assert_true(y.is_leap());
			cmoon::test::assert_false(cmoon::datetime::year{2019}.is_leap());
			cmoon::test::assert_equal(y + cmoon::measures::years{44}, cmoon::datetime::year{2064});
			cmoon::test::assert_equal(y - cmoon::measures::years{44}, cmoon::datetime::year{1976});
			cmoon::test::assert_equal(y - cmoon::measures::years{2044}, cmoon::datetime::year{-24});
			cmoon::test::assert_equal(y - cmoon::datetime::year{2}, cmoon::measures::years{2018});
			cmoon::test::assert_equal(cmoon::datetime::year{50} - y, cmoon::measures::years{-1970});
		}
};

class days_in_month_test : public cmoon::test::test_case
{
	public:
		days_in_month_test()
			: cmoon::test::test_case{"days_in_month_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::year y1 {2020};
			constexpr cmoon::datetime::year y2 {2019};
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::january, y1), cmoon::measures::days{31});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::march, y1), cmoon::measures::days{31});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::may, y1), cmoon::measures::days{31});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::july, y1), cmoon::measures::days{31});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::august, y1), cmoon::measures::days{31});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::october, y1), cmoon::measures::days{31});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::december, y1), cmoon::measures::days{31});

			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::april, y1), cmoon::measures::days{30});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::june, y1), cmoon::measures::days{30});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::september, y1), cmoon::measures::days{30});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::november, y1), cmoon::measures::days{30});

			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::february, y1), cmoon::measures::days{29});
			cmoon::test::assert_equal(cmoon::datetime::days_in_month(cmoon::datetime::february, y2), cmoon::measures::days{28});
		}
};

class day_of_month_test : public cmoon::test::test_case
{
	public:
		day_of_month_test()
			: cmoon::test::test_case{"day_of_month_test"} {}

		void operator()() override
		{
			cmoon::test::assert_true(cmoon::datetime::day_of_month{1}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{2}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{3}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{4}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{5}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{6}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{7}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{8}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{9}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{10}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{11}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{12}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{13}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{14}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{15}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{16}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{17}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{18}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{19}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{20}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{21}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{22}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{23}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{24}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{25}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{26}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{27}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{28}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{29}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{30}.ok());
			cmoon::test::assert_true(cmoon::datetime::day_of_month{31}.ok());
			cmoon::test::assert_false(cmoon::datetime::day_of_month{32}.ok());
			cmoon::test::assert_false(cmoon::datetime::day_of_month{0}.ok());

			cmoon::test::assert_equal(cmoon::datetime::day_of_month{1}, cmoon::datetime::day_of_month{1});
			cmoon::test::assert_less(cmoon::datetime::day_of_month{1}, cmoon::datetime::day_of_month{2});
			cmoon::test::assert_less_equal(cmoon::datetime::day_of_month{1}, cmoon::datetime::day_of_month{5});
			cmoon::test::assert_greater(cmoon::datetime::day_of_month{3}, cmoon::datetime::day_of_month{1});
			cmoon::test::assert_greater_equal(cmoon::datetime::day_of_month{1}, cmoon::datetime::day_of_month{1});
			cmoon::test::assert_not_equal(cmoon::datetime::day_of_month{1}, cmoon::datetime::day_of_month{2});

			cmoon::test::assert_equal(cmoon::datetime::day_of_month{1} + cmoon::measures::days{1}, cmoon::datetime::day_of_month{2});
			cmoon::test::assert_equal(cmoon::measures::days{3} + cmoon::datetime::day_of_month{10}, cmoon::datetime::day_of_month{13});
			cmoon::test::assert_equal(cmoon::datetime::eleventh - cmoon::measures::days{4}, cmoon::datetime::seventh);

			cmoon::datetime::day_of_month test{5};
			++test;
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_month{6});
			--test;
			--test;
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_month{4});
			test += cmoon::measures::days{14};
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_month{18});
			test -= cmoon::measures::days{2};
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_month{16});
			cmoon::test::assert_equal(unsigned{test}, 16u);
		}
};

class day_of_year_test : public cmoon::test::test_case
{
	public:
		day_of_year_test()
			: cmoon::test::test_case{"day_of_year_test"} {}

		void operator()() override
		{
			for (auto i{1u}; i <= 365u; ++i)
			{
				cmoon::test::assert_true(cmoon::datetime::day_of_year{i}.ok());
			}

			cmoon::test::assert_false(cmoon::datetime::day_of_year{0}.ok());
			cmoon::test::assert_false(cmoon::datetime::day_of_year{366}.ok());

			cmoon::test::assert_equal(cmoon::datetime::day_of_year{1}, cmoon::datetime::day_of_year{1});
			cmoon::test::assert_less(cmoon::datetime::day_of_year{1}, cmoon::datetime::day_of_year{2});
			cmoon::test::assert_less_equal(cmoon::datetime::day_of_year{1}, cmoon::datetime::day_of_year{5});
			cmoon::test::assert_greater(cmoon::datetime::day_of_year{3}, cmoon::datetime::day_of_year{1});
			cmoon::test::assert_greater_equal(cmoon::datetime::day_of_year{1}, cmoon::datetime::day_of_year{1});
			cmoon::test::assert_not_equal(cmoon::datetime::day_of_year{1}, cmoon::datetime::day_of_year{2});

			cmoon::datetime::day_of_year test{5};
			++test;
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_year{6});
			--test;
			--test;
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_year{4});
			test += cmoon::measures::days{14};
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_year{18});
			test -= cmoon::measures::days{2};
			cmoon::test::assert_equal(test, cmoon::datetime::day_of_year{16});
			cmoon::test::assert_equal(unsigned{test}, 16u);
		}
};

class weekday_test : public cmoon::test::test_case
{
	public:
		weekday_test()
			: cmoon::test::test_case{"weekday_test"} {}

		void operator()() override
		{
			cmoon::test::assert_true(cmoon::datetime::weekday{0}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{1}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{2}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{3}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{4}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{5}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{6}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{7}.ok());
			cmoon::test::assert_true(cmoon::datetime::weekday{8}.ok());

			cmoon::test::assert_equal(cmoon::datetime::weekday{0}.c_encoding(), 0u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{1}.c_encoding(), 1u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{2}.c_encoding(), 2u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{3}.c_encoding(), 3u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{4}.c_encoding(), 4u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{5}.c_encoding(), 5u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{6}.c_encoding(), 6u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{7}.c_encoding(), 0u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{8}.c_encoding(), 1u);

			cmoon::test::assert_equal(cmoon::datetime::weekday{0}.iso_encoding(), 7u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{1}.iso_encoding(), 1u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{2}.iso_encoding(), 2u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{3}.iso_encoding(), 3u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{4}.iso_encoding(), 4u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{5}.iso_encoding(), 5u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{6}.iso_encoding(), 6u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{7}.iso_encoding(), 7u);
			cmoon::test::assert_equal(cmoon::datetime::weekday{8}.iso_encoding(), 1u);

			cmoon::datetime::weekday test{5};
			++test;
			cmoon::test::assert_equal(test, cmoon::datetime::weekday{6});
			--test;
			--test;
			cmoon::test::assert_equal(test, cmoon::datetime::weekday{4});
			test += cmoon::measures::days{14};
			cmoon::test::assert_equal(test, cmoon::datetime::weekday{4});
			test -= cmoon::measures::days{2};
			cmoon::test::assert_equal(test, cmoon::datetime::weekday{2});
			test -= cmoon::measures::days{5};
			cmoon::test::assert_equal(test, cmoon::datetime::weekday{4});

			cmoon::test::assert_equal(test[2], cmoon::datetime::weekday_indexed{cmoon::datetime::thursday, 2});
			cmoon::test::assert_equal(test[cmoon::datetime::last], cmoon::datetime::weekday_last{cmoon::datetime::thursday});
		}
};

class month_day_test : public cmoon::test::test_case
{
	public:
		month_day_test()
			: cmoon::test::test_case{"month_day_test"} {}

		void operator()() override
		{
			cmoon::test::assert_true(cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::fourth}.ok());
			cmoon::test::assert_false(cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::day_of_month{55}}.ok());
			cmoon::test::assert_less(cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::fourth},
									 cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::eleventh});

			cmoon::test::assert_less_equal(cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::fourth},
										   cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::eleventh});
			
			cmoon::test::assert_greater(cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::fourth},
										cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::first});

			cmoon::test::assert_greater_equal(cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::fourth},
											  cmoon::datetime::month_day{cmoon::datetime::march, cmoon::datetime::first});
		}
};

class month_day_last_test : public cmoon::test::test_case
{
	public:
		month_day_last_test()
			: cmoon::test::test_case{"month_day_last_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::month_day_last test {cmoon::datetime::february};
			cmoon::test::assert_equal(test.month(), cmoon::datetime::february);
			cmoon::test::assert_true(test.ok());
			cmoon::test::assert_less(test, cmoon::datetime::month_day_last{cmoon::datetime::december});
			cmoon::test::assert_greater(test, cmoon::datetime::month_day_last{cmoon::datetime::january});
		}
};

class month_weekday_test : public cmoon::test::test_case
{
	public:
		month_weekday_test()
			: cmoon::test::test_case{"month_weekday_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::month_weekday test {cmoon::datetime::april, cmoon::datetime::tuesday[3]};
			cmoon::test::assert_equal(test.month(), cmoon::datetime::april);
			cmoon::test::assert_equal(test.weekday_indexed(), cmoon::datetime::tuesday[3]);
			cmoon::test::assert_true(test.ok());
		}
};

class month_weekday_last_test : public cmoon::test::test_case
{
	public:
		month_weekday_last_test()
			: cmoon::test::test_case{"month_weekday_last_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::month_weekday_last test{cmoon::datetime::april, cmoon::datetime::tuesday[cmoon::datetime::last]};
			cmoon::test::assert_equal(test.month(), cmoon::datetime::april);
			cmoon::test::assert_equal(test.weekday_last(), cmoon::datetime::tuesday[cmoon::datetime::last]);
			cmoon::test::assert_true(test.ok());
		}
};

class year_month_test : public cmoon::test::test_case
{
	public:
		year_month_test()
			: cmoon::test::test_case{"year_month_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::year_month test {cmoon::datetime::year{2020}, cmoon::datetime::march};

			cmoon::test::assert_equal(test.year(), cmoon::datetime::year{2020});
			cmoon::test::assert_equal(test.month(), cmoon::datetime::march);
			cmoon::test::assert_true(test.ok());

			cmoon::datetime::year_month test2 {cmoon::datetime::year{2000}, cmoon::datetime::january};
			test2 += cmoon::measures::months{4};

			cmoon::test::assert_equal(test2.year(), cmoon::datetime::year{2000});
			cmoon::test::assert_equal(test2.month(), cmoon::datetime::may);

			test2 -= cmoon::measures::months{1};
			cmoon::test::assert_equal(test2.year(), cmoon::datetime::year{2000});
			cmoon::test::assert_equal(test2.month(), cmoon::datetime::april);

			test2 += cmoon::measures::years{2};
			cmoon::test::assert_equal(test2.year(), cmoon::datetime::year{2002});
			cmoon::test::assert_equal(test2.month(), cmoon::datetime::april);

			test2 += cmoon::measures::months{25};
			cmoon::test::assert_equal(test2.year(), cmoon::datetime::year{2004});
			cmoon::test::assert_equal(test2.month(), cmoon::datetime::may);

			test2 -= cmoon::measures::months{52};
			cmoon::test::assert_equal(test2.year(), cmoon::datetime::year{2000});
			cmoon::test::assert_equal(test2.month(), cmoon::datetime::january);

			cmoon::test::assert_equal(test - test2, cmoon::measures::months{242});
			cmoon::test::assert_equal(test2 - test, cmoon::measures::months{-242});
			cmoon::test::assert_less(test2, test);
			cmoon::test::assert_greater(test, test2);
		}
};

class year_month_day_test : public cmoon::test::test_case
{
	public:
		year_month_day_test()
			: cmoon::test::test_case{"year_month_day_test"} {}

		void operator()() override
		{
			constexpr cmoon::datetime::year_month_day test_default {};
			cmoon::test::assert_equal(test_default.year(), cmoon::datetime::year{0});
			cmoon::test::assert_equal(test_default.month(), cmoon::datetime::month{0});
			cmoon::test::assert_equal(test_default.day(), cmoon::datetime::day_of_month{0});

			const auto now = std::chrono::system_clock::now();
			const auto now2 = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
			const auto now_days = std::chrono::time_point_cast<cmoon::measures::chrono_t<cmoon::measures::days>>(now);

			const cmoon::datetime::year_month_day test_sys_days {now_days};
			cmoon::test::assert_true(test_sys_days.ok());

			constexpr cmoon::datetime::year_month_day test_days {cmoon::datetime::january/1/2020};
			cmoon::test::assert_true(test_days.ok());

			constexpr auto test_days2 = test_days + cmoon::measures::days{55};

			cmoon::test::assert_equal(test_days2,
				cmoon::datetime::year_month_day{cmoon::datetime::february/25/2020});
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<is_am_test>();
	suite.add_test_case<is_pm_test>();
	suite.add_test_case<make12_test>();
	suite.add_test_case<make24_test>();
	suite.add_test_case<hh_mm_ss_hours_test>();
	suite.add_test_case<hh_mm_ss_minutes_test>();
	suite.add_test_case<hh_mm_ss_seconds_test>();
	suite.add_test_case<month_test>();
	suite.add_test_case<year_test>();
	suite.add_test_case<days_in_month_test>();
	suite.add_test_case<day_of_month_test>();
	suite.add_test_case<day_of_year_test>();
	suite.add_test_case<weekday_test>();
	suite.add_test_case<month_day_test>();
	suite.add_test_case<month_day_last_test>();
	suite.add_test_case<month_weekday_test>();
	suite.add_test_case<month_weekday_last_test>();
	suite.add_test_case<year_month_test>();
	suite.add_test_case<year_month_day_test>();

	cmoon::datetime::get_tzdb();

	cmoon::test::text_test_runner runner {std::cout};

	return !runner.run(suite);
}