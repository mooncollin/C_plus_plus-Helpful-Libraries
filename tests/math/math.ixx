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
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::math>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"math library tests", alloc};
			suite.add_test_case<cmoon::tests::math::is_odd_test>();
			suite.add_test_case<cmoon::tests::math::is_even_test>();
			suite.add_test_case<cmoon::tests::math::factorial_test>();
			suite.add_test_case<cmoon::tests::math::rational_constructor_test>();
			suite.add_test_case<cmoon::tests::math::rational_add_test>();
			suite.add_test_case<cmoon::tests::math::rational_multiply_test>();
			suite.add_test_case<cmoon::tests::math::rational_mult_inverse_test>();
			suite.add_test_case<cmoon::tests::math::rational_add_inverse_test>();
			suite.add_test_case<cmoon::tests::math::rational_canonical_test>();
			suite.add_test_case<cmoon::tests::math::pow_test>();
			suite.add_test_case<cmoon::tests::math::static_pow_test>();
			suite.add_test_case<cmoon::tests::math::abs_test>();
			suite.add_test_case<cmoon::tests::math::ceil_test>();
			suite.add_test_case<cmoon::tests::math::cot_test>();
			suite.add_test_case<cmoon::tests::math::sign_test>();

			return std::move(suite);
		}
	};
}