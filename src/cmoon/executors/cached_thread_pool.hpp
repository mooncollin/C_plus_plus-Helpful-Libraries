#pragma once

#include <thread>
#include <chrono>
#include <concepts>
#include <future>
#include <atomic>
#include <queue>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <memory>
#include <cstddef>

#include "cmoon/mutex.hpp"
#include "cmoon/executors/rejected_execution.hpp"

namespace cmoon
{
	namespace executors
	{
		class cached_thread_pool
		{
			public:
				cached_thread_pool(const std::chrono::milliseconds& wait_time = std::chrono::seconds{60})
					: wait_time{wait_time}, is_shutdown_{false}, running_threads{0}, free_threads{0} {}

				cached_thread_pool(const cached_thread_pool&) = delete;
				cached_thread_pool& operator=(const cached_thread_pool&) = delete;
				cached_thread_pool(cached_thread_pool&&) = delete;
				cached_thread_pool& operator=(cached_thread_pool&&) = delete;

				~cached_thread_pool()
				{
					shutdown();
					std::unique_lock l {running_threads_m};
					running_threads_cv.wait(l, [this]{ return running_threads == 0; });
				}

				template<std::invocable<> F>
				std::future<std::invoke_result_t<F>> submit(F&& f)
				{
					if (is_shutdown())
					{
						throw rejected_execution{"cannot submit when shut down"};
					}

					std::packaged_task<std::invoke_result_t<F>()> task{std::forward<F>(f)};
					auto result = task.get_future();

					{
						std::scoped_lock l {add_job_m};
						jobs.push(std::make_unique<any_job<std::invoke_result_t<F>>>(std::move(task)));
					}

					if (free_threads.load() == 0)
					{
						{
							std::scoped_lock l{running_threads_m};
							++running_threads;
						}

						++free_threads;
						std::thread t{&cached_thread_pool::run, std::ref(*this)};
						t.detach();
					}

					add_job_cv.notify_one();
					return result;
				}

				void shutdown()
				{
					if (!is_shutdown_)
					{
						std::scoped_lock l {shutdown_m};
						is_shutdown_ = true;
						add_job_cv.notify_all();
					}
				}

				bool await_termination()
				{
					if (!is_shutdown())
					{
						return false;
					}

					std::unique_lock l {running_threads_m};
					running_threads_cv.wait(l, [this]{ return running_threads == 0; });
					return true;
				}

				template<class Rep, class Period>
				bool await_termination(const std::chrono::duration<Rep, Period>& wait)
				{
					if (!is_shutdown())
					{
						return false;
					}

					std::unique_lock l {running_threads_m};
					return running_threads_cv.wait_for(l, wait, [this]{ return running_threads == 0; });
				}

				void cancel()
				{
					std::scoped_lock l {add_job_m};
					jobs = {};
				}

				[[nodiscard]] bool is_shutdown() const noexcept
				{
					return is_shutdown_;
				}

				[[nodiscard]] bool is_terminated() const noexcept
				{
					return is_shutdown() && running_threads == 0;
				}

				[[nodiscard]] std::uint64_t threads() const noexcept
				{
					return running_threads;
				}

				[[nodiscard]] std::size_t queued_jobs() const noexcept
				{
					return jobs.size();
				}
			private:
				class job
				{
					public:
						virtual ~job() {}
						virtual void execute() = 0;
				};

				template<class R>
				class any_job : public job
				{
					public:
						any_job(std::packaged_task<R()>&& t)
							: task{std::move(t)} {}

						void execute() override
						{
							task();
						}
					private:
						std::packaged_task<R()> task;
				};

				std::chrono::milliseconds wait_time;
				bool is_shutdown_;
				std::uint64_t running_threads;
				std::atomic_uint64_t free_threads;

				std::mutex running_threads_m;
				std::condition_variable running_threads_cv;

				std::shared_mutex shutdown_m;

				std::queue<std::unique_ptr<job>> jobs;
				std::mutex add_job_m;
				std::condition_variable_any add_job_cv;

				void run()
				{
					std::shared_lock shutdown_read {shutdown_m, std::defer_lock};
					while(true)
					{
						std::unique_ptr<job> j;
						{
							cmoon::multi_lock l {add_job_m, shutdown_read};
							const auto r = add_job_cv.wait_for(l, wait_time,
								[this]{ return is_shutdown_ || !jobs.empty(); });

							--free_threads;

							if (jobs.empty() && (is_shutdown_ || !r))
							{
								l.unlock();
								std::unique_lock l2 {running_threads_m};
								--running_threads;
								std::notify_all_at_thread_exit(running_threads_cv, std::move(l2));
								return;
							}

							j = std::move(jobs.front());
							jobs.pop();
						}

						j->execute();
						++free_threads;
					}
				}
		};
	}
}