export module cmoon.tests.net;
export import cmoon.tests.net.system_executor;
export import cmoon.tests.net.io_context;
export import cmoon.tests.net.dispatch;
export import cmoon.tests.net.post;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::net>()
	{
		cmoon::test::test_suite suite{"net library tests"};
		suite.add_test_case<net::system_executor_test>();
		suite.add_test_case<net::io_context_blocking_execute_test>();
		suite.add_test_case<net::io_context_execute_test>();
		suite.add_test_case<net::io_context_stop_test>();
		suite.add_test_case<net::dispatch_test>();
		suite.add_test_case<net::dispatch_executor_test>();
		suite.add_test_case<net::post_test>();
		suite.add_test_case<net::post_executor_test>();

		return std::move(suite);
	}
}