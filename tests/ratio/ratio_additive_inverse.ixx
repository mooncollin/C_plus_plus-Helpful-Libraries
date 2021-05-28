export module cmoon.tests.ratio.ratio_additive_inverse;

import <ratio>;

import cmoon.test;
import cmoon.ratio;

namespace cmoon::tests::ratio
{
	export
	class ratio_additive_inverse_test : public cmoon::test::test_case
	{
		public:
			ratio_additive_inverse_test()
				: cmoon::test::test_case{"ratio_additive_inverse_test"} {}

			void operator()() override
			{
				using expected = std::ratio<-15, 44>;

				using actual = cmoon::ratio_additive_inverse<std::ratio<15, 44>>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};
}