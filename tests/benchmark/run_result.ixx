export module cmoon.tests.benchmark.run_result;

import <chrono>;

import cmoon.test;
import cmoon.benchmark;

namespace cmoon::tests::benchmark
{
	export
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
}