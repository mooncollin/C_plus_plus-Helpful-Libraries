export module cmoon.tests.files;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::files>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"files library tests"};

			return std::move(suite);
		}
	};
}