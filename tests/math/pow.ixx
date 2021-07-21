export module cmoon.tests.math.pow;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class pow_test : public cmoon::test::test_case
	{
		public:
			pow_test()
				: cmoon::test::test_case{"pow_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::pow(2, 0), 1);
				cmoon::test::assert_equal(cmoon::pow(2, 1), 2);
				cmoon::test::assert_equal(cmoon::pow(2, 2), 4);
				cmoon::test::assert_equal(cmoon::pow(2, 3), 8);
				cmoon::test::assert_equal(cmoon::pow(34, 3), 39304);
				cmoon::test::assert_equal(cmoon::pow(9, 5), 59049);
				cmoon::test::assert_almost_equal(cmoon::pow(1.5, 5), 7.59375, 0.000001);
				cmoon::test::assert_almost_equal(cmoon::pow(9, 0.5), 3.0, 0.01);

				static_assert(cmoon::pow(2, 0) == 1);
				static_assert(cmoon::pow(2, 1) == 2);
				static_assert(cmoon::pow(2, 2) == 4);
				static_assert(cmoon::pow(2, 3) == 8);
				static_assert(cmoon::pow(34, 3) == 39304);
				static_assert(cmoon::pow(9, 5) == 59049);
				static_assert(cmoon::abs(cmoon::pow(1.5, 5) - 7.59375) <= 0.000001);
			}
	};

	export
	class static_pow_test : public cmoon::test::test_case
	{
		public:
			static_pow_test()
				: cmoon::test::test_case{"static_pow_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::static_pow<0>(2), 1);
				cmoon::test::assert_equal(cmoon::static_pow<1>(2), 2);
				cmoon::test::assert_equal(cmoon::static_pow<2>(2), 4);
				cmoon::test::assert_equal(cmoon::static_pow<3>(2), 8);
				cmoon::test::assert_equal(cmoon::static_pow<3>(34), 39304);
				cmoon::test::assert_equal(cmoon::static_pow<5>(9), 59049);

				static_assert(cmoon::static_pow<0>(2) == 1);
				static_assert(cmoon::static_pow<1>(2) == 2);
				static_assert(cmoon::static_pow<2>(2) == 4);
				static_assert(cmoon::static_pow<3>(2) == 8);
				static_assert(cmoon::static_pow<3>(34) == 39304);
				static_assert(cmoon::static_pow<5>(9) == 59049);
			}
	};
}