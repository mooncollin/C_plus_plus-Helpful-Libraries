export module cmoon.tests.ratio.ratio_divide_many;

import <ratio>;

import cmoon.test;
import cmoon.ratio;

namespace cmoon::tests::ratio
{
	export
	class ratio_divide_many_single_test : public cmoon::test::test_case
	{
		public:
			ratio_divide_many_single_test()
				: cmoon::test::test_case{"ratio_divide_many_single_test"} {}

			void operator()() override
			{
				using expected = std::ratio<15, 44>;
				using actual = cmoon::ratio_divide_many<std::ratio<15, 44>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};

	export
	class ratio_divide_many_double_test : public cmoon::test::test_case
	{
		public:
			ratio_divide_many_double_test()
				: cmoon::test::test_case{"ratio_divide_many_double_test"} {}

			void operator()() override
			{
				using expected = std::ratio<15, 44>;
				using actual = cmoon::ratio_divide_many<std::ratio<30, 44>,
														std::ratio<2, 1>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};

	export
	class ratio_divide_many_test : public cmoon::test::test_case
	{
		public:
			ratio_divide_many_test()
				: cmoon::test::test_case{"ratio_divide_many_test"} {}

			void operator()() override
			{
				using expected = std::ratio<25, 11>;
				using actual = cmoon::ratio_divide_many<std::ratio<150, 44>,
														std::ratio<2, 1>,
														std::ratio<3, 4>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};
}