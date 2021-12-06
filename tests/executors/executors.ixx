export module cmoon.tests.executors;
export import cmoon.tests.executors.inline_scheduler;
export import cmoon.tests.executors.static_thread_pool;
export import cmoon.tests.executors.cached_thread_pool;
//export import cmoon.tests.executors.strand;

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
		suite.add_test_case<executors::inline_scheduler_test>();
		suite.add_test_case<executors::static_thread_pool_execute_test>();
		suite.add_test_case<executors::static_thread_pool_bulk_execute_test>();
		suite.add_test_case<executors::static_thread_pool_schedule_test>();
		suite.add_test_case<executors::static_thread_pool_schedule_on_test>();
		suite.add_test_case<executors::cached_thread_pool_execute_test>();
		suite.add_test_case<executors::cached_thread_pool_bulk_execute_test>();
		suite.add_test_case<executors::cached_thread_pool_schedule_test>();
		suite.add_test_case<executors::cached_thread_pool_schedule_on_test>();
		suite.add_test_case<executors::cached_thread_pool_timeout_test>();
		//suite.add_test_case<executors::strand_single_execute_test>();

		return std::move(suite);
	}
}