export module cmoon.benchmark.runner;

import cmoon.benchmark.benchmark_c;
import cmoon.benchmark.suite;

namespace cmoon::benchmark
{
	export
	class benchmark_runner
	{
		public:
			benchmark_runner() = default;

			virtual ~benchmark_runner() noexcept = default;

			virtual void run(benchmark&) = 0;
			virtual void run(benchmark_suite&) = 0;
	};
}