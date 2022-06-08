export module cmoon.tests.net;
//export import cmoon.tests.net.system_context;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon::tests
{
	export
	template<>
	struct library_traits<library::net>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"net library tests"};
			suite.add_test_case<cmoon::tests::net::service_context_test>();

			return std::move(suite);
		}
	};
	
}