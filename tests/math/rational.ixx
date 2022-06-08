export module cmoon.tests.math.rational;

import cmoon.test;
import cmoon.math;

namespace cmoon::tests::math
{
	export
	class rational_constructor_test : public cmoon::test::test_case
	{
		public:
			rational_constructor_test()
				: cmoon::test::test_case{"rational_constructor_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::rational(1), 1);
				cmoon::test::assert_equal(static_cast<double>(cmoon::rational(1, 2)), 0.5);
				cmoon::test::assert_equal(static_cast<double>(cmoon::rational(1, 4)), 0.25);
				cmoon::test::assert_equal(cmoon::rational(12, 3), 4);
				cmoon::test::assert_equal(cmoon::rational(3, 1), 3);
				cmoon::test::assert_equal(cmoon::rational(-3, 1), -3);
				cmoon::test::assert_equal(cmoon::rational(3, -1), -3);

				static_assert(cmoon::rational(1) == 1);
				static_assert(cmoon::rational(1, 3) < cmoon::rational(1, 2));
				static_assert(cmoon::rational(1) > cmoon::rational(1, 5));
				static_assert(cmoon::rational(2, 5) >= cmoon::rational(20, 50));
				static_assert(cmoon::rational(12, 3) == 4);
				static_assert(cmoon::rational(3, 1) == 3);
				static_assert(cmoon::rational(-3, 1) == -3);
				static_assert(cmoon::rational(3, -1) == -3);
			}
	};

	export
	class rational_add_test : public cmoon::test::test_case
	{
		public:
			rational_add_test()
				: cmoon::test::test_case{"rational_add_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::rational(1, 2) + cmoon::rational(1, 2), 1);
				cmoon::test::assert_equal(cmoon::rational(1, 3) + cmoon::rational(1, 3) + cmoon::rational(1, 3), 1);

				static_assert(cmoon::rational(1, 2) + cmoon::rational(1, 2) == 1);
				static_assert(cmoon::rational(1, 3) + cmoon::rational(1, 3) + cmoon::rational(1, 3) == 1);
			}
	};

	export
	class rational_multiply_test : public cmoon::test::test_case
	{
		public:
			rational_multiply_test()
				: cmoon::test::test_case{"rational_multiply_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::rational(1, 3) * 3, 1);

				static_assert(cmoon::rational(1, 3) * 3 == 1);
			}
	};

	export
	class rational_mult_inverse_test : public cmoon::test::test_case
	{
		public:
			rational_mult_inverse_test()
				: cmoon::test::test_case{"rational_mult_inverse_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::mult_inverse(cmoon::rational(1, -3)), -3);

				static_assert(cmoon::mult_inverse(cmoon::rational(1, -3)) == -3);
			}
	};

	export
	class rational_add_inverse_test : public cmoon::test::test_case
	{
		public:
			rational_add_inverse_test()
				: cmoon::test::test_case{"rational_add_inverse_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::add_inverse(cmoon::rational(-5, 1)), 5);
				cmoon::test::assert_equal(cmoon::add_inverse(cmoon::rational(5, 1)), -5);

				static_assert(cmoon::add_inverse(cmoon::rational(-5, 1)) == 5);
				static_assert(cmoon::add_inverse(cmoon::rational(5, 1)) == -5);
			}
	};

	export
	class rational_canonical_test : public cmoon::test::test_case
	{
		public:
			rational_canonical_test()
				: cmoon::test::test_case{"rational_canonical_test"} {}

			void operator()() override
			{
				cmoon::test::assert_equal(cmoon::canonical(cmoon::rational(12, 3)), cmoon::rational(4, 1));

				static_assert(cmoon::canonical(cmoon::rational(12, 3)) == cmoon::rational(4, 1));
			}
	};
}