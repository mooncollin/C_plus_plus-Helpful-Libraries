export module cmoon.tests.benchmarking.noop_benchmark;

import <sstream>;

import cmoon.test;
import cmoon.benchmarking;

namespace cmoon::tests::benchmarking
{
	export
	class noop_benchmark_test : public cmoon::test::test_case
	{
		public:
			noop_benchmark_test()
				: cmoon::test::test_case{"noop_benchmark_test"} {}

			void operator()() override
			{
				cmoon::benchmarking::benchmark_suite suite;
				suite.add_benchmark(cmoon::benchmarking::noop_benchmark{});

				std::stringstream ss;

				cmoon::benchmarking::text_benchmark_runner runner{ss};

				runner.run(suite);
			}
	};
}