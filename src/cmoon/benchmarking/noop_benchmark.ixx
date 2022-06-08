export module cmoon.benchmarking.noop_benchmark;

import <cstddef>;

import cmoon.benchmarking.benchmark;
import cmoon.benchmarking.do_not_optimize;

namespace cmoon::benchmarking
{
	export
	class noop_benchmark : public benchmark
	{
		public:
			noop_benchmark(std::size_t runs = 10, std::size_t iterations = 100)
				: benchmark{"cmoon::benchmarking::noop", runs, iterations} {}

			void operator()() final
			{
				do_not_optimize(0);
			}
	};
}