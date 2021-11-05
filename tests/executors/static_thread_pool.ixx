export module cmoon.tests.executors.static_thread_pool;

import <array>;
import <chrono>;

import cmoon.property;
import cmoon.execution;
import cmoon.executors;
import cmoon.test;

namespace cmoon::tests::executors
{
	export
	class static_thread_pool_blocking_execute_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_blocking_execute_test()
				: cmoon::test::test_case{"static_thread_pool_blocking_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				cmoon::executors::static_thread_pool p{1};
				int value {0};

				auto e = cmoon::require(p.executor(), cmoon::execution::blocking.always);
				cmoon::execution::execute(e, [&value, expected] { value = expected; });
				cmoon::test::assert_equal(value, expected);
				cmoon::test::assert_true(e.running_in_this_thread());
			}
	};

	export
	class static_thread_pool_blocking_bulk_execute_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_blocking_bulk_execute_test()
				: cmoon::test::test_case{"static_thread_pool_blocking_bulk_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {1};
				cmoon::executors::static_thread_pool p{1};

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
	class static_thread_pool_attach_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_attach_test()
				: cmoon::test::test_case{"static_thread_pool_attach_test"} {}

			void operator()() override
			{
				bool another_thread {false};
				bool this_thread {false};
				cmoon::executors::static_thread_pool p {2};
				cmoon::executors::static_thread_pool p2 {0};

				auto e = p.executor();
				auto e2 = p2.executor();

				cmoon::execution::execute(e, [&another_thread, e] { another_thread = e.running_in_this_thread(); });
				cmoon::execution::execute(e2, [&this_thread, e2, &p2] { this_thread = e2.running_in_this_thread(); p2.stop(); });

				p.wait();
				p2.attach();

				cmoon::test::assert_true(another_thread);
				cmoon::test::assert_true(this_thread);
			}
	};

	export
	class static_thread_pool_blocking_schedule_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_blocking_schedule_test()
				: cmoon::test::test_case{"static_thread_pool_blocking_schedule_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				cmoon::executors::static_thread_pool p {2};
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
	class static_thread_pool_schedule_test : public cmoon::test::test_case
	{
		public:
			static_thread_pool_schedule_test()
				: cmoon::test::test_case{"static_thread_pool_schedule_test"} {}

			void operator()() override
			{
				constexpr int expected {10};
				cmoon::executors::static_thread_pool p {2};
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
	class static_thread_pool_schedule_stop_test : public cmoon::test::test_case
	{
		struct receiver_s
		{
			receiver_s(bool& value)
				: done_set{value} {}

			void set_value() {}
			void set_done() noexcept
			{
				done_set = true;
			}
			void set_error(std::exception_ptr) noexcept {}

			bool& done_set;
		};

		static_assert(cmoon::execution::receiver<receiver_s>);

		public:
			static_thread_pool_schedule_stop_test()
				: cmoon::test::test_case{"static_thread_pool_schedule_stop_test"} {}

			void operator()() override
			{
				bool value {false};
				receiver_s r {value};
				cmoon::executors::static_thread_pool p {0};

				cmoon::execution::execute(p.executor(), []{});
				auto work = cmoon::execution::schedule(p.executor());

				cmoon::execution::submit(std::move(work), r);
				p.stop();
				p.wait();

				cmoon::test::assert_true(value);
			}
	};
}