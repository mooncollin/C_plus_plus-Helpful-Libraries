export module cmoon.benchmarking.benchmark_suite;

import <deque>;
import <memory>;
import <utility>;
import <type_traits>;
import <cstddef>;
import <ranges>;
import <algorithm>;

import cmoon.benchmarking.benchmark;

namespace cmoon::benchmarking
{
	class benchmark_suite;

	template<class It>
	class benchmark_suite_iterator
	{
		public:
			using difference_type = std::iter_difference_t<It>;
			using value_type = benchmark*;
			using iterator_category = std::forward_iterator_tag;

			benchmark_suite_iterator() noexcept = default;

			value_type operator*() const noexcept
			{
				return ptr->get();
			}

			benchmark_suite_iterator& operator++()
			{
				++ptr;
				return *this;
			}

			benchmark_suite_iterator operator++(int)
			{
				auto copy {*this};
				++ptr;
				return copy;
			}

			[[nodiscard]] friend bool operator==(const benchmark_suite_iterator&, const benchmark_suite_iterator&) noexcept = default;
			[[nodiscard]] friend bool operator!=(const benchmark_suite_iterator&, const benchmark_suite_iterator&) noexcept = default;
		private:
			benchmark_suite_iterator(It ptr) noexcept
				: ptr{ptr} {}

			It ptr;

			friend class benchmark_suite;
	};

	export
	class benchmark_suite
	{
		public:
			template<std::derived_from<benchmark> T, class... Args>
			void add_benchmark(Args&&... args)
			{
				benchmarks_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
			}

			template<std::derived_from<benchmark> T>
			void add_benchmark(T&& bench)
			{
				benchmarks_.push_back(std::make_unique<T>(std::move(bench)));
			}

			void add_benchmark_suite(benchmark_suite&& suite)
			{
				std::ranges::move(suite.benchmarks_,
								  std::back_inserter(benchmarks_));
				suite.clear();
			}

			template<std::output_iterator<benchmark_result> Out>
			void run_all(Out out)
			{
				std::ranges::transform(benchmarks_,
									   out,
									   run_benchmark);
			}

			[[nodiscard]] auto begin() const noexcept
			{
				return benchmark_suite_iterator{std::ranges::begin(benchmarks_)};
			}

			[[nodiscard]] auto end() const noexcept
			{
				return benchmark_suite_iterator{std::ranges::end(benchmarks_)};
			}

			[[nodiscard]] std::size_t size() const noexcept
			{
				return std::ranges::size(benchmarks_);
			}

			void clear()
			{
				benchmarks_.clear();
			}
		private:
			std::deque<std::unique_ptr<benchmark>> benchmarks_;
	};
}