export module cmoon.tests.math.factorial;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class factorial_test : public cmoon::test::test_case
	{
		public:
			factorial_test()
				: cmoon::test::test_case{"factorial_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::factorial(1), 1);
				cmoon::test::assert_equal(cmoon::factorial(2), 2);
				cmoon::test::assert_equal(cmoon::factorial(3), 6);
				cmoon::test::assert_equal(cmoon::factorial(4), 24);
				cmoon::test::assert_equal(cmoon::factorial(5), 120);
				cmoon::test::assert_equal(cmoon::factorial(6), 720);
				cmoon::test::assert_equal(cmoon::factorial(7), 5040);
				cmoon::test::assert_equal(cmoon::factorial(8), 40320);
				cmoon::test::assert_equal(cmoon::factorial(9), 362880);
				cmoon::test::assert_equal(cmoon::factorial(10), 3628800);

				static_assert(cmoon::factorial(1) == 1);
				static_assert(cmoon::factorial(2) == 2);
				static_assert(cmoon::factorial(3) == 6);
				static_assert(cmoon::factorial(4) == 24);
				static_assert(cmoon::factorial(5) == 120);
				static_assert(cmoon::factorial(6) == 720);
				static_assert(cmoon::factorial(7) == 5040);
				static_assert(cmoon::factorial(8) == 40320);
				static_assert(cmoon::factorial(9) == 362880);
				static_assert(cmoon::factorial(10) == 3628800);

			}
	};
}