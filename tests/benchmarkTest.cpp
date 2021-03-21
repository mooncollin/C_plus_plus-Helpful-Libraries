#include <iostream>
#include <chrono>

#include "cmoon/test/assert.hpp"
#include "cmoon/test/runner.hpp"
#include "cmoon/test/test_case.hpp"
#include "cmoon/test/test_suite.hpp"

#include "cmoon/benchmark/benchmark.hpp"
#include "cmoon/benchmark/runner.hpp"
#include "cmoon/benchmark/suite.hpp"

class run_result_test : public cmoon::test::test_case
{
	public:
		run_result_test()
			: cmoon::test::test_case{"run_result_test"} {}

		void operator()() override
		{
			cmoon::benchmark::run_result value;
			
			cmoon::test::assert_true(value.iterations().empty());
			cmoon::test::assert_equal(value.fastest(), std::chrono::nanoseconds{0});
			cmoon::test::assert_equal(value.slowest(), std::chrono::nanoseconds{0});
			cmoon::test::assert_equal(value.average(), std::chrono::nanoseconds{0});
			cmoon::test::assert_equal(value.total_time(), std::chrono::nanoseconds{0});

			value.add_iteration(std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.iterations().size(), 1);
			cmoon::test::assert_equal(value.fastest(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.slowest(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.average(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.total_time(), std::chrono::nanoseconds{5});

			value.add_iteration(std::chrono::nanoseconds{15});
			cmoon::test::assert_equal(value.iterations().size(), 2);
			cmoon::test::assert_equal(value.fastest(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.slowest(), std::chrono::nanoseconds{15});
			cmoon::test::assert_equal(value.average(), std::chrono::nanoseconds{10});
			cmoon::test::assert_equal(value.total_time(), std::chrono::nanoseconds{20});
		}
};

class benchmark_result_test : public cmoon::test::test_case
{
	public:
		benchmark_result_test()
			: cmoon::test::test_case{"benchmark_result_test"} {}

		void operator()() override
		{
			cmoon::benchmark::benchmark_result value;
			
			cmoon::test::assert_true(value.runs().empty());

			value.start_run();
			cmoon::test::assert_equal(value.runs().size(), 1);
			cmoon::test::assert_equal(value.fastest_run().total_time(), std::chrono::nanoseconds{0});
			cmoon::test::assert_equal(value.slowest_run().total_time(), std::chrono::nanoseconds{0});
			cmoon::test::assert_equal(value.average_run_time(), std::chrono::nanoseconds{0});
			cmoon::test::assert_equal(value.total_run_time(), std::chrono::nanoseconds{0});

			value.add_iteration(std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.runs().size(), 1);
			cmoon::test::assert_equal(value.fastest_run().total_time(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.slowest_run().total_time(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.average_run_time(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.total_run_time(), std::chrono::nanoseconds{5});

			value.add_iteration(std::chrono::nanoseconds{15});
			cmoon::test::assert_equal(value.fastest_run().total_time(), std::chrono::nanoseconds{20});
			cmoon::test::assert_equal(value.slowest_run().total_time(), std::chrono::nanoseconds{20});
			cmoon::test::assert_equal(value.average_run_time(), std::chrono::nanoseconds{20});
			cmoon::test::assert_equal(value.total_run_time(), std::chrono::nanoseconds{20});

			value.start_run();
			cmoon::test::assert_equal(value.runs().size(), 2);

			value.add_iteration(std::chrono::nanoseconds{7});
			cmoon::test::assert_equal(value.runs().size(), 2);
			cmoon::test::assert_equal(value.fastest_run().total_time(), std::chrono::nanoseconds{7});
			cmoon::test::assert_equal(value.slowest_run().total_time(), std::chrono::nanoseconds{20});
			cmoon::test::assert_equal(value.average_run_time(), std::chrono::nanoseconds{13});
			cmoon::test::assert_equal(value.total_run_time(), std::chrono::nanoseconds{27});

			cmoon::test::assert_equal(value.fastest_run().fastest(), std::chrono::nanoseconds{7});
			cmoon::test::assert_equal(value.fastest_run().slowest(), std::chrono::nanoseconds{7});
			cmoon::test::assert_equal(value.slowest_run().fastest(), std::chrono::nanoseconds{5});
			cmoon::test::assert_equal(value.slowest_run().slowest(), std::chrono::nanoseconds{15});
		}
};

class benchmark_text_runner_test : public cmoon::test::test_case
{
	public:
		benchmark_text_runner_test()
			: cmoon::test::test_case{"benchmark_text_runner_test"} {}

		void operator()() override
		{
			std::cout << "\n";
			cmoon::benchmark::text_benchmark_runner runner {std::cout};
			cmoon::benchmark::benchmark b{"test"};
			runner.run(b);
			std::cout << "\n";
		}
};

int main()
{
	cmoon::test::test_suite suite;
	suite.add_test_case<run_result_test>();
	suite.add_test_case<benchmark_result_test>();
	suite.add_test_case<benchmark_text_runner_test>();

	cmoon::test::text_test_runner runner{std::cout};

	return !runner.run(suite);
}