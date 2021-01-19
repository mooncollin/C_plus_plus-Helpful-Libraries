#pragma once

#undef max

#include <chrono>
#include <cstdint>
#include <mutex>
#include <list>
#include <vector>
#include <thread>
#include <limits>
#include <vector>
#include <condition_variable>
#include <atomic>

#include "cmoon/net/executor.hpp"
#include "cmoon/concepts.hpp"
#include "cmoon/utility.hpp"

namespace cmoon
{
	namespace net
	{
		template<class Clock, class WaitTraits>
		class basic_waitable_timer;

		class io_context : public execution_context
		{
			public:
				using count_type = std::uint_least32_t;

				class executor_type
				{
					public:
						executor_type(const executor_type& other) noexcept = default;
						executor_type(executor_type&& other) noexcept = default;
						executor_type& operator=(const executor_type& other) noexcept = default;
						executor_type& operator=(executor_type&& other) noexcept = default;

						bool running_in_this_thread() const noexcept
						{
							//return context().concurrency_limit == 0;
							return false;
						}

						io_context& context() const noexcept
						{
							return ctx.get();
						}

						void on_work_started() const noexcept
						{
							++context().work_count;
						}

						void on_work_finished() const noexcept
						{
							--context().work_count;
						}

						template<class Func, proto_allocator ProtoAllocator>
						void dispatch(Func&& f, const ProtoAllocator& a) const
						{
							if(running_in_this_thread())
							{
								cmoon::decay_copy(std::forward<Func>(f))();
								on_work_finished();
							}
							else
							{
								post(std::forward<Func>(f), a);
							}
						}

						template<class Func, proto_allocator ProtoAllocator>
						void post(Func&& f, const ProtoAllocator& a) const
						{
							std::scoped_lock l{context().mtx_};
							context().tasks_.push_back(std::forward<Func>(f));
						}

						template<class Func, proto_allocator ProtoAllocator>
						void defer(Func&& f, const ProtoAllocator& a) const
						{
							post(std::forward<Func>(f), a);
						}
					private:
						explicit executor_type(io_context& ctx)
							: ctx{ctx} {}

						friend class io_context;
						std::reference_wrapper<io_context> ctx;
				};

				io_context() = default;
				
				explicit io_context(count_type) {}

				io_context(const io_context&) = delete;
				io_context& operator=(const io_context&) = delete;

				executor_type get_executor() noexcept
				{
					return executor_type{*this};
				}

				count_type run()
				{
					count_type n = 0;
					while(run_one())
					{
						if (n != std::numeric_limits<count_type>::max())
						{
							++n;
						}
					}

					return n;
				}

				template<class Rep, class Period>
				count_type run_for(const std::chrono::duration<Rep, Period>& rel_time)
				{
					return run_until(std::chrono::steady_clock::now() + rel_time);
				}

				template<class Clock, class Duration>
				count_type run_until(const std::chrono::time_point<Clock, Duration>& abs_time)
				{
					count_type n = 0;
					while(run_one_until(abs_time))
					{
						if (n != std::numeric_limits<count_type>::max())
						{
							++n;
						}
					}

					return n;
				}

				count_type run_one()
				{
					std::scoped_lock l{mtx_};
					if(stopped_)
					{
						return 0;
					}

					if (outstanding_work() == 0)
					{
						stop();
						return 0;
					}

					if (tasks_.empty())
					{
						return 0;
					}

					auto f = std::move(tasks_.front());
					tasks_.pop_front();

					get_executor().on_work_started();
					auto fut = std::async(std::launch::async, [this, &f] { this->wrapper_function(std::forward<decltype(f)>(f)); });
					fut.wait();
					return 1;
				}

				template<class Rep, class Period>
				count_type run_one_for(const std::chrono::duration<Rep, Period>& rel_time)
				{
					return run_one_until(std::chrono::steady_clock::now() + rel_time);
				}

				template<class Clock, class Duration>
				count_type run_one_until(const std::chrono::time_point<Clock, Duration>& abs_time)
				{
					std::scoped_lock l{mtx_};
					if (stopped_)
					{
						return 0;
					}

					if (outstanding_work() == 0)
					{
						stop();
						return 0;
					}

					if (tasks_.empty())
					{
						return 0;
					}

					auto f = std::move(tasks_.front());
					tasks_.pop_front();

					if(!cv_.wait_until(l, abs_time, [this]() { return !stopped_; }) && stopped_)
					{
						return 0;
					}

					get_executor().on_work_started();
					std::async(std::launch::async, &io_context::wrapper_function<decltype(f)>, this, std::ref(f));
					return 1;
				}

				count_type poll()
				{
					std::scoped_lock l{mtx_};
					count_type n = 0;
					while (poll_one())
					{
						if (n != std::numeric_limits<count_type>::max())
						{
							++n;
						}
					}

					return n;
				}

				count_type poll_one()
				{
					std::scoped_lock l{mtx_};
					if (stopped_)
					{
						return 0;
					}

					if (outstanding_work() == 0)
					{
						stop();
						return 0;
					}

					if (tasks_.empty())
					{
						return 0;
					}
					
					auto f = std::move(tasks_.front());
					tasks_.pop_front();
					f();
					return 1;
				}

				void stop()
				{
					std::scoped_lock l{mtx_};
					stopped_ = true;
					cv_.notify_one();
				}

				bool stopped() const noexcept
				{
					std::scoped_lock l{mtx_};
					return stopped_;
				}

				void restart()
				{
					std::scoped_lock l{mtx_};
					stopped_ = false;
				}
			private:
				friend executor_type;

				mutable std::mutex mtx_;
				std::list<std::function<void()>> tasks_;
				std::condition_variable cv_;
				std::atomic<count_type> work_count{0};
				bool stopped_ {false};

				count_type outstanding_work() const noexcept
				{
					std::scoped_lock l{mtx_};
					return work_count + static_cast<count_type>(std::size(tasks_));
				}

				template<class Func>
				void wrapper_function(Func&& f)
				{
					std::forward<Func>(f)();
					get_executor().on_work_finished();
					cv_.notify_one();
				}
		};

		bool operator==(const io_context::executor_type& a, const io_context::executor_type& b) noexcept
		{
			return std::addressof(a) == std::addressof(b);
		}

		bool operator!=(const io_context::executor_type& a, const io_context::executor_type& b) noexcept
		{
			return !(a == b);
		}

		//static_assert(specialized_execution_context<io_context>);
	}
}