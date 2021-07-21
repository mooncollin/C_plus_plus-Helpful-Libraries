export module cmoon.tests.math.sign;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class sign_test : public cmoon::test::test_case
	{
		public:
			sign_test()
				: cmoon::test::test_case{"sign_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::sign(0), 1);
				cmoon::test::assert_equal(cmoon::sign(1), 1);
				cmoon::test::assert_equal(cmoon::sign(-1), -1);

				static_assert(cmoon::sign(0) == 1);
				static_assert(cmoon::sign(1) == 1);
				static_assert(cmoon::sign(-1) == -1);

				cmoon::test::assert_true(cmoon::is_positive(0));
				cmoon::test::assert_true(cmoon::is_positive(1));
				cmoon::test::assert_false(cmoon::is_positive(-1));

				static_assert(cmoon::is_positive(0));
				static_assert(cmoon::is_positive(1));
				static_assert(!cmoon::is_positive(-1));

				cmoon::test::assert_false(cmoon::is_negative(0));
				cmoon::test::assert_false(cmoon::is_negative(1));
				cmoon::test::assert_true(cmoon::is_negative(-1));

				static_assert(!cmoon::is_negative(0));
				static_assert(!cmoon::is_negative(1));
				static_assert(cmoon::is_negative(-1));
			}
	};
}