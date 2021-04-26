export module cmoon.benchmark.benchmark_c;

import <cstddef>;
import <string_view>;
import <algorithm>;
import <chrono>;
import <string>;

import cmoon.benchmark.benchmark_result;

namespace cmoon::benchmark
{
	export
	class benchmark
	{
		public:
			benchmark(std::string_view str, std::size_t runs = 10, std::size_t iterations = 100)
				: name_{str}, runs_{runs}, iterations_per_run_{iterations} {}

			virtual void set_up() {}
			virtual void tear_down() {}
			inline virtual void run() {}
			virtual ~benchmark() = default;

			[[nodiscard]] benchmark_result do_benchmark()
			{
				benchmark_result results;
				for (std::size_t run{0}; run < runs_; ++run)
				{
					results.start_run();
					set_up();
					for (std::size_t iteration{0}; iteration < iterations_per_run_; ++iteration)
					{
						const auto start_baseline = std::chrono::high_resolution_clock::now();
						const auto end_baseline = std::chrono::high_resolution_clock::now() - start_baseline;
						const auto start = std::chrono::high_resolution_clock::now();
						this->run();
						const auto end = std::chrono::high_resolution_clock::now() - start;
						const auto benchmark_time_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - end_baseline);

						results.add_iteration(std::max(std::chrono::nanoseconds{0}, benchmark_time_elapsed));
					}
					tear_down();
				}

				return results;
			}

			[[nodiscard]] const std::string& name() const noexcept
			{
				return name_;
			}

			void name(std::string_view str)
			{
				name_ = str;
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
}