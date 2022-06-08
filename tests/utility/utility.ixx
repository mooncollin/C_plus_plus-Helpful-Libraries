export module cmoon.tests.utility;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::utility>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"utility library tests"};

			return std::move(suite);
		}
	};
}