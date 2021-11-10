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
	class cached_thread_pool_blocking_execute_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_blocking_execute_test()
				: cmoon::test::test_case{"cached_thread_pool_blocking_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				cmoon::executors::cached_thread_pool p;
				int value {0};

				auto e = cmoon::require(p.executor(), cmoon::execution::blocking.always);
				cmoon::execution::execute(e, [&value, expected] { value = expected; });
				cmoon::test::assert_equal(value, expected);
				cmoon::test::assert_true(e.running_in_this_thread());
			}
	};

	export
	class cached_thread_pool_blocking_bulk_execute_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_blocking_bulk_execute_test()
				: cmoon::test::test_case{"cached_thread_pool_blocking_bulk_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {1};
				cmoon::executors::cached_thread_pool p;

				std::array values {0, 0, 0};

				auto e = cmoon::require(p.executor(), cmoon::execution::blocking.always);
				cmoon::execution::bulk_execute(e, [&values, expected](std::size_t i) {
					values[i] = expected;
				}, std::size(values));

				cmoon::test::assert_equal(values[0], expected);
				cmoon::test::assert_equal(values[1], expected);
				cmoon::test::assert_equal(values[2], expected);
				cmoon::test::assert_true(e.running_in_this_thread());
			}
	};

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

				auto e = p.executor();
				cmoon::execution::execute(e, [&value1, expected1] { value1 = expected1; });
				cmoon::execution::execute(e, [&value2, expected2] { value2 = expected2; });
				cmoon::execution::execute(e, [&value3, expected3] { value3 = expected3; });
				cmoon::test::assert_false(e.running_in_this_thread());

				p.wait();

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

				auto e = p.executor();
				cmoon::execution::bulk_execute(e, [&values, expected](std::size_t i) {
					values[i] = expected;
				}, std::size(values));

				p.wait();

				cmoon::test::assert_equal(values[0], expected);
				cmoon::test::assert_equal(values[1], expected);
				cmoon::test::assert_equal(values[2], expected);
				cmoon::test::assert_false(e.running_in_this_thread());
			}
	};

	export
	class cached_thread_pool_blocking_schedule_test : public cmoon::test::test_case
	{
		public:
			cached_thread_pool_blocking_schedule_test()
				: cmoon::test::test_case{"cached_thread_pool_blocking_schedule_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				cmoon::executors::cached_thread_pool p;
				int value {0};

				auto ex = cmoon::require(p.executor(), cmoon::execution::blocking.always);
				auto work = cmoon::execution::schedule(ex) |
							cmoon::execution::then([] {}) |
							cmoon::execution::then([]{ return 5; }) |
							cmoon::execution::then([](int arg) { return 5 + arg; });

				auto capture_result = cmoon::execution::as_receiver([&value](int arg) { value = arg; });

				cmoon::execution::submit(std::move(work), capture_result);
				p.wait();

				cmoon::test::assert_equal(value, expected);
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
				cmoon::executors::cached_thread_pool p;
				int value {0};

				auto work = cmoon::execution::schedule(p.executor()) |
							cmoon::execution::then([] {}) |
							cmoon::execution::then([]{ return 5; }) |
							cmoon::execution::then([](int arg) { return 5 + arg; });

				auto capture_result = cmoon::execution::as_receiver([&value](int arg) { value = arg; });

				cmoon::execution::submit(std::move(work), capture_result);
				p.wait();

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

				auto e = p.executor();

				cmoon::execution::execute(e, [&value1, expected1]{ value1 = expected1; });

				std::this_thread::sleep_for(std::chrono::seconds{1});

				cmoon::execution::execute(e, [&value2, expected2]{ value2 = expected2; });

				p.wait();

				cmoon::test::assert_equal(value1, expected1);
				cmoon::test::assert_equal(value2, expected2);
			}
	};
}