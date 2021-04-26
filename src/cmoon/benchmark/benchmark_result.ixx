export module cmoon.benchmark.benchmark_result;

import <vector>;
import <chrono>;
import <cstddef>;
import <numeric>;
import <algorithm>;

import cmoon.benchmark.run_result;

namespace cmoon::benchmark
{
	export
	class benchmark_result
	{
		using container_t = std::vector<run_result>;
		using container_iterator = typename container_t::const_iterator;
		public:
			benchmark_result() noexcept = default;

			void start_run()
			{
				runs_.emplace_back();
			}

			void add_iteration(const std::chrono::nanoseconds& i)
			{
				runs_.back().add_iteration(i);
			}

			[[nodiscard]] const container_t& runs() const noexcept
			{
				return runs_;
			}

			[[nodiscard]] std::size_t amount_runs() const noexcept
			{
				return runs_.size();
			}

			[[nodiscard]] std::size_t amount_iterations() const noexcept
			{
				return std::reduce(std::begin(runs_), std::end(runs_),
					std::size_t{0},
					[](const auto& sum, const auto& next) {
						return sum + next.amount_iterations();
					});
			}

			[[nodiscard]] const run_result& fastest_run() const noexcept
			{
				return *std::min_element(std::begin(runs_), std::end(runs_),
					[](const run_result& run1, const run_result& run2) {
						return run1.total_time() < run2.total_time();
					});
			}

			[[nodiscard]] const std::chrono::nanoseconds fastest_iteration() const noexcept
			{
				return std::min_element(std::begin(runs_), std::end(runs_),
					[](const run_result& run1, const run_result& run2) {
						return run1.fastest() < run2.fastest();
					})->fastest();
			}

			[[nodiscard]] const std::chrono::nanoseconds slowest_iteration() const noexcept
			{
				return std::max_element(std::begin(runs_), std::end(runs_),
					[](const run_result& run1, const run_result& run2) {
						return run1.slowest() < run2.slowest();
					})->slowest();
			}

			[[nodiscard]] const run_result& slowest_run() const noexcept
			{
				return *std::max_element(std::begin(runs_), std::end(runs_),
					[](const run_result& run1, const run_result& run2) {
						return run1.total_time() < run2.total_time();
					});
			}

			[[nodiscard]] std::chrono::nanoseconds average_run_time() const noexcept
			{
				return total_run_time() / amount_runs();
			}

			[[nodiscard]] std::chrono::nanoseconds average_iteration_time() const noexcept
			{
				return total_run_time() / amount_iterations();
			}

			[[nodiscard]] std::chrono::nanoseconds total_run_time() const noexcept
			{
				return std::reduce(std::begin(runs_), std::end(runs_),
					std::chrono::nanoseconds::zero(),
					[](const auto& sum, const auto& next) {
						return sum + next.total_time();
					});
			}
		private:
			container_t runs_ {};
	};
}