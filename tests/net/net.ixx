export module cmoon.tests.net;
export import cmoon.tests.net.system_context;

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
		suite.add_test_case<net::system_context_test>();

		return std::move(suite);
	}
}