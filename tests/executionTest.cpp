#include <iostream>
#include <mutex>

#include "cmoon/execution/execution.hpp"
#include "cmoon/execution/thread_pool.hpp"

int main()
{
	constexpr std::size_t num_threads {155};
	cmoon::execution::static_thread_pool pool {num_threads};
	auto exec = pool.executor();

	std::mutex m;

	for (std::size_t i {0}; i < num_threads; ++i)
	{
		exec.execute([&m, i] { 
			std::scoped_lock l {m};
			std::cout << "I am thread #: " << i << '\n';
		});
	}
}