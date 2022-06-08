export module cmoon.tests.functional;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::functional>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"functional library tests"};

			return std::move(suite);
		}
	};
}