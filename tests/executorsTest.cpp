#include <iostream>

#include "cmoon/executors/cached_thread_pool.hpp"
#include "cmoon/executors/fixed_thread_pool.hpp"
#include "cmoon/executors/single_thread_executor.hpp"
#include "cmoon/executors/timer.hpp"
#include "cmoon/executors/rejected_execution.hpp"
#include "cmoon/executors/cancelled_execution.hpp"
#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

class cached_thread_pool_default : public cmoon::test::test_case
{
	public:
		cached_thread_pool_default()
			: cmoon::test::test_case{"cached_thread_pool_default"} {}

		void operator()() override
		{
			cmoon::executors::cached_thread_pool pool;
		}
};

class cached_thread_pool_submit : public cmoon::test::test_case
{
	public:
		cached_thread_pool_submit()
			: cmoon::test::test_case{"cached_thread_pool_submit"} {}

		void operator()() override
		{
			cmoon::executors::cached_thread_pool pool;

			cmoon::test::assert_false(pool.is_shutdown());
			cmoon::test::assert_false(pool.is_terminated());

			auto f = pool.submit([]{ return 5; });

			cmoon::test::assert_equal(f.get(), 5);

			for (auto i = 0; i < 100; ++i)
			{
				pool.submit([] { std::this_thread::sleep_for(std::chrono::milliseconds{1}); });
			}

			cmoon::test::assert_false(pool.is_shutdown());
			cmoon::test::assert_false(pool.is_terminated());

			const auto r = pool.submit([]{ std::this_thread::sleep_for(std::chrono::seconds{1}); });
			pool.shutdown();
			cmoon::test::assert_true(pool.is_shutdown());
			r.wait();

			cmoon::test::assert_throws(cmoon::executors::rejected_execution{""}, [&pool] {
				pool.submit([]{ return 5; });
			});

			cmoon::test::assert_true(pool.await_termination());
		}
};

class fixed_thread_pool_submit : public cmoon::test::test_case
{
	public:
		fixed_thread_pool_submit()
			: cmoon::test::test_case{"fixed_thread_pool_submit"} {}

		void operator()() override
		{
			cmoon::executors::fixed_thread_pool pool{8};

			cmoon::test::assert_false(pool.is_shutdown());
			cmoon::test::assert_false(pool.is_terminated());

			auto f = pool.submit([]{ return 5; });

			cmoon::test::assert_equal(f.get(), 5);

			for (auto i = 0; i < 100; ++i)
			{
				pool.submit([] { std::this_thread::sleep_for(std::chrono::milliseconds{1}); });
			}

			cmoon::test::assert_false(pool.is_shutdown());
			cmoon::test::assert_false(pool.is_terminated());

			const auto r = pool.submit([]{ std::this_thread::sleep_for(std::chrono::seconds{1}); });
			pool.shutdown();
			cmoon::test::assert_true(pool.is_shutdown());
			r.wait();

			cmoon::test::assert_throws(cmoon::executors::rejected_execution{""}, [&pool] {
				pool.submit([]{ return 5; });
			});

			cmoon::test::assert_true(pool.await_termination());
		}
};

class single_thread_executor_submit : public cmoon::test::test_case
{
	public:
		single_thread_executor_submit()
			: cmoon::test::test_case{"single_thread_executor_submit"} {}

		void operator()() override
		{
			cmoon::executors::single_thread_executor ex;

			cmoon::test::assert_equal(ex.threads(), 1);
			cmoon::test::assert_false(ex.is_shutdown());
			cmoon::test::assert_false(ex.is_terminated());

			auto f = ex.submit([]{ return 5; });

			cmoon::test::assert_equal(f.get(), 5);

			for (auto i = 0; i < 100; ++i)
			{
				ex.submit([] { std::this_thread::sleep_for(std::chrono::milliseconds{1}); });
			}

			cmoon::test::assert_false(ex.is_shutdown());
			cmoon::test::assert_false(ex.is_terminated());

			const auto r = ex.submit([]{ std::this_thread::sleep_for(std::chrono::seconds{1}); });
			ex.shutdown();
			cmoon::test::assert_true(ex.is_shutdown());
			r.wait();

			cmoon::test::assert_throws(cmoon::executors::rejected_execution{""}, [&ex] {
				ex.submit([]{ return 5; });
			});

			cmoon::test::assert_true(ex.await_termination());
		}
};

class timer_submit_test : public cmoon::test::test_case
{
	public:
		timer_submit_test()
			: cmoon::test::test_case{"timer_submit_test"} {}

		void operator()() override
		{
			cmoon::executors::timer t;

			auto r = t.submit([]{ return 5; }, std::chrono::milliseconds{500});
			cmoon::test::assert_equal(r.future().wait_for(std::chrono::milliseconds{100}), std::future_status::timeout);
			cmoon::test::assert_equal(r.future().wait_for(std::chrono::milliseconds{500}), std::future_status::ready);
			cmoon::test::assert_equal(r.future().get(), 5);
		}
};

class timer_cancelled_test : public cmoon::test::test_case
{
	public:
		timer_cancelled_test()
			: cmoon::test::test_case{"timer_cancelled_test"} {}

		void operator()() override
		{
			cmoon::executors::timer t;

			auto r2 = t.submit([]{ return 5; }, std::chrono::seconds{5});
			cmoon::test::assert_true(r2.cancel());
			cmoon::test::assert_throws(cmoon::executors::cancelled_execution{""}, [&r2] {
				r2.future().get();
			});
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<cached_thread_pool_default>();
	suite.add_test_case<cached_thread_pool_submit>();
	suite.add_test_case<fixed_thread_pool_submit>();
	suite.add_test_case<single_thread_executor_submit>();
	suite.add_test_case<timer_submit_test>();
	suite.add_test_case<timer_cancelled_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}