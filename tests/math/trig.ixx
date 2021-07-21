export module cmoon.tests.math.trig;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class cot_test : public cmoon::test::test_case
	{
		public:
			cot_test()
				: cmoon::test::test_case{"cot_test"} {}

			void operator()() override
			{
				cmoon::test::assert_almost_equal(cmoon::cot(0.523599), 1.73205, 0.00001);
				cmoon::test::assert_almost_equal(cmoon::cot(1.5708), 0, 0.1);
			}
	};
}