#pragma once

#include <chrono>
#include <concepts>
#include <mutex>
#include <condition_variable>

#include "cmoon/executors/cancelled_execution.hpp"
#include "cmoon/executors/cached_thread_pool.hpp"

namespace cmoon
{
	namespace executors
	{
		class timer;
		
		template<std::invocable<> F>
		class timed_future
		{
			public:
				timed_future(const timed_future&) = delete;
				timed_future(timed_future&&) = default;

				timed_future& operator=(const timed_future&) = delete;
				timed_future& operator=(timed_future&&) = default;

				bool cancel()
				{
					// Lock mutex first to allow user to cancel task
					// as soon to calling the cancel() method as possible.
					std::scoped_lock l {cancelled_m};
					if (is_cancelled_ || std::chrono::system_clock::now() >= start_time_)
					{
						return false;
					}

					is_cancelled_ = true;
					cancelled_cv.notify_one();
					return true;
				}

				[[nodiscard]] bool is_cancelled() const noexcept
				{
					return is_cancelled_;
				}

				std::future<std::invoke_result_t<F>>& future() noexcept
				{
					return future_;
				}

				[[nodiscard]] const std::future<std::invoke_result_t<F>>& future() const noexcept
				{
					return future_;
				}

				[[nodiscard]] const std::chrono::time_point<std::chrono::system_clock>& start_time() const noexcept
				{
					return start_time_;
				}
			private:
				template<class Executor>
				timed_future(const std::chrono::time_point<std::chrono::system_clock>& start, F&& func, Executor& ex)
					: start_time_{start}, is_cancelled_{false}, future_{ex.submit([this, forward_func = std::forward<F>(func)] {
						std::unique_lock l {cancelled_m};
						if (cancelled_cv.wait_until(l, start_time_, [this] { return is_cancelled_; }))
						{
							throw cancelled_execution{"task in timer has been cancelled"};
						}
						return forward_func();
					})} {}

				std::chrono::time_point<std::chrono::system_clock> start_time_;
				bool is_cancelled_;
				std::mutex cancelled_m;
				std::condition_variable cancelled_cv;

				std::future<std::invoke_result_t<F>> future_;

				friend class timer;
		};

		enum class timed_task_type
		{
			delay,
			fixed
		};

		template<timed_task_type Type, class Rep, class Period>
		class timed_task
		{
			public:
				timed_task(const timed_task&) = delete;
				timed_task(timed_task&&) = default;

				timed_task& operator=(const timed_task&) = delete;
				timed_task& operator=(timed_task&&) = default;

				bool cancel()
				{
					std::scoped_lock l {cancelled_m};
					is_cancelled_ = true;
					cancelled_cv.notify_one();
					return true;
				}

				[[nodiscard]] bool is_cancelled() const noexcept
				{
					return is_cancelled_;
				}

				[[nodiscard]] const std::chrono::time_point<std::chrono::system_clock>& start_time() const noexcept
				{
					return start_time_;
				}

				[[nodiscard]] const std::chrono::duration<Rep, Period>& period() const noexcept
				{
					return period_;
				}

				[[nodiscard]] static constexpr timed_task_type type() noexcept
				{
					return Type;
				}
			private:
				template<class Rep, class Period, std::invocable<> F, class Executor>
				timed_task(const std::chrono::time_point<std::chrono::system_clock>& start, const std::chrono::duration<Rep, Period>& period, F&& func, Executor& ex)
					: start_time_{start}, period_{period}, is_cancelled_{false}
				{
					ex.submit([this, forward_func = std::forward<F>(func), &ex] {
						std::unique_lock l{cancelled_m};
						if (cancelled_cv.wait_until(l, start_time_, [this] { return is_cancelled_; }))
						{
							throw cancelled_execution{"task in timer has been cancelled"};
						}

						if constexpr (type() == timed_task_type::delay)
						{
							forward_func();
							if (!ex.is_shutdown())
							{
								submit_repeat_delay(std::move(forward_func), ex);
							}
						}
						else
						{
							if (!ex.is_shutdown())
							{
								submit_repeat_fixed(forward_func, ex);
							}
							forward_func();
						}
					});
				}

				std::chrono::time_point<std::chrono::system_clock> start_time_;
				std::chrono::duration<Rep, Period> period_;
				bool is_cancelled_;
				std::mutex cancelled_m;
				std::condition_variable cancelled_cv;

