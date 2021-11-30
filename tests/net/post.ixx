export module cmoon.tests.net.post;

import <atomic>;

import cmoon.executors;
import cmoon.net;
import cmoon.test;

namespace cmoon::tests::net
{
	export
	class post_test : public cmoon::test::test_case
	{
		public:
			post_test()
				: cmoon::test::test_case{"post_test"} {}

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
	class post_executor_test : public cmoon::test::test_case
	{
		public:
			post_executor_test()
				: cmoon::test::test_case{"post_executor_test"} {}

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