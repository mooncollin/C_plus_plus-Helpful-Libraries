#pragma once

#include <cstddef>
#include <concepts>
#include <tuple>
#include <vector>
#include <chrono>
#include <ranges>
#include <numeric>
#include <algorithm>
#include <functional>
#include <tuple>
#include <string_view>
#include <string>

namespace cmoon
{
	namespace benchmark
	{
		class run_result
		{
			public:
				run_result() noexcept = default;

				[[nodiscard]] const std::vector<std::chrono::nanoseconds>& iterations() const noexcept
				{
					return iterations_;
				}

				[[nodiscard]] std::size_t amount_iterations() const noexcept
				{
					return iterations_.size();
				}

				[[nodiscard]] std::chrono::nanoseconds fastest() const noexcept
				{
					return fastest_;
				}

				[[nodiscard]] std::chrono::nanoseconds slowest() const noexcept
				{
					return slowest_;
				}

				[[nodiscard]] std::chrono::nanoseconds average() const noexcept
				{
					return average_;
				}

				void add_iteration(const std::chrono::nanoseconds& i)
				{
					iterations_.push_back(i);
					if (std::size(iterations_) == 1)
					{
						fastest_ = i;
						slowest_ = i;
					}
					else if (i < fastest_)
					{
						fastest_ = i;
					}
					else if (i > slowest_)
					{
						slowest_ = i;
					}

					running_sum += i;
					average_ = running_sum / std::size(iterations_);
				}

				[[nodiscard]] std::chrono::nanoseconds total_time() const noexcept
				{
					return running_sum;
				}
			private:
				std::vector<std::chrono::nanoseconds> iterations_;
				std::chrono::nanoseconds fastest_ {std::chrono::nanoseconds::zero()};
				std::chrono::nanoseconds slowest_ {std::chrono::nanoseconds::zero()};
				std::chrono::nanoseconds average_ {std::chrono::nanoseconds::zero()};

				std::chrono::nanoseconds running_sum {std::chrono::nanoseconds::zero()};
		};

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
}