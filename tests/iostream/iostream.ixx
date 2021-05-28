export module cmoon.tests.iostream;
export import cmoon.tests.iostream.lines;
export import cmoon.tests.iostream.read;
export import cmoon.tests.iostream.ostream_joiner;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::iostream>()
	{
		cmoon::test::test_suite suite{"iostream library tests"};
		suite.add_test_case<iostream::lines_string_single_test>();
		suite.add_test_case<iostream::lines_string_test>();
		suite.add_test_case<iostream::lines_integer_single_test>();
		suite.add_test_case<iostream::read_all_streambuf_test>();
		suite.add_test_case<iostream::read_all_string_test>();
		suite.add_test_case<iostream::read_all_iterator_streambuf_test>();
		suite.add_test_case<iostream::read_all_iterator_test>();
		suite.add_test_case<iostream::read_until_streambuf_test>();
		suite.add_test_case<iostream::read_until_string_test>();
		suite.add_test_case<iostream::read_until_iterator_streambuf_test>();
		suite.add_test_case<iostream::read_until_iterator_test>();
		suite.add_test_case<iostream::ignore_until_streambuf_test>();
		suite.add_test_case<iostream::ignore_until_test>();
		suite.add_test_case<iostream::ostream_joiner_test>();

		return std::move(suite);
	}
}