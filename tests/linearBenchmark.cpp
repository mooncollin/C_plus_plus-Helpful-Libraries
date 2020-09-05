#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>

#include "collin/linear/matrix.hpp"
#include "collin/linear/algorithms.hpp"

template<class Rep, class Period>
void print_time(const std::chrono::duration<Rep, Period>& c)
{
	const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(c);
	const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(c);
	const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(c);
	if (seconds.count() > 0)
	{
		std::cout << (static_cast<long double>(seconds.count()) + (static_cast<long double>(milliseconds.count()) / static_cast<long double>(std::milli::den)));
		std::cout << "s";
	}
	else if (milliseconds.count() > 0)
	{
		std::cout << (static_cast<long double>(milliseconds.count()) + (static_cast<long double>(nanoseconds.count()) / static_cast<long double>(std::nano::den)));
		std::cout << "ms";
	}
	else
	{
		std::cout << nanoseconds.count();
		std::cout << "ns";
	}
}

template<class T>
void fill_matrix(collin::linear::matrix<T>& m)
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

template<class T, std::size_t Rows, std::size_t Cols>
void fill_matrix(collin::linear::fixed_matrix<T, Rows, Cols>& m)
{
	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::conditional_t<std::is_integral_v<T>,
		std::uniform_int_distribution<T>,
		std::uniform_real_distribution<T>> d;

	for (auto& v : m)
	{
		v = d(gen);
	}
}

template<class T>
void benchmark_multiplication_matrix(std::size_t matrix_size)
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<> d;

	collin::linear::matrix<T> m1{matrix_size, matrix_size};
	collin::linear::matrix<T> m2{matrix_size, matrix_size};

	fill_matrix(m1);
	fill_matrix(m2);

	std::cout << "Multiplication benchmark for a matrix of size " << matrix_size << ":\n";

	const auto time_now = std::chrono::system_clock::now();
	m1 * m2;
	const auto elapsed_time = std::chrono::system_clock::now() - time_now;
	std::cout << "Time Elapsed: ";
	print_time(elapsed_time);
	std::cout << '\n';
}

template<class T, std::size_t Size>
void benchmark_multiplication_fixed_matrix()
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<> d;

	collin::linear::fixed_matrix<T, Size, Size> m1;
	collin::linear::fixed_matrix<T, Size, Size> m2;

	fill_matrix(m1);
	fill_matrix(m2);

	std::cout << "Multiplication benchmark for a matrix of size " << Size << ":\n";

	const auto time_now = std::chrono::system_clock::now();
	m1 * m2;
	const auto elapsed_time = std::chrono::system_clock::now() - time_now;
	std::cout << "Time Elapsed: ";
	print_time(elapsed_time);
	std::cout << '\n';
}

template<class T>
void benchmark_inverse_matrix(std::size_t matrix_size)
{
	std::random_device rd;
	std::mt19937 gen{rd()};
	std::uniform_int_distribution<> d;

	collin::linear::matrix<T> m1{matrix_size, matrix_size};

	fill_matrix(m1);

	std::cout << "Inverse benchmark for a matrix of size " << matrix_size << ":\n";

	const auto time_now = std::chrono::system_clock::now();
	collin::linear::inverse(m1);
	const auto elapsed_time = std::chrono::system_clock::now() - time_now;
	std::cout << "Time Elapsed: ";
	print_time(elapsed_time);
	std::cout << '\n';
}

int main()
{
	constexpr std::array sizes = {10, 20, 30, 40, 50, 60, 70, 80, 90,
					 100, 200, 300, 400, 500, 600, 700, 800, 900,
					 1000, 1500, 2000, 2500, 3000, 3500, 4000};

	//benchmark_multiplication_fixed_matrix<int, 10>();
	//benchmark_multiplication_fixed_matrix<int, 20>();
	//benchmark_multiplication_fixed_matrix<int, 30>();
	//benchmark_multiplication_fixed_matrix<int, 40>();
	//benchmark_multiplication_fixed_matrix<int, 50>();
	//benchmark_multiplication_fixed_matrix<int, 60>();
	//benchmark_multiplication_fixed_matrix<int, 70>();
	//benchmark_multiplication_fixed_matrix<int, 80>();
	//benchmark_multiplication_fixed_matrix<int, 90>();
	//benchmark_multiplication_fixed_matrix<int, 100>();

	for(auto size : sizes)
	{
		benchmark_multiplication_matrix<int>(size);
		//benchmark_inverse_matrix<int>(size);
	}
}