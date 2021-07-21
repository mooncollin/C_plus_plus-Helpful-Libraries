export module cmoon.tests.math.is_odd_even;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class is_odd_test : public cmoon::test::test_case
	{
		public:
			is_odd_test()
				: cmoon::test::test_case{"is_odd_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::is_odd(1), true);
				cmoon::test::assert_equal(cmoon::is_odd(2), false);
				cmoon::test::assert_equal(cmoon::is_odd(3), true);
				cmoon::test::assert_equal(cmoon::is_odd(4), false);
				cmoon::test::assert_equal(cmoon::is_odd(5), true);
				cmoon::test::assert_equal(cmoon::is_odd(6), false);
				cmoon::test::assert_equal(cmoon::is_odd(7), true);
				cmoon::test::assert_equal(cmoon::is_odd(8), false);
				cmoon::test::assert_equal(cmoon::is_odd(9), true);
				cmoon::test::assert_equal(cmoon::is_odd(10), false);
				cmoon::test::assert_equal(cmoon::is_odd(-3), true);
				cmoon::test::assert_equal(cmoon::is_odd(-2), false);
				cmoon::test::assert_equal(cmoon::is_odd(0), false);

				static_assert(cmoon::is_odd(1) == true);
				static_assert(cmoon::is_odd(2) == false);
				static_assert(cmoon::is_odd(3) == true);
				static_assert(cmoon::is_odd(4) == false);
				static_assert(cmoon::is_odd(5) == true);
				static_assert(cmoon::is_odd(6) == false);
				static_assert(cmoon::is_odd(7) == true);
				static_assert(cmoon::is_odd(8) == false);
				static_assert(cmoon::is_odd(9) == true);
				static_assert(cmoon::is_odd(10) == false);
				static_assert(cmoon::is_odd(-3) == true);
				static_assert(cmoon::is_odd(-2) == false);
				static_assert(cmoon::is_odd(0) == false);

			}
	};

	export
	class is_even_test : public cmoon::test::test_case
	{
		public:
			is_even_test()
				: cmoon::test::test_case{"is_odd_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::is_even(1), false);
				cmoon::test::assert_equal(cmoon::is_even(2), true);
				cmoon::test::assert_equal(cmoon::is_even(3), false);
				cmoon::test::assert_equal(cmoon::is_even(4), true);
				cmoon::test::assert_equal(cmoon::is_even(5), false);
				cmoon::test::assert_equal(cmoon::is_even(6), true);
				cmoon::test::assert_equal(cmoon::is_even(7), false);
				cmoon::test::assert_equal(cmoon::is_even(8), true);
				cmoon::test::assert_equal(cmoon::is_even(9), false);
				cmoon::test::assert_equal(cmoon::is_even(10), true);
				cmoon::test::assert_equal(cmoon::is_even(-3), false);
				cmoon::test::assert_equal(cmoon::is_even(-2), true);
				cmoon::test::assert_equal(cmoon::is_even(0), true);

				static_assert(cmoon::is_even(1) == false);
				static_assert(cmoon::is_even(2) == true);
				static_assert(cmoon::is_even(3) == false);
				static_assert(cmoon::is_even(4) == true);
				static_assert(cmoon::is_even(5) == false);
				static_assert(cmoon::is_even(6) == true);
				static_assert(cmoon::is_even(7) == false);
				static_assert(cmoon::is_even(8) == true);
				static_assert(cmoon::is_even(9) == false);
				static_assert(cmoon::is_even(10) == true);
				static_assert(cmoon::is_even(-3) == false);
				static_assert(cmoon::is_even(-2) == true);
				static_assert(cmoon::is_even(0) == true);

			}
	};
}