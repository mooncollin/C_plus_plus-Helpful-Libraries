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

#include "cmoon/mutex.hpp"
#include "cmoon/executors/rejected_execution.hpp"

namespace cmoon
{
	namespace executors
	{
		class single_thread_executor
		{
			public:
				single_thread_executor()
					: is_shutdown_{false}, is_terminated_{false}
				{
					std::thread t{&single_thread_executor::run, std::ref(*this)};
					t.detach();
				}

				single_thread_executor(const single_thread_executor&) = delete;
				single_thread_executor(single_thread_executor&&) = delete;
				single_thread_executor& operator=(const single_thread_executor&) = delete;
				single_thread_executor& operator=(single_thread_executor&&) = delete;

				~single_thread_executor()
				{
					shutdown();
					std::unique_lock l {terminated_m};
					terminated_cv.wait(l, [this] { return is_terminated_; });
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

					std::scoped_lock l {add_job_m};
					jobs.push(std::make_unique<any_job<std::invoke_result_t<F>>>(std::move(task)));

					add_job_cv.notify_one();
					return result;
				}

				void shutdown()
				{
					if (!is_shutdown_)
					{
						std::scoped_lock shutdown_write {shutdown_m};
						is_shutdown_ = true;
						add_job_cv.notify_all();
					}
				}

				void cancel()
				{
					std::scoped_lock l {add_job_m};
					jobs = {};
				}

				bool await_termination()
				{
					if (!is_shutdown())
					{
						return false;
					}

					std::unique_lock l {terminated_m};
					terminated_cv.wait(l, [this] { return is_terminated_; });
					return true;
				}

				template<class Rep, class Period>
				bool await_termination(const std::chrono::duration<Rep, Period>& wait)
				{
					if (!is_shutdown())
					{
						return false;
					}

					std::unique_lock l {terminated_m};
					return terminated_cv.wait_for(l, wait, [this] { return is_terminated_; });
				}

				[[nodiscard]] bool is_shutdown() const noexcept
				{
					return is_shutdown_;
				}

				[[nodiscard]] bool is_terminated() const noexcept
				{
					return is_terminated_;
				}

				[[nodiscard]] std::uint64_t threads() const noexcept
				{
					return 1;
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

				bool is_shutdown_;
				std::mutex shutdown_m;

				bool is_terminated_;
				std::mutex terminated_m;
				std::condition_variable terminated_cv;

				std::queue<std::unique_ptr<job>> jobs;
				std::mutex add_job_m;
				std::condition_variable_any add_job_cv;

				void run()
				{
					while(true)
					{
						std::unique_ptr<job> j;
						{
							cmoon::multi_lock l {add_job_m, shutdown_m};
							add_job_cv.wait(l, [this]() { return is_shutdown_ || !jobs.empty(); });

							if (jobs.empty() && is_shutdown_)
							{
								l.unlock();
								std::unique_lock l2 {terminated_m};
								is_terminated_ = true;
								std::notify_all_at_thread_exit(terminated_cv, std::move(l2));
								return;
							}

							j = std::move(jobs.front());
							jobs.pop();
						}

						j->execute();
					}
				}
		};
	}
}