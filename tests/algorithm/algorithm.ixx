export module cmoon.tests.algorithm;
export import cmoon.tests.algorithm.repeat;
export import cmoon.tests.algorithm.min;
export import cmoon.tests.algorithm.max;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::algorithm>()
	{
		cmoon::test::test_suite suite;
		suite.add_test_case<algorithm::repeat_none_test>();
		suite.add_test_case<algorithm::repeat_test>();
		suite.add_test_case<algorithm::min_single_test>();
		suite.add_test_case<algorithm::min_test>();
		suite.add_test_case<algorithm::max_single_test>();
		suite.add_test_case<algorithm::max_test>();

		return std::move(suite);
	}
}