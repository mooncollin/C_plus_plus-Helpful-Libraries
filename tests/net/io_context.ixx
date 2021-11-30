export module cmoon.tests.net.io_context;

import <array>;
import <chrono>;

import cmoon.property;
import cmoon.execution;
import cmoon.net;
import cmoon.test;

namespace cmoon::tests::net
{
	export
	class io_context_blocking_execute_test : public cmoon::test::test_case
	{
		public:
			io_context_blocking_execute_test()
				: cmoon::test::test_case{"io_context_blocking_execute_test"} {}

			void operator()() override
			{
				constexpr int expected {5};
				int value {0};

				cmoon::net::io_context io;

				auto e = cmoon::require(io.executor(), cmoon::execution::blocking.always);
				cmoon::execution::execute(e, [&value, expected] { value = expected; });

				cmoon::test::assert_equal(value, expected);
				cmoon::test::assert_true(e.running_in_this_thread());

				cmoon::test::assert_equal(io.run(), 0);
				cmoon::test::assert_true(io.stopped());
			}
	};

	export
	class io_context_execute_test : public cmoon::test::test_case
	{
		public:
			io_context_execute_test()
				: cmoon::test::test_case{"io_context_execute_test"} {}

			void operator()() override
			{
				constexpr int expected1 {5};
				constexpr int expected2 {10};
				constexpr int expected3 {15};

				int value1 {0};
				int value2 {0};
				int value3 {0};

				cmoon::net::io_context io;

				auto e = io.executor();
				cmoon::execution::execute(e, [&value1, expected1] { value1 = expected1; });
				cmoon::execution::execute(e, [&value2, expected2] { value2 = expected2; });
				cmoon::execution::execute(e, [&value3, expected3] { value3 = expected3; });
				cmoon::test::assert_false(e.running_in_this_thread());

				cmoon::test::assert_equal(io.run(), 3);
				cmoon::test::assert_true(io.stopped());

				cmoon::test::assert_equal(value1, expected1);
				cmoon::test::assert_equal(value2, expected2);
				cmoon::test::assert_equal(value3, expected3);
			}
	};

	export
	class io_context_stop_test : public cmoon::test::test_case
	{
		public:
			io_context_stop_test()
				: cmoon::test::test_case{"io_context_stop_test"} {}

			void operator()() override
			{
				constexpr int expected {0};
				int value {0};
				cmoon::net::io_context io;

				auto e = io.executor();
				cmoon::execution::execute(e, [&value] { value = 5; });
				io.stop();
				
				cmoon::test::assert_equal(io.run(), 0);
				cmoon::test::assert_true(io.stopped());
				cmoon::test::assert_equal(value, expected);

				io.restart();
				cmoon::test::assert_false(io.stopped());
				cmoon::test::assert_equal(io.run(), 0);
				cmoon::test::assert_true(io.stopped());

				cmoon::test::assert_equal(value, expected);

				io.restart();
				cmoon::test::assert_false(io.stopped());
				cmoon::execution::execute(e, []{});
				cmoon::execution::execute(e, []{});

				cmoon::test::assert_equal(io.run_one(), 1);
				cmoon::test::assert_false(io.stopped());
				cmoon::test::assert_equal(io.run_one(), 1);
				cmoon::test::assert_true(io.stopped());

				cmoon::test::assert_equal(io.run_one(), 0);
				cmoon::test::assert_true(io.stopped());
			}
	};
}
