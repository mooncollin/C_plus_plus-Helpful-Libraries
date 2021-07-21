export module cmoon.tests.benchmark.text_benchmark_runner;

import <iostream>;

import cmoon.test;
import cmoon.benchmark;

namespace cmoon::tests::benchmark
{
	export
	class text_benchmark_runner_test : public cmoon::test::test_case
	{
		public:
			text_benchmark_runner_test()
				: cmoon::test::test_case{"text_benchmark_runner_test"} {}

			void operator()() override
			{
				std::cout << "\n";
				cmoon::benchmark::text_benchmark_runner runner {std::cout};
				cmoon::benchmark::benchmark b{"test"};
				runner.run(b);
				std::cout << "\n";
			}
	};
}