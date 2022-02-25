export module cmoon.tests.algorithm;
export import cmoon.tests.algorithm.repeat;
export import cmoon.tests.algorithm.min;
export import cmoon.tests.algorithm.max;
export import cmoon.tests.algorithm.fold;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::algorithm>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{alloc};
			suite.add_test_case<cmoon::tests::algorithm::repeat_none_test>();
			suite.add_test_case<cmoon::tests::algorithm::repeat_test>();
			suite.add_test_case<cmoon::tests::algorithm::min_test>();
			suite.add_test_case<cmoon::tests::algorithm::min_custom_test>();
			suite.add_test_case<cmoon::tests::algorithm::max_test>();
			suite.add_test_case<cmoon::tests::algorithm::max_custom_test>();
			suite.add_test_case<cmoon::tests::algorithm::foldl_test>();
			suite.add_test_case<cmoon::tests::algorithm::foldr_test>();
			suite.add_test_case<cmoon::tests::algorithm::foldl_while_test>();

			return std::move(suite);
		}
	};
}