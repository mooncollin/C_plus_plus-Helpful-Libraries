export module cmoon.tests.execution;
export import cmoon.tests.execution.set_value;
export import cmoon.tests.execution.set_done;
export import cmoon.tests.execution.set_error;
export import cmoon.tests.execution.execute;
export import cmoon.tests.execution.start;
export import cmoon.tests.execution.connect;
export import cmoon.tests.execution.submit;
export import cmoon.tests.execution.schedule;
export import cmoon.tests.execution.bulk_execute;
export import cmoon.tests.execution.just;
export import cmoon.tests.execution.then;
export import cmoon.tests.execution.retry;
export import cmoon.tests.execution.sync_wait;
export import cmoon.tests.execution.when_all;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::execution>()
	{
		cmoon::test::test_suite suite{"execution library tests"};
		suite.add_test_case<execution::set_value_member_fn_test>();
		suite.add_test_case<execution::set_value_default_fn_test>();
		suite.add_test_case<execution::set_done_member_fn_test>();
		suite.add_test_case<execution::set_done_default_fn_test>();
		suite.add_test_case<execution::set_error_member_fn_test>();
		suite.add_test_case<execution::set_error_default_fn_test>();
		suite.add_test_case<execution::execute_member_fn_test>();
		suite.add_test_case<execution::execute_default_fn_test>();
		suite.add_test_case<execution::execute_submit_test>();
		suite.add_test_case<execution::start_member_fn_test>();
		suite.add_test_case<execution::start_default_fn_test>();
		suite.add_test_case<execution::connect_member_fn_test>();
		suite.add_test_case<execution::connect_default_fn_test>();
		suite.add_test_case<execution::submit_member_fn_test>();
		suite.add_test_case<execution::submit_default_fn_test>();
		suite.add_test_case<execution::submit_start_fn_test>();
		suite.add_test_case<execution::schedule_member_fn_test>();
		suite.add_test_case<execution::schedule_default_fn_test>();
		suite.add_test_case<execution::schedule_as_executor_test>();
		suite.add_test_case<execution::bulk_execute_member_fn_test>();
		suite.add_test_case<execution::bulk_execute_default_fn_test>();
		suite.add_test_case<execution::bulk_execute_inline_fn_test>();
		suite.add_test_case<execution::just_set_value_test>();
		suite.add_test_case<execution::just_set_error_test>();
		suite.add_test_case<execution::then_test>();
		suite.add_test_case<execution::retry_test>();
		suite.add_test_case<execution::sync_wait_member_fn_test>();
		suite.add_test_case<execution::sync_wait_default_fn_test>();
		suite.add_test_case<execution::sync_wait_test>();
		suite.add_test_case<execution::when_all_value_test>();
		suite.add_test_case<execution::when_all_error_test>();
		suite.add_test_case<execution::when_all_done_test>();
		suite.add_test_case<execution::when_all_error_and_done_test>();

		return std::move(suite);
	}
}