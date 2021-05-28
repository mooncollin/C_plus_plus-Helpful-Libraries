export module cmoon.tests.ratio.ratio_canonical;

import <ratio>;

import cmoon.test;
import cmoon.ratio;

namespace cmoon::tests::ratio
{
	export
	class ratio_canonical_test : public cmoon::test::test_case
	{
		public:
			ratio_canonical_test()
				: cmoon::test::test_case{"ratio_canonical_test"} {}

			void operator()() override
			{
				using expected1 = std::ratio<1, 2>;
				using expected2 = std::ratio<1, 1>;
				using expected3 = std::ratio<15, 44>;

				using actual1 = cmoon::ratio_canonical<std::ratio<2, 4>>;
				using actual2 = cmoon::ratio_canonical<std::ratio<13, 13>>;
				using actual3 = cmoon::ratio_canonical<std::ratio<15, 44>>;

				cmoon::test::assert_is_type<actual1, expected1>();
				cmoon::test::assert_is_type<actual2, expected2>();
				cmoon::test::assert_is_type<actual3, expected3>();
			}
	};
}