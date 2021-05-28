#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>

#include "cmoon/cuda/unified_memory.hpp"
#include "cmoon/cuda/algorithm.hpp"

#include "cmoon/format.hpp"
#include "cmoon/benchmark/benchmark.hpp"
#include "cmoon/benchmark/runner.hpp"
#include "cmoon/benchmark/suite.hpp"

class cpu_add_benchmark : public cmoon::benchmark::benchmark
{
	public:
		cpu_add_benchmark(std::size_t size)
			: cmoon::benchmark::benchmark{cmoon::format("cpu_add_benchmark Size: {}", size)}, size{size} {}

		void set_up() override
		{
			x.resize(size);
			y.resize(size);

			std::fill(x.begin(), x.end(), 1.0f);
			std::fill(y.begin(), y.end(), 2.0f);
		}

		void tear_down() override
		{
			x.resize(0);
			y.resize(0);
		}

		void run() override
		{
			std::transform(std::begin(x), std::end(x), std::begin(y), std::begin(y), std::plus<>{});
		}
	private:
		std::size_t size;
		std::vector<float> x;
		std::vector<float> y;
};

class gpu_add_benchmark : public cmoon::benchmark::benchmark
{
	public:
		gpu_add_benchmark(std::size_t size)
			: cmoon::benchmark::benchmark{cmoon::format("gpu_add_benchmark Size: {}", size)}, size{size} {}

		void set_up() override
		{
			x.resize(size);
			y.resize(size);

			std::fill(x.begin(), x.end(), 1.0f);
			std::fill(y.begin(), y.end(), 2.0f);
		}

		void tear_down() override
		{
			x.resize(0);
			y.resize(0);
		}

		void run() override
		{
			cmoon::cuda::plus_ranges(x.size(), x.data(), y.data(), y.data());
			cudaDeviceSynchronize();
		}
	private:
		std::size_t size;
		std::vector<float, cmoon::cuda::unified_memory_allocator<float>> x;
		std::vector<float, cmoon::cuda::unified_memory_allocator<float>> y;
};

int main()
{
	cmoon::benchmark::benchmark_suite suite;
	suite.add_benchmark<cpu_add_benchmark>(10);
	suite.add_benchmark<gpu_add_benchmark>(10);
	suite.add_benchmark<cpu_add_benchmark>(100);
	suite.add_benchmark<gpu_add_benchmark>(100);
	suite.add_benchmark<cpu_add_benchmark>(1'000);
	suite.add_benchmark<gpu_add_benchmark>(1'000);
	suite.add_benchmark<cpu_add_benchmark>(10'000);
	suite.add_benchmark<gpu_add_benchmark>(10'000);
	suite.add_benchmark<cpu_add_benchmark>(100'000);
	suite.add_benchmark<gpu_add_benchmark>(100'000);
	suite.add_benchmark<cpu_add_benchmark>(1'000'000);
	suite.add_benchmark<gpu_add_benchmark>(1'000'000);
	suite.add_benchmark<cpu_add_benchmark>(10'000'000);
	suite.add_benchmark<gpu_add_benchmark>(10'000'000);

	cmoon::benchmark::text_benchmark_runner runner{std::cout};
	runner.run(suite);
}