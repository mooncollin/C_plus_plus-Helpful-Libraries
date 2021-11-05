export module cmoon.tests.io;
export import cmoon.tests.io.lines;
export import cmoon.tests.io.read;
export import cmoon.tests.io.ostream_joiner;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::io>()
	{
		cmoon::test::test_suite suite{"iostream library tests"};
		suite.add_test_case<io::lines_string_single_test>();
		suite.add_test_case<io::lines_string_test>();
		suite.add_test_case<io::lines_integer_single_test>();
		suite.add_test_case<io::read_all_streambuf_test>();
		suite.add_test_case<io::read_all_string_test>();
		suite.add_test_case<io::read_all_iterator_streambuf_test>();
		suite.add_test_case<io::read_all_iterator_test>();
		suite.add_test_case<io::read_until_streambuf_test>();
		suite.add_test_case<io::read_until_string_test>();
		suite.add_test_case<io::read_until_iterator_streambuf_test>();
		suite.add_test_case<io::read_until_iterator_test>();
		suite.add_test_case<io::ignore_until_streambuf_test>();
		suite.add_test_case<io::ignore_until_test>();
		suite.add_test_case<io::ostream_joiner_test>();

		return std::move(suite);
	}
}