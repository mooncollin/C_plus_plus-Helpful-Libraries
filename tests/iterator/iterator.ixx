export module cmoon.tests.iterator;
export import cmoon.tests.iterator.istream_iterator_sep;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::iterator>()
	{
		cmoon::test::test_suite suite{"iterator library tests"};
		suite.add_test_case<iterator::istream_iterator_sep_string_single_test>();
		suite.add_test_case<iterator::istream_iterator_sep_string_test>();
		suite.add_test_case<iterator::istream_iterator_sep_integer_single_test>();
		suite.add_test_case<iterator::istream_iterator_sep_integer_test>();

		return std::move(suite);
	}
}