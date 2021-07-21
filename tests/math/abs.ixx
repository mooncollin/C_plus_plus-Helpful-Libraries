export module cmoon.tests.math.abs;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class abs_test : public cmoon::test::test_case
	{
		public:
			abs_test()
				: cmoon::test::test_case{"abs_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::abs(1), 1);
				cmoon::test::assert_equal(cmoon::abs(2), 2);
				cmoon::test::assert_equal(cmoon::abs(3), 3);
				cmoon::test::assert_equal(cmoon::abs(4), 4);
				cmoon::test::assert_equal(cmoon::abs(5), 5);
				cmoon::test::assert_equal(cmoon::abs(6), 6);
				cmoon::test::assert_equal(cmoon::abs(7), 7);
				cmoon::test::assert_equal(cmoon::abs(8), 8);
				cmoon::test::assert_equal(cmoon::abs(9), 9);
				cmoon::test::assert_equal(cmoon::abs(10), 10);

				cmoon::test::assert_equal(cmoon::abs(-1), 1);
				cmoon::test::assert_equal(cmoon::abs(-2), 2);
				cmoon::test::assert_equal(cmoon::abs(-3), 3);
				cmoon::test::assert_equal(cmoon::abs(-4), 4);
				cmoon::test::assert_equal(cmoon::abs(-5), 5);
				cmoon::test::assert_equal(cmoon::abs(-6), 6);
				cmoon::test::assert_equal(cmoon::abs(-7), 7);
				cmoon::test::assert_equal(cmoon::abs(-8), 8);
				cmoon::test::assert_equal(cmoon::abs(-9), 9);
				cmoon::test::assert_equal(cmoon::abs(-10), 10);

				cmoon::test::assert_equal(cmoon::abs(1.0), 1.0);
				cmoon::test::assert_equal(cmoon::abs(2.0), 2.0);
				cmoon::test::assert_equal(cmoon::abs(3.0), 3.0);
				cmoon::test::assert_equal(cmoon::abs(4.0), 4.0);
				cmoon::test::assert_equal(cmoon::abs(5.0), 5.0);
				cmoon::test::assert_equal(cmoon::abs(6.0), 6.0);
				cmoon::test::assert_equal(cmoon::abs(7.0), 7.0);
				cmoon::test::assert_equal(cmoon::abs(8.0), 8.0);
				cmoon::test::assert_equal(cmoon::abs(9.0), 9.0);
				cmoon::test::assert_equal(cmoon::abs(10.0), 10.0);

				cmoon::test::assert_equal(cmoon::abs(-1.0), 1.0);
				cmoon::test::assert_equal(cmoon::abs(-2.0), 2.0);
				cmoon::test::assert_equal(cmoon::abs(-3.0), 3.0);
				cmoon::test::assert_equal(cmoon::abs(-4.0), 4.0);
				cmoon::test::assert_equal(cmoon::abs(-5.0), 5.0);
				cmoon::test::assert_equal(cmoon::abs(-6.0), 6.0);
				cmoon::test::assert_equal(cmoon::abs(-7.0), 7.0);
				cmoon::test::assert_equal(cmoon::abs(-8.0), 8.0);
				cmoon::test::assert_equal(cmoon::abs(-9.0), 9.0);
				cmoon::test::assert_equal(cmoon::abs(-10.0), 10.0);

				static_assert(cmoon::abs(1) == 1);
				static_assert(cmoon::abs(2) == 2);
				static_assert(cmoon::abs(3) == 3);
				static_assert(cmoon::abs(4) == 4);
				static_assert(cmoon::abs(5) == 5);
				static_assert(cmoon::abs(6) == 6);
				static_assert(cmoon::abs(7) == 7);
				static_assert(cmoon::abs(8) == 8);
				static_assert(cmoon::abs(9) == 9);
				static_assert(cmoon::abs(10) == 10);

				static_assert(cmoon::abs(-1) == 1);
				static_assert(cmoon::abs(-2) == 2);
				static_assert(cmoon::abs(-3) == 3);
				static_assert(cmoon::abs(-4) == 4);
				static_assert(cmoon::abs(-5) == 5);
				static_assert(cmoon::abs(-6) == 6);
				static_assert(cmoon::abs(-7) == 7);
				static_assert(cmoon::abs(-8) == 8);
				static_assert(cmoon::abs(-9) == 9);
				static_assert(cmoon::abs(-10) == 10);

				static_assert(cmoon::abs(1.0) == 1.0);
				static_assert(cmoon::abs(2.0) == 2.0);
				static_assert(cmoon::abs(3.0) == 3.0);
				static_assert(cmoon::abs(4.0) == 4.0);
				static_assert(cmoon::abs(5.0) == 5.0);
				static_assert(cmoon::abs(6.0) == 6.0);
				static_assert(cmoon::abs(7.0) == 7.0);
				static_assert(cmoon::abs(8.0) == 8.0);
				static_assert(cmoon::abs(9.0) == 9.0);
				static_assert(cmoon::abs(10.0) == 10.0);

				static_assert(cmoon::abs(-1.0) == 1.0);
				static_assert(cmoon::abs(-2.0) == 2.0);
				static_assert(cmoon::abs(-3.0) == 3.0);
				static_assert(cmoon::abs(-4.0) == 4.0);
				static_assert(cmoon::abs(-5.0) == 5.0);
				static_assert(cmoon::abs(-6.0) == 6.0);
				static_assert(cmoon::abs(-7.0) == 7.0);
				static_assert(cmoon::abs(-8.0) == 8.0);
				static_assert(cmoon::abs(-9.0) == 9.0);
				static_assert(cmoon::abs(-10.0) == 10.0);
			}
	};
}