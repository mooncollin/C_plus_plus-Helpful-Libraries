export module cmoon.tests.functional;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::functional>()
	{
		cmoon::test::test_suite suite{"functional library tests"};

		return std::move(suite);
	}
}