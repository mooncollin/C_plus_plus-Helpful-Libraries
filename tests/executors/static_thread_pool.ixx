export module cmoon.tests.executors.static_thread_pool;

import <array>;
import <atomic>;

import cmoon.execution;
import cmoon.executors;
import cmoon.test;

namespace cmoon::tests::executors
{
	export
	class static_thread_pool_execute_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_execute_test()
				: cmoon::test::test_case{"static_thread_pool_execute_test"} {}

			void operator()() override
			{
				constexpr int expected1 {5};
				constexpr int expected2 {10};
				constexpr int expected3 {15};

				int value1 {0};
				int value2 {0};
				int value3 {0};

				cmoon::executors::static_thread_pool p {2};

				auto s = p.get_scheduler();

				cmoon::execution::execute(s, [&value1, expected1] { value1 = expected1; });
				cmoon::execution::execute(s, [&value2, expected2] { value2 = expected2; });
				cmoon::execution::execute(s, [&value3, expected3] { value3 = expected3; });

				p.wait();

				cmoon::test::assert_equal(value1, expected1);
				cmoon::test::assert_equal(value2, expected2);
				cmoon::test::assert_equal(value3, expected3);
			}
	};

	export
	class static_thread_pool_bulk_execute_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_bulk_execute_test()
				: cmoon::test::test_case{"static_thread_pool_bulk_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {5};

				std::array values {0, 0, 0};
				cmoon::executors::static_thread_pool p {2};

				auto s = cmoon::execution::bulk(cmoon::execution::schedule(p.get_scheduler()), std::size(values),
												[&values, expected](std::size_t i) {
													values[i] = expected;
												});

				cmoon::execution::start_detached(std::move(s));

				p.wait();

				cmoon::test::assert_equal(values[0], expected);
				cmoon::test::assert_equal(values[1], expected);
				cmoon::test::assert_equal(values[2], expected);
			}
	};

	export
	class static_thread_pool_schedule_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_schedule_test()
				: cmoon::test::test_case{"static_thread_pool_schedule_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				int value {0};
				cmoon::executors::static_thread_pool p {2};

				auto work = cmoon::execution::schedule(p.get_scheduler()) |
							cmoon::execution::then([] {}) |
							cmoon::execution::then([]{ return 5; }) |
							cmoon::execution::then([](int arg) { return 5 + arg; }) |
							cmoon::execution::then([&value](int arg) { value = arg; });

				cmoon::execution::start_detached(std::move(work));
				p.wait();

				cmoon::test::assert_equal(value, expected);
			}
	};

	export
	class static_thread_pool_schedule_on_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_schedule_on_test()
				: cmoon::test::test_case{"static_thread_pool_schedule_on_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				std::atomic<int> value {0};
				cmoon::executors::static_thread_pool p {2};
				cmoon::executors::static_thread_pool p2 {1};

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
	class static_thread_pool_schedule_stop_test : public cmoon::test::test_case
	{
		public:
			struct receiver_s
			{
				receiver_s(bool& value)
					: done_set{value} {}

				friend void tag_invoke(cmoon::execution::set_value_t, receiver_s&&) {}

				friend void tag_invoke(cmoon::execution::set_done_t, receiver_s&& s) noexcept
				{
					s.done_set = true;
				}

				friend void tag_invoke(cmoon::execution::set_error_t, receiver_s&&, std::exception_ptr) noexcept {}

				bool& done_set;
			};

			static_thread_pool_schedule_stop_test()
				: cmoon::test::test_case{"static_thread_pool_schedule_stop_test"} {}

			void operator()() override
			{
				bool value {false};
				receiver_s r {value};
				cmoon::executors::static_thread_pool p {0};

				cmoon::execution::execute(p.get_scheduler(), []{});
				auto work = cmoon::execution::schedule(p.get_scheduler());

				cmoon::execution::start(
					cmoon::execution::connect(
						std::move(work),
						receiver_s{value}
					)
				);

				p.stop();
				p.wait();

				cmoon::test::assert_true(value);
			}
	};
}