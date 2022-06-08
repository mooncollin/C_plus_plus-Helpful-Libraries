import <cstdlib>;
import <iostream>;

import cmoon.math;
import cmoon.benchmarking;

class is_odd_benchmark : public cmoon::benchmarking::benchmark
{
	public:
		is_odd_benchmark()
			: cmoon::benchmarking::benchmark{"is_odd_benchmark"} {}

		void operator()() final
		{
			const auto r{cmoon::is_odd(rand())};
			cmoon::benchmarking::do_not_optimize(r);
		}
};

class is_even_benchmark : public cmoon::benchmarking::benchmark
{
	public:
		is_even_benchmark()
			: cmoon::benchmarking::benchmark{"is_even_benchmark"} {}

		void operator()() final
		{
			const auto r{cmoon::is_even(rand())};
			cmoon::benchmarking::do_not_optimize(r);
		}
};

class default_odd_benchmark : public cmoon::benchmarking::benchmark
{
	public:
		default_odd_benchmark()
			: cmoon::benchmarking::benchmark{"default_odd_benchmark"} {}

		void operator()() final
		{
			const auto r{rand() % 2 != 0};
			cmoon::benchmarking::do_not_optimize(r);
		}
};

class default_even_benchmark : public cmoon::benchmarking::benchmark
{
	public:
		default_even_benchmark()
			: cmoon::benchmarking::benchmark{"default_even_benchmark"} {}

		void operator()() final
		{
			const auto r{rand() % 2 == 0};
			cmoon::benchmarking::do_not_optimize(r);
		}
};

int main()
{
	cmoon::benchmarking::benchmark_compare compare_odd;
	compare_odd.run(is_odd_benchmark{});
	compare_odd.run(default_odd_benchmark{});

	std::cout << compare_odd;
	std::cout << "------------------------------------------\n\n";

	cmoon::benchmarking::benchmark_compare compare_even;
	compare_even.run(is_even_benchmark{});
	compare_even.run(default_even_benchmark{});

	std::cout << compare_even;
}