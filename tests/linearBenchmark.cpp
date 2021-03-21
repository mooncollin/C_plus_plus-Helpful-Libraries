#include <iostream>
#include <fstream>
#include <random>
#include <string>

#include "cmoon/format.hpp"
#include "cmoon/linear/matrix.hpp"
#include "cmoon/linear/algorithms.hpp"
#include "cmoon/cuda/unified_memory.hpp"
#include "cmoon/cuda/algorithm.hpp"
#include "cmoon/benchmark/benchmark.hpp"
#include "cmoon/benchmark/runner.hpp"
#include "cmoon/benchmark/suite.hpp"

template<class T, class Allocator>
void fill_matrix(cmoon::linear::matrix<T, Allocator>& m)
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::conditional_t<std::is_integral_v<T>, 
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>> d;

	for (auto& v : m)
	{
		v = d(gen);
	}
}

template<class T, std::size_t S>
void fill_matrix(cmoon::linear::square_matrix<T, S>& m)
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::conditional_t<std::is_integral_v<T>, 
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>> d;

	for (auto& v : m)
	{
		v = d(gen);
	}
}

//template<class T>
//void benchmark_inverse_matrix(std::size_t matrix_size)
//{
//	std::random_device rd;
//	std::mt19937 gen{rd()};
//	std::uniform_int_distribution<> d;
//
//	cmoon::linear::matrix<T> m1{matrix_size, matrix_size};
//
//	fill_matrix(m1);
//
//	std::cout << "Inverse benchmark for a matrix of size " << matrix_size << ":\n";
//
//	const auto time_now = std::chrono::system_clock::now();
//	cmoon::linear::inverse(m1);
//	const auto elapsed_time = std::chrono::system_clock::now() - time_now;
//	std::cout << "Time Elapsed: ";
//	print_time(elapsed_time);
//	std::cout << '\n';
//}

template<class T>
class matrix_multiplication_benchmark : public cmoon::benchmark::benchmark
{
	public:
		matrix_multiplication_benchmark(std::size_t size)
			: cmoon::benchmark::benchmark{cmoon::format("Matrix Multiplication ({0:}). Size: {1:}x{1:}", typeid(T).name(), size), 1, 5}, size{size} {}

		virtual void set_up() override
		{
			m1.resize(size, size);
			m2.resize(size, size);
			fill_matrix(m1);
			fill_matrix(m2);
		}

		virtual void tear_down() override
		{
			// Release space between runs
			m1.resize(0, 0);
			m2.resize(0, 0);
		}

		inline virtual void run() override
		{
			const auto m3 = m1 * m2;
		}
	private:
		std::size_t size;
		cmoon::linear::matrix<T> m1;
		cmoon::linear::matrix<T> m2;
};

template<class T, std::size_t S>
class fixed_matrix_multiplication_benchmark : public cmoon::benchmark::benchmark
{
	public:
		fixed_matrix_multiplication_benchmark()
			: cmoon::benchmark::benchmark{cmoon::format("Fixed Matrix Multiplication ({0:}). Size: {1:}x{1:}", typeid(T).name(), S), 1, 5} {}

		virtual void set_up() override
		{
			fill_matrix(m1);
			fill_matrix(m2);
		}

		inline virtual void run() override
		{
			const auto m3 = m1 * m2;
		}
	private:
		cmoon::linear::square_matrix<T, S> m1;
		cmoon::linear::square_matrix<T, S> m2;
};

template<class T>
class gpu_matrix_multiplication_benchmark : public cmoon::benchmark::benchmark
{
	public:
		gpu_matrix_multiplication_benchmark(std::size_t size)
			: cmoon::benchmark::benchmark{cmoon::format("Matrix Multiplication ({0:}) with GPU. Size: {1:}x{1:}", typeid(T).name(), size), 1, 20}, size{size} {}

		virtual void set_up() override
		{
			m1.resize(size, size);
			m2.resize(size, size);
			fill_matrix(m1);
			fill_matrix(m2);
		}

		virtual void tear_down() override
		{
			// Release space between runs
			m1.resize(0, 0);
			m2.resize(0, 0);
		}

		inline virtual void run() override
		{
			const auto m3 = cmoon::cuda::matrix_multiply(m1, m2);
		}
	private:
		std::size_t size;
		cmoon::linear::matrix<T, cmoon::cuda::unified_memory_allocator<T>> m1;
		cmoon::linear::matrix<T, cmoon::cuda::unified_memory_allocator<T>> m2;
};

int main()
{
	cmoon::benchmark::benchmark_suite cpu_suite;
	cmoon::benchmark::benchmark_suite cpu_power2_suite;
	cmoon::benchmark::benchmark_suite cpu_power2_fixed_suite;
	cmoon::benchmark::benchmark_suite gpu_suite;
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(10);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(20);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(30);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(40);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(50);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(60);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(70);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(80);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(90);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(100);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(200);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(300);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(400);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(500);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(600);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(700);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(800);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(900);
	cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(1000);
	//cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(1500);
	//cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(2000);
	//cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(2500);
	//cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(3000);
	//cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(3500);
	//cpu_suite.add_benchmark<matrix_multiplication_benchmark<int>>(4000);

	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(1);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(2);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(4);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(8);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(16);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(32);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(64);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(128);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(256);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(512);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(1024);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(2048);
	cpu_power2_suite.add_benchmark<matrix_multiplication_benchmark<int>>(4096);

	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 1>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 2>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 4>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 8>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 16>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 32>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 64>>();
	cpu_power2_fixed_suite.add_benchmark<fixed_matrix_multiplication_benchmark<int, 128>>();

	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(10);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(20);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(30);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(40);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(50);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(60);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(70);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(80);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(90);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(100);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(200);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(300);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(400);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(500);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(600);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(700);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(800);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(900);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(1000);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(1500);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(2000);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(2500);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(3000);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(3500);
	gpu_suite.add_benchmark<gpu_matrix_multiplication_benchmark<int>>(4000);

	std::ofstream cpu_benchmarks {"cpu_matrix_multiplication.txt"};
	std::ofstream gpu_benchmarks {"gpu_matrix_multiplication.txt"};

	cmoon::benchmark::text_benchmark_runner cpu_runner{std::cout};
	cmoon::benchmark::text_benchmark_runner cpu_power2_runner {cpu_benchmarks};
	cmoon::benchmark::text_benchmark_runner gpu_runner{std::cout};

	//cpu_runner.run(cpu_suite);
	//cpu_power2_runner.run(cpu_power2_suite);
	//cpu_power2_runner.run(cpu_power2_fixed_suite);
	gpu_runner.run(gpu_suite);
}