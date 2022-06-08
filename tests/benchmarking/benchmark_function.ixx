export module cmoon.tests.benchmarking.benchmark_function;

import <cstdlib>;
import <cmath>;
import <sstream>;

import cmoon.test;
import cmoon.benchmarking;

namespace cmoon::tests::benchmarking
{
	export
	class benchmark_function_test : public cmoon::test::test_case
	{
		public:
			benchmark_function_test()
				: cmoon::test::test_case{"benchmark_function_test"} {}

			void operator()() override
			{
				cmoon::benchmarking::benchmark_suite suite;
				suite.add_benchmark(cmoon::benchmarking::benchmark_function{[]{
					const auto r {std::exp(rand())};
					cmoon::benchmarking::do_not_optimize(r);
				}});

				std::stringstream ss;

				cmoon::benchmarking::text_benchmark_runner runner{ss};

				runner.run(suite);
			}
	};
}