				friend class timer;

				template<std::invocable<> F, class Executor>
				void submit_repeat_delay(F&& f, Executor& ex)
				{
					ex.pool.submit([this, forward_func = std::forward<F>(f), &ex] {
						std::unique_lock l{cancelled_m};
						if (cancelled_cv.wait_until(l, start_time_, [this] { return is_cancelled_; }))
						{
							throw cancelled_execution{"task in timer has been cancelled"};
						}
						forward_func();
						if (!ex.is_shutdown())
						{
							submit_repeat_delay(std::move(forward_func), period_);
						}
					});
				}

				template<std::invocable<> F, class Executor>
				void submit_repeat_fixed(F&& f, Executor& ex)
				{
					ex.pool.submit([this, forward_func = std::forward<F>(f), &ex] {
						std::unique_lock l{cancelled_m};
						if (cancelled_cv.wait_until(l, start_time_, [this] { return is_cancelled_; }))
						{
							throw cancelled_execution{"task in timer has been cancelled"};
						}
						if (!ex.is_shutdown())
						{
							submit_repeat_fixed(forward_func, period_);
						}
						forward_func();
					});
				}
		};

		class timer
		{
			public:
				timer() = default;

				template<std::invocable<> F>
				timed_future<F> submit(F&& f, const std::chrono::time_point<std::chrono::system_clock>& time)
				{
					if (pool.is_shutdown())
					{
						throw rejected_execution{"cannot submit when shut down"};
					}

					return {time, std::forward<F>(f), pool};
				}

				template<std::invocable<> F, class Rep, class Period>
				timed_future<F> submit(F&& f, const std::chrono::duration<Rep, Period>& delay)
				{
					return submit(std::forward<F>(f), std::chrono::system_clock::now() + delay);
				}

				template<std::invocable<> F, class Rep, class Period>
				timed_task<timed_task_type::delay, Rep, Period> submit(F&& f, const std::chrono::time_point<std::chrono::system_clock> time, const std::chrono::duration<Rep, Period>& period)
				{
					if (pool.is_shutdown())
					{
						throw rejected_execution{"cannot submit when shut down"};
					}

					return {time, period, std::forward<F>(f)};
				}

				template<std::invocable<> F, class Rep, class Period, class Rep2, class Period2>
				timed_task<timed_task_type::delay, Rep2, Period2> submit(F&& f, const std::chrono::duration<Rep, Period>& delay, const std::chrono::duration<Rep2, Period2>& period)
				{
					return submit(std::forward<F>(f), std::chrono::system_clock::now() + delay, period);
				}

				template<std::invocable<> F, class Rep, class Period>
				timed_task<timed_task_type::fixed, Rep, Period> submit_fixed(F&& f, const std::chrono::time_point<std::chrono::system_clock> time, const std::chrono::duration<Rep, Period>& period)
				{
					if (pool.is_shutdown())
					{
						throw rejected_execution{"cannot submit when shut down"};
					}

					return {time, period, std::forward<F>(f)};
				}

				template<std::invocable<> F, class Rep, class Period, class Rep2, class Period2>
				timed_task<timed_task_type::fixed, Rep2, Period2> submit_fixed(F&& f, const std::chrono::duration<Rep, Period>& delay, const std::chrono::duration<Rep2, Period2>& period)
				{
					return submit_fixed(std::forward<F>(f), std::chrono::system_clock::now() + delay, period);
				}

				void shutdown()
				{
					pool.shutdown();
				}

				void cancel()
				{
					pool.cancel();
				}

				bool await_termination()
				{
					return pool.await_termination();
				}

				template<class Rep, class Period>
				bool await_termination(const std::chrono::duration<Rep, Period>& wait)
				{
					return pool.await_termination(wait);
				}

				[[nodiscard]] bool is_shutdown() const noexcept
				{
					return pool.is_shutdown();
				}

				[[nodiscard]] bool is_terminated() const noexcept
				{
					return pool.is_terminated();
				}

				[[nodiscard]] std::uint64_t threads() const noexcept
				{
					return pool.threads();
				}

				[[nodiscard]] std::size_t queued_jobs() const noexcept
				{
					return pool.queued_jobs();
				}
			private:
				cached_thread_pool pool;

				template<timed_task_type, class, class>
				friend class timed_task;
		};
	}
}