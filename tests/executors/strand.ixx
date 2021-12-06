export module cmoon.tests.executors.strand;

import <atomic>;

import cmoon.execution;
import cmoon.executors;
import cmoon.test;

namespace cmoon::tests::executors
{
	export
	class strand_single_execute_test : public cmoon::test::test_case
	{
		public:
			strand_single_execute_test()
				: cmoon::test::test_case{"strand_single_execute_test"} {}

			void operator()() override
			{
				constexpr int expected{5};
				std::atomic<int> value{0};

				cmoon::executors::cached_thread_pool p;
				cmoon::executors::strand s{p.get_scheduler()};

				//cmoon::execution::execute(s.get_scheduler(), [&value, expected] { value = expected; value.notify_one(); });
				cmoon::test::fail("MSVC has a C1001: Internal compiler error compiling this test");

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};
}