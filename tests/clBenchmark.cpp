#include <memory>
#include <iostream>
#include <vector>
#include <string_view>
#include <array>
#include <algorithm>
#include <functional>

#include <CL/cl.hpp>

#include "cmoon/format.hpp"
#include "cmoon/benchmark/benchmark.hpp"
#include "cmoon/benchmark/runner.hpp"
#include "cmoon/benchmark/suite.hpp"

class cpu_add_benchmark : public cmoon::benchmark::benchmark
{
	public:
		cpu_add_benchmark(std::size_t s)
			: cmoon::benchmark::benchmark{cmoon::format("cpu_add_benchmark Size: {}", s)}, size{s} {}

		void set_up() override
		{
			a.resize(size);
			b.resize(size);
			c.resize(size);

			std::fill(std::begin(a), std::end(a), 1);
			std::fill(std::begin(b), std::end(b), 2);
		}

		void run() override
		{
			std::transform(std::begin(a), std::end(a), std::begin(b), std::begin(c), std::plus<>{});
		}

		void tear_down() override
		{
			a.resize(0);
			b.resize(0);
			c.resize(0);
		}
	private:
		std::size_t size;
		std::vector<int> a;
		std::vector<int> b;
		std::vector<int> c;
};

class gpu_add_benchmark : public cmoon::benchmark::benchmark
{
	public:
		gpu_add_benchmark(std::size_t s)
			: cmoon::benchmark::benchmark{cmoon::format("gpu_add_benchmark Size: {}", s)}, size{s} {}

		void set_up() override
		{
			a.resize(size);
			b.resize(size);
			c.resize(size);

			std::fill(std::begin(a), std::end(a), 1);
			std::fill(std::begin(b), std::end(b), 2);

			cl_int err{CL_SUCCESS};
			const auto platform = cl::Platform::get(std::addressof(err));
			if (err != CL_SUCCESS)
			{
				throw std::runtime_error{cmoon::format("Error getting platform. Error code: {}", err)};
			}

			if (platform.getDevices(CL_DEVICE_TYPE_ALL, std::addressof(devices)) != CL_SUCCESS)
			{
				throw std::runtime_error{"No OpenCL devices found"};
			}

			
			context = {devices};
			cl::Program::Sources sources;
			sources.emplace_back(add_cl.data(), add_cl.size());

			auto& device = devices.front();
			cl::Program program{context, sources};
			if (program.build(devices) != CL_SUCCESS)
			{
				throw std::runtime_error{cmoon::format("Error building kernel code: {}", program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device))};
			}

			simple_add = {program, "simple_add"};

			buffer_A = {context, CL_MEM_READ_WRITE, sizeof(int) * a.size()};
			buffer_B = {context, CL_MEM_READ_WRITE, sizeof(int) * b.size()};
			buffer_C = {context, CL_MEM_READ_WRITE, sizeof(int) * c.size()};

			cl::CommandQueue queue{context, devices.front()};
			queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, a.size() * sizeof(int), a.data());
			queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, b.size() * sizeof(int), b.data());
		}

		void tear_down() override
		{
			a.resize(0);
			b.resize(0);
			c.resize(0);
		}

		void run() override
		{
			cl::CommandQueue queue{context, devices.front()};

			simple_add.setArg(0, buffer_A);
			simple_add.setArg(1, buffer_B);
			simple_add.setArg(2, buffer_C);

			queue.enqueueNDRangeKernel(simple_add, cl::NullRange, cl::NDRange{a.size()}, cl::NullRange);
			queue.finish();

			queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, c.size() * sizeof(int), c.data());
		}

	private:
		std::size_t size;
		std::vector<int> a;
		std::vector<int> b;
		std::vector<int> c;

		std::vector<cl::Device> devices;
		cl::Kernel simple_add;
		cl::Context context;
		cl::Buffer buffer_A;
		cl::Buffer buffer_B;
		cl::Buffer buffer_C;

		static constexpr std::string_view add_cl = R"(
			__kernel void simple_add(__global const int* A, __global const int* B, __global int* C) {

				// Get the index of the current element to be processed
				int i = get_global_id(0);

				// Do the operation
				C[i] = A[i] + B[i];
			}
			)";
};

int main()
{
	cmoon::benchmark::benchmark_suite suite;
	suite.add_benchmark<cpu_add_benchmark>(10);
	suite.add_benchmark<gpu_add_benchmark>(10);
	suite.add_benchmark<cpu_add_benchmark>(1'000'000);
	suite.add_benchmark<gpu_add_benchmark>(1'000'000);

	cmoon::benchmark::text_benchmark_runner runner{std::cout};

	runner.run(suite);
}