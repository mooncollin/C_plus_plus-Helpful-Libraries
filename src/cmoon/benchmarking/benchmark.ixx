export module cmoon.benchmarking.benchmark;

import <cstddef>;
import <algorithm>;
import <chrono>;
import <string>;
import <utility>;

import cmoon.benchmarking.benchmark_result;
import cmoon.utility;

namespace cmoon::benchmarking
{
	export
	class benchmark
	{
		public:
			benchmark(std::string str, std::size_t runs = 10, std::size_t iterations = 10000) noexcept
				: name_{std::move(str)}, runs_{runs}, iterations_per_run_{iterations} {}

			virtual void set_up() {}
			virtual void tear_down() {}
			virtual void operator()() = 0;
			virtual ~benchmark() noexcept = default;

			[[nodiscard]] const std::string& name() const noexcept
			{
				return name_;
			}

			[[nodiscard]] std::size_t runs() const noexcept
			{
				return runs_;
			}

			[[nodiscard]] std::size_t iterations_per_run() const noexcept
			{
				return iterations_per_run_;
			}
		private:
			std::string name_;
			std::size_t runs_;
			std::size_t iterations_per_run_;
	};

	export
	constexpr struct run_benchmark_fn
	{
		benchmark_result operator()(benchmark& bench) const
		{
			benchmark_result results;
			for (std::size_t run{0}; run < bench.runs(); ++run)
			{
				bench.set_up();
				results.start_run();
				for (std::size_t iteration{0}; iteration < bench.iterations_per_run(); ++iteration)
				{
					cmoon::stopwatch stopwatch;
					bench();
					const auto benchmark_duration {stopwatch.get_elapsed_time()};

					results.add_iteration(benchmark_duration);
				}
				bench.tear_down();
			}

			return results;
		}

		benchmark_result operator()(benchmark&& bench) const
		{
			return operator()(bench);
		}
	} run_benchmark {};
}