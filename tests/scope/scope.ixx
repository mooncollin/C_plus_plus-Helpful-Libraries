export module cmoon.tests.scope;
export import cmoon.tests.scope.unique_resource;
export import cmoon.tests.scope.scope_exit;
export import cmoon.tests.scope.scope_success;
export import cmoon.tests.scope.scope_fail;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::scope>()
	{
		cmoon::test::test_suite suite{"scope library tests"};
		suite.add_test_case<scope::unique_resource_initialization_test>();
		suite.add_test_case<scope::scope_exit_test>();
		suite.add_test_case<scope::scope_success_test>();
		suite.add_test_case<scope::scope_fail_test>();

		return std::move(suite);
	}
}