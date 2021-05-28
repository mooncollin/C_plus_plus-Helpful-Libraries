export module cmoon.test.runner;

import cmoon.test.test_case;
import cmoon.test.test_suite;

namespace cmoon::test
{
	export
	class test_runner
	{
		public:
			test_runner() = default;
			virtual ~test_runner() noexcept = default;
			virtual bool run(test_case&) = 0;
			virtual bool run(test_suite&) = 0;
	};
}