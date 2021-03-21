#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "cmoon/benchmark/benchmark.hpp"

namespace cmoon
{
	namespace benchmark
	{
		class benchmark_suite
		{
			using container = std::vector<std::unique_ptr<benchmark>>;
			using iterator = typename container::iterator;

			public:
				template<std::derived_from<benchmark> T, class... Args>
				void add_benchmark(Args&&... args)
				{
					benchmarks_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
				}

				void add_benchmark_suite(benchmark_suite&& suite)
				{
					for (auto& bench : suite.benchmarks_)
					{
						benchmarks_.push_back(std::move(bench));
					}

					suite.clear();
				}

				std::vector<benchmark_result> run_all()
				{
					std::vector<benchmark_result> results;
					results.reserve(benchmarks_.size());
					for (auto& bench : benchmarks_)
					{
						results.push_back(bench->do_benchmark());
					}

					return results;
				}

				iterator begin()
				{
					return std::begin(benchmarks_);
				}

				iterator end()
				{
					return std::end(benchmarks_);
				}

				[[nodiscard]] const container& benchmarks() const noexcept
				{
					return benchmarks_;
				}

				[[nodiscard]] std::size_t num_benchmarks() const noexcept
				{
					return benchmarks_.size();
				}

				void clear()
				{
					benchmarks_.clear();
				}
			private:
				container benchmarks_;
		};
	}
}