export module cmoon.tests.thread;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::thread>()
	{
		cmoon::test::test_suite suite{"thread library tests"};

		return std::move(suite);
	}
}