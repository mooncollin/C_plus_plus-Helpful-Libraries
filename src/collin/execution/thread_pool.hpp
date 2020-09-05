#pragma once

#include <cstddef>
#include <mutex>

namespace collin
{
	namespace execution
	{
		class static_thread_pool
		{
			public:
				using scheduler_type = ;
				using executor_type = ;

				explicit static_thread_pool(std::size_t num_threads);

				static_thread_pool(const static_thread_pool&) = delete;
				static_thread_pool& operator=(const static_thread_pool&) = delete;

				~static_thread_pool()
				{
					stop();
					wait();
				}

				// attach()
				void join()
				{
					std::scoped_lock l{mtx_};
					// Add this thread to the current available threads
					stop();
					wait();
				}

				void stop()
				{
					std::scoped_lock l{mtx_};
					stopped_ = true;
				}

				void wait()
				{
					std::scoped_lock l{mtx_};
					stop();
					// Wait until outstanding work is 0
				}

				scheduler_type scheduler() noexcept
				{
					std::scoped_lock l{mtx_};
				}

				executor_type executor() noexcept
				{
					std::scoped_lock l{mtx_};
				}
			private:
				bool stopped_ {false};
				std::mutex mtx_;
		};
	}
}