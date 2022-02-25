export module cmoon.tests.iterator;
export import cmoon.tests.iterator.istream_iterator_sep;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::iterator>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"iterator library tests", alloc};
			suite.add_test_case<cmoon::tests::iterator::istream_iterator_sep_string_single_test>();
			suite.add_test_case<cmoon::tests::iterator::istream_iterator_sep_string_test>();
			suite.add_test_case<cmoon::tests::iterator::istream_iterator_sep_integer_single_test>();
			suite.add_test_case<cmoon::tests::iterator::istream_iterator_sep_integer_test>();

			return std::move(suite);
		}
	};
}