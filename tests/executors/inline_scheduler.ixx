export module cmoon.tests.executors.inline_scheduler;

import cmoon.execution;
import cmoon.executors;
import cmoon.test;

namespace cmoon::tests::executors
{
	export
	class inline_scheduler_test : public cmoon::test::test_case
	{
		public:
			inline_scheduler_test()
				: cmoon::test::test_case{"inline_scheduler_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int v {0};
				cmoon::executors::inline_scheduler e;
				cmoon::execution::execute(e, [&v, expected]{ v = expected; });

				cmoon::test::assert_equal(v, expected);
			}
	};
}