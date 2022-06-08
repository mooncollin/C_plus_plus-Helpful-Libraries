export module cmoon.benchmarking.benchmark_result;

import <deque>;
import <chrono>;
import <cstddef>;
import <numeric>;
import <algorithm>;
import <ranges>;
import <functional>;

import cmoon.benchmarking.run_result;

namespace cmoon::benchmarking
{
	export
	class benchmark_result
	{
		public:
			void start_run()
			{
				runs_.emplace_back();
			}

			void add_iteration(const std::chrono::nanoseconds& i)
			{
				runs_.back().add_iteration(i);
			}

			[[nodiscard]] auto begin() const noexcept
			{
				return std::ranges::begin(runs_);
			}

			[[nodiscard]] auto end() const noexcept
			{
				return std::ranges::end(runs_);
			}

			[[nodiscard]] std::size_t size() const noexcept
			{
				return std::ranges::size(runs_);
			}

			[[nodiscard]] bool empty() const noexcept
			{
				return std::ranges::empty(runs_);
			}

			[[nodiscard]] std::size_t total_iterations() const noexcept
			{
				return std::transform_reduce(
					std::begin(runs_),
					std::end(runs_),
					std::size_t{0},
					std::plus{},
					std::ranges::size
				);
			}

			[[nodiscard]] const run_result& fastest_run() const noexcept
			{
				return *std::ranges::min_element(runs_,
												 std::less{},
												 [](const auto& run) {
													return run.total_time();
												 });
			}

			[[nodiscard]] const run_result& slowest_run() const noexcept
			{
				return *std::ranges::max_element(runs_,
												 std::less{},
												 [](const auto& run) {
													return run.total_time();
												 });
			}

			[[nodiscard]] std::chrono::nanoseconds fastest_iteration() const noexcept
			{
				return std::ranges::min_element(runs_,
												std::less{},
												[](const auto& run) {
													return run.fastest();
												})->fastest();
			}

			[[nodiscard]] std::chrono::nanoseconds slowest_iteration() const noexcept
			{
				return std::ranges::max_element(runs_,
												std::less{},
												[](const auto& run) {
													return run.slowest();
												})->slowest();
			}

			[[nodiscard]] std::chrono::nanoseconds average_run_time() const noexcept
			{
				return total_run_time() / size();
			}

			[[nodiscard]] std::chrono::nanoseconds average_iteration_time() const noexcept
			{
				return total_run_time() / total_iterations();
			}

			[[nodiscard]] std::chrono::nanoseconds total_run_time() const noexcept
			{
				return std::transform_reduce(
					std::begin(runs_),
					std::end(runs_),
					std::chrono::nanoseconds::zero(),
					std::plus{},
					[](const auto& run_r) {
						return run_r.total_time();
					}
				);
			}
		private:
			std::deque<run_result> runs_ {};
	};
}