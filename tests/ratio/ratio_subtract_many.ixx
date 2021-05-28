export module cmoon.tests.ratio.ratio_subtract_many;

import <ratio>;

import cmoon.test;
import cmoon.ratio;

namespace cmoon::tests::ratio
{
	export
	class ratio_subtract_many_single_test : public cmoon::test::test_case
	{
		public:
			ratio_subtract_many_single_test()
				: cmoon::test::test_case{"ratio_subtract_many_single_test"} {}

			void operator()() override
			{
				using expected = std::ratio<15, 44>;
				using actual = cmoon::ratio_subtract_many<std::ratio<15, 44>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};

	export
	class ratio_subtract_many_double_test : public cmoon::test::test_case
	{
		public:
			ratio_subtract_many_double_test()
				: cmoon::test::test_case{"ratio_subtract_many_double_test"} {}

			void operator()() override
			{
				using expected = std::ratio<15, 44>;
				using actual = cmoon::ratio_subtract_many<std::ratio<30, 44>,
														  std::ratio<15, 44>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};

	export
	class ratio_subtract_many_test : public cmoon::test::test_case
	{
		public:
			ratio_subtract_many_test()
				: cmoon::test::test_case{"ratio_subtract_many_test"} {}

			void operator()() override
			{
				using expected = std::ratio<15, 44>;
				using actual = cmoon::ratio_subtract_many<std::ratio<100, 44>,
														  std::ratio<15, 44>,
														  std::ratio<30, 44>,
														  std::ratio<5, 44>,
														  std::ratio<35, 44>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};
}