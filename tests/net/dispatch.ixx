export module cmoon.tests.net.dispatch;

import <atomic>;

import cmoon.executors;
import cmoon.net;
import cmoon.test;

namespace cmoon::tests::net
{
	export
	class dispatch_test : public cmoon::test::test_case
	{
		public:
			dispatch_test()
				: cmoon::test::test_case{"dispatch_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};

				cmoon::net::dispatch([expected, &value] { value = expected; value.notify_one(); });

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class dispatch_executor_test : public cmoon::test::test_case
	{
		public:
			dispatch_executor_test()
				: cmoon::test::test_case{"dispatch_executor_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				std::atomic<int> value {0};
				cmoon::executors::cached_thread_pool p;

				cmoon::net::dispatch(p.executor(), [expected, &value] { value = expected; value.notify_one(); });

				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};
}