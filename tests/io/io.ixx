export module cmoon.tests.io;
export import cmoon.tests.io.lines;
export import cmoon.tests.io.read;
export import cmoon.tests.io.ostream_joiner;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::io>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"iostream library tests", alloc};
			suite.add_test_case<cmoon::tests::io::lines_string_single_test>();
			suite.add_test_case<cmoon::tests::io::lines_string_test>();
			suite.add_test_case<cmoon::tests::io::lines_integer_single_test>();
			suite.add_test_case<cmoon::tests::io::read_all_streambuf_test>();
			suite.add_test_case<cmoon::tests::io::read_all_string_test>();
			suite.add_test_case<cmoon::tests::io::read_all_iterator_streambuf_test>();
			suite.add_test_case<cmoon::tests::io::read_all_iterator_test>();
			suite.add_test_case<cmoon::tests::io::read_until_streambuf_test>();
			suite.add_test_case<cmoon::tests::io::read_until_string_test>();
			suite.add_test_case<cmoon::tests::io::read_until_iterator_streambuf_test>();
			suite.add_test_case<cmoon::tests::io::read_until_iterator_test>();
			suite.add_test_case<cmoon::tests::io::ignore_until_streambuf_test>();
			suite.add_test_case<cmoon::tests::io::ignore_until_test>();
			suite.add_test_case<cmoon::tests::io::ostream_joiner_test>();

			return std::move(suite);
		}
	};
}