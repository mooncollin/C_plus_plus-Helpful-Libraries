export module cmoon.tests.files;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;


namespace cmoon
{
	export
	template<>
	struct library_traits<library::files>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"files library tests", alloc};

			return std::move(suite);
		}
	};
}