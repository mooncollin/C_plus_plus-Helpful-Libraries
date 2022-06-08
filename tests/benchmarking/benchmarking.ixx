export module cmoon.tests.benchmarking;
export import cmoon.tests.benchmarking.run_result;
export import cmoon.tests.benchmarking.benchmark_result;
export import cmoon.tests.benchmarking.benchmark_function;
export import cmoon.tests.benchmarking.noop_benchmark;

import <utility>;

import cmoon.test;
import cmoon.library;

namespace cmoon
{
	export
	template<>
	struct library_traits<library::benchmarking>
	{
		static cmoon::test::test_suite tests()
		{
			cmoon::test::test_suite suite{"benchmarking library tests"};
			suite.add_test_case<cmoon::tests::benchmarking::run_result_test>();
			suite.add_test_case<cmoon::tests::benchmarking::benchmark_result_test>();
			suite.add_test_case<cmoon::tests::benchmarking::benchmark_function_test>();
			suite.add_test_case<cmoon::tests::benchmarking::noop_benchmark_test>();

			return std::move(suite);
		}
	};
}