export module cmoon.tests.mutex;
export import cmoon.tests.mutex.multi_lock;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::mutex>()
	{
		cmoon::test::test_suite suite{"mutex library tests"};
		suite.add_test_case<mutex::multi_lock_default_constructor>();
		suite.add_test_case<mutex::multi_lock_regular_constructor>();

		return std::move(suite);
	}
}