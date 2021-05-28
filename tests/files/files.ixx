export module cmoon.tests.files;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::files>()
	{
		cmoon::test::test_suite suite{"files library tests"};

		return std::move(suite);
	}
}