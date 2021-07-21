export module cmoon.tests.math;
export import cmoon.tests.math.is_odd_even;
export import cmoon.tests.math.factorial;
export import cmoon.tests.math.rational;
export import cmoon.tests.math.pow;
export import cmoon.tests.math.abs;
export import cmoon.tests.math.ceil;
export import cmoon.tests.math.trig;
export import cmoon.tests.math.sign;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::math>()
	{
		cmoon::test::test_suite suite{"math library tests"};
		suite.add_test_case<math::is_odd_test>();
		suite.add_test_case<math::is_even_test>();
		suite.add_test_case<math::factorial_test>();
		suite.add_test_case<math::rational_constructor_test>();
		suite.add_test_case<math::rational_add_test>();
		suite.add_test_case<math::rational_multiply_test>();
		suite.add_test_case<math::rational_mult_inverse_test>();
		suite.add_test_case<math::rational_add_inverse_test>();
		suite.add_test_case<math::rational_canonical_test>();
		suite.add_test_case<math::pow_test>();
		suite.add_test_case<math::static_pow_test>();
		suite.add_test_case<math::abs_test>();
		suite.add_test_case<math::ceil_test>();
		suite.add_test_case<math::cot_test>();
		suite.add_test_case<math::sign_test>();

		return std::move(suite);
	}
}