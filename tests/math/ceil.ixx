export module cmoon.tests.math.ceil;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class ceil_test : public cmoon::test::test_case
	{
		public:
			ceil_test()
				: cmoon::test::test_case{"ceil_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::ceil(5), 5);
				cmoon::test::assert_equal(cmoon::ceil(5.6), 6);
				cmoon::test::assert_equal(cmoon::ceil(-1), -1);
				cmoon::test::assert_equal(cmoon::ceil(-1.2), -1);

				static_assert(cmoon::ceil(5) == 5);
				static_assert(cmoon::ceil(5.6) == 6);
				static_assert(cmoon::ceil(-1) == -1);
				static_assert(cmoon::ceil(-1.2) == -1);
			}
	};
}