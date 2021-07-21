export module cmoon.tests.benchmark.benchmark_result;

import <chrono>;

import cmoon.test;
import cmoon.benchmark;

namespace cmoon::tests::benchmark
{
	export
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
}