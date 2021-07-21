export module cmoon.tests.json;
export import cmoon.tests.json.json_parser;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::json>()
	{
		cmoon::test::test_suite suite{"json library tests"};
		suite.add_test_case<json::json_parse_test>();

		return std::move(suite);
	}
}