export module cmoon.tests.thread;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::thread>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"thread library tests"};

			return std::move(suite);
		}
	};
}