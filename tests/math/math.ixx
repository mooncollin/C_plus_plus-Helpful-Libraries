export module cmoon.tests.math;
export import cmoon.tests.math.is_odd_even;
export import cmoon.tests.math.factorial;
export import cmoon.tests.math.rational;
export import cmoon.tests.math.trig;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::math>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"math library tests"};
			suite.add_test_case<cmoon::tests::math::is_odd_test>();
			suite.add_test_case<cmoon::tests::math::is_even_test>();
			suite.add_test_case<cmoon::tests::math::factorial_test>();
			suite.add_test_case<cmoon::tests::math::rational_constructor_test>();
			suite.add_test_case<cmoon::tests::math::rational_add_test>();
			suite.add_test_case<cmoon::tests::math::rational_multiply_test>();
			suite.add_test_case<cmoon::tests::math::rational_mult_inverse_test>();
			suite.add_test_case<cmoon::tests::math::rational_add_inverse_test>();
			suite.add_test_case<cmoon::tests::math::rational_canonical_test>();
			suite.add_test_case<cmoon::tests::math::cot_test>();

			return std::move(suite);
		}
	};
}