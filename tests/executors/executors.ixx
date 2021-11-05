export module cmoon.tests.executors;
export import cmoon.tests.executors.inline_executor;
export import cmoon.tests.executors.static_thread_pool;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::executors>()
	{
		cmoon::test::test_suite suite{"executors library tests"};
		suite.add_test_case<executors::inline_executor_test>();
		suite.add_test_case<executors::inline_executor_bulk_sequenced_test>();
		suite.add_test_case<executors::inline_executor_bulk_unsequenced_test>();
		suite.add_test_case<executors::static_thread_pool_blocking_execute_test>();
		suite.add_test_case<executors::static_thread_pool_blocking_bulk_execute_test>();
		suite.add_test_case<executors::static_thread_pool_execute_test>();
		suite.add_test_case<executors::static_thread_pool_bulk_execute_test>();
		suite.add_test_case<executors::static_thread_pool_attach_test>();
		suite.add_test_case<executors::static_thread_pool_blocking_schedule_test>();
		suite.add_test_case<executors::static_thread_pool_schedule_test>();
		suite.add_test_case<executors::static_thread_pool_schedule_stop_test>();

		return std::move(suite);
	}
}