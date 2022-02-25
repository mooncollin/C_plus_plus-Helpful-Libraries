export module cmoon.tests.thread;

import <utility>;
import <memory>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::thread>
	{
		template<class Allocator = std::allocator<cmoon::test::test_case>>
		static cmoon::test::test_suite<Allocator> tests(const Allocator& alloc = {})
		{
			cmoon::test::test_suite<Allocator> suite{"thread library tests", alloc};

			return std::move(suite);
		}
	};
}