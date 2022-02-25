export module cmoon.tests.ratio;
export import cmoon.tests.ratio.is_ratio;
export import cmoon.tests.ratio.ratio_add_many;
export import cmoon.tests.ratio.ratio_subtract_many;
export import cmoon.tests.ratio.ratio_multiply_many;
export import cmoon.tests.ratio.ratio_divide_many;
export import cmoon.tests.ratio.ratio_additive_inverse;
export import cmoon.tests.ratio.ratio_canonical;
export import cmoon.tests.ratio.ratio_reciprocal;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::ratio>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"ratio library tests", alloc};
			suite.add_test_case<cmoon::tests::ratio::is_ratio_test>();
			suite.add_test_case<cmoon::tests::ratio::is_ratio_v_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_type_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_reciprocal_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_canonical_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_additive_inverse_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_add_many_single_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_add_many_double_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_add_many_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_subtract_many_single_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_subtract_many_double_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_subtract_many_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_multiply_many_single_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_multiply_many_double_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_multiply_many_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_divide_many_single_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_divide_many_double_test>();
			suite.add_test_case<cmoon::tests::ratio::ratio_divide_many_test>();

			return std::move(suite);
		}
	};
}