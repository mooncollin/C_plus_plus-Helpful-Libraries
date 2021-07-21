export module cmoon.tests.benchmark;
export import cmoon.tests.benchmark.run_result;
export import cmoon.tests.benchmark.benchmark_result;
export import cmoon.tests.benchmark.text_benchmark_runner;

import <utility>;

import cmoon.test;

import cmoon.tests;

namespace cmoon::tests
{
	export
	template<>
	cmoon::test::test_suite get_test_suite<library::benchmark>()
	{
		cmoon::test::test_suite suite{"benchmark library tests"};
		suite.add_test_case<benchmark::run_result_test>();
		suite.add_test_case<benchmark::benchmark_result_test>();
		suite.add_test_case<benchmark::text_benchmark_runner_test>();

		return std::move(suite);
	}
}