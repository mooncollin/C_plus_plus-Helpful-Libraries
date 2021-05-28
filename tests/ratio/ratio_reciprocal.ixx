export module cmoon.tests.ratio.ratio_reciprocal;

import <ratio>;
import <concepts>;

import cmoon.test;
import cmoon.ratio;

namespace cmoon::tests::ratio
{
	export
	class ratio_reciprocal_test : public cmoon::test::test_case
	{
		public:
			ratio_reciprocal_test()
				: cmoon::test::test_case{"ratio_reciprocal_test"} {}

			void operator()() override
			{
				constexpr auto N = 5;
				constexpr auto D = 2;
				using expected = std::ratio<D, N>;
				using r = std::ratio<N, D>;

				using actual = cmoon::ratio_reciprocal<r>;

				cmoon::test::assert_is_type<actual, expected>();
			}
	};
}