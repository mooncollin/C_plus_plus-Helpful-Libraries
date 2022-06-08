export module cmoon.benchmarking.benchmark_function;

import <concepts>;
import <utility>;
import <cstddef>;

import cmoon.benchmarking.benchmark;

namespace cmoon::benchmarking
{
	export
	template<std::invocable<> F>
	class benchmark_function : public benchmark
	{
		public:
			benchmark_function(F&& f, std::size_t runs = 10, std::size_t iterations = 100)
				: benchmark{typeid(f).name(), runs, iterations}, f_{std::forward<F>(f)} {}

			void operator()() final
			{
				std::invoke(f_);
			}
		private:
			F f_;
	};

	export
	template<class F>
	benchmark_function(F&&) -> benchmark_function<F>;
}