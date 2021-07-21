export module cmoon.tests.math.floor;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class floor_test : public cmoon::test::test_case
	{
		public:
			floor_test()
				: cmoon::test::test_case{"floor_test"} {}

			void operator()()
			{
				cmoon::test::assert_equal(cmoon::floor(5), 5);
				cmoon::test::assert_equal(cmoon::floor(5.6), 5);
				cmoon::test::assert_equal(cmoon::floor(-1), -1);
				cmoon::test::assert_equal(cmoon::floor(-1.2), -2);

				static_assert(cmoon::floor(5) == 5);
				static_assert(cmoon::floor(5.6) == 5);
				static_assert(cmoon::floor(-1) == -1);
				static_assert(cmoon::floor(-1.2) == -2);
			}
	};
}