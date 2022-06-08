export module cmoon.benchmarking.benchmark;

import <cstddef>;
import <algorithm>;
import <chrono>;
import <string>;
import <utility>;

import cmoon.benchmarking.benchmark_result;

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
					const auto start_baseline {std::chrono::high_resolution_clock::now()};
					const auto end_baseline {std::chrono::high_resolution_clock::now() - start_baseline};
					const auto start {std::chrono::high_resolution_clock::now()};
					bench();
					const auto end {std::chrono::high_resolution_clock::now() - start};
					const auto benchmark_time_elapsed {std::chrono::duration_cast<std::chrono::nanoseconds>(end - end_baseline)};

					results.add_iteration(std::max(std::chrono::nanoseconds{0}, benchmark_time_elapsed));
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