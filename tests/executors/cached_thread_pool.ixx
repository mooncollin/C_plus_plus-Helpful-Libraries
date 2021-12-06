export module cmoon.tests.executors.cached_thread_pool;

import <array>;
import <chrono>;

import cmoon.property;
import cmoon.execution;
import cmoon.executors;
import cmoon.test;

namespace cmoon::tests::executors
{
	export
	class cached_thread_pool_execute_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_execute_test()
				: cmoon::test::test_case{"cached_thread_pool_execute_test"} {}

			void operator()() override
			{
				constexpr int expected1 {5};
				constexpr int expected2 {10};
				constexpr int expected3 {15};

				int value1 {0};
				int value2 {0};
				int value3 {0};

				cmoon::executors::cached_thread_pool p;

				auto s = p.get_scheduler();

				cmoon::execution::execute(s, [&value1, expected1] { value1 = expected1; });
				cmoon::execution::execute(s, [&value2, expected2] { value2 = expected2; });
				cmoon::execution::execute(s, [&value3, expected3] { value3 = expected3; });

				p.request_stop();
				p.join();

				cmoon::test::assert_equal(value1, expected1);
				cmoon::test::assert_equal(value2, expected2);
				cmoon::test::assert_equal(value3, expected3);
			}
	};

	export
	class cached_thread_pool_bulk_execute_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_bulk_execute_test()
				: cmoon::test::test_case{"cached_thread_pool_bulk_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {5};

				std::array values {0, 0, 0};
				cmoon::executors::cached_thread_pool p;

				auto s = cmoon::execution::bulk(cmoon::execution::schedule(p.get_scheduler()), std::size(values),
												[&values, expected](std::size_t i) {
													values[i] = expected;
												});

				cmoon::execution::start_detached(std::move(s));

				p.request_stop();
				p.join();

				cmoon::test::assert_equal(values[0], expected);
				cmoon::test::assert_equal(values[1], expected);
				cmoon::test::assert_equal(values[2], expected);
			}
	};

	export
	class cached_thread_pool_schedule_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_schedule_test()
				: cmoon::test::test_case{"cached_thread_pool_schedule_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				int value {0};
				cmoon::executors::cached_thread_pool p;

				auto work = cmoon::execution::schedule(p.get_scheduler()) |
							cmoon::execution::then([] {}) |
							cmoon::execution::then([]{ return 5; }) |
							cmoon::execution::then([&value](int arg) { value = 5 + arg; });

				cmoon::execution::start_detached(std::move(work));
				p.request_stop();
				p.join();

				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class cached_thread_pool_schedule_on_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_schedule_on_test()
				: cmoon::test::test_case{"cached_thread_pool_schedule_on_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				std::atomic<int> value {0};
				cmoon::executors::cached_thread_pool p;
				cmoon::executors::cached_thread_pool p2;

				auto work = cmoon::execution::schedule(p.get_scheduler()) |
							cmoon::execution::then([] {}) |
							cmoon::execution::then([]{ return 5; }) |
							cmoon::execution::on(p2.get_scheduler()) |
							cmoon::execution::then([](int arg) { return 5 + arg; }) |
							cmoon::execution::on(p.get_scheduler()) |
							cmoon::execution::then([&value](int arg) { value = arg; value.notify_one(); });

				cmoon::execution::start_detached(std::move(work));
				value.wait(0);
				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class cached_thread_pool_timeout_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_timeout_test()
				: cmoon::test::test_case{"cached_thread_pool_timeout_test"} {}

			void operator()() override
			{
				constexpr int expected1 {1};
				constexpr int expected2 {2};
				int value1 {0};
				int value2 {0};
				cmoon::executors::cached_thread_pool p{std::chrono::milliseconds{200}};

				auto s = p.get_scheduler();

				cmoon::execution::execute(s, [&value1, expected1]{ value1 = expected1; });

				std::this_thread::sleep_for(std::chrono::seconds{1});

				cmoon::execution::execute(s, [&value2, expected2]{ value2 = expected2; });

				p.request_stop();
				p.join();

				cmoon::test::assert_equal(value1, expected1);
				cmoon::test::assert_equal(value2, expected2);
			}
	};
}