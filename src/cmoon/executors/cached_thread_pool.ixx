export module cmoon.executors.cached_thread_pool;

import <atomic>;
import <vector>;
import <type_traits>;
import <utility>;
import <thread>;
import <mutex>;
import <condition_variable>;
import <chrono>;
import <algorithm>;

import cmoon.execution;
import cmoon.intrusive_queue;

namespace cmoon::executors
{
	template<cmoon::execution::receiver R>
	struct operation;

	struct task_base
	{
		task_base* next;
		void (*execute)(task_base*) noexcept;
	};

	export
	class cached_thread_pool
	{
		public:
			struct scheduler_t
			{
				struct sender_t
				{
					cached_thread_pool* pool_;

					template<template<class...> class Tuple, template<class...> class Variant>
					using value_types = Variant<Tuple<>>;

					template<template<class...> class Variant>
					using error_types = Variant<std::exception_ptr>;

					static constexpr bool sends_done {true};

					template<cmoon::execution::receiver R>
					friend operation<std::decay_t<R>> tag_invoke(cmoon::execution::connect_t, cached_thread_pool::scheduler_t::sender_t s, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return {s.pool_, std::forward<R>(r)};
					}

					template<class CPO>
					friend cached_thread_pool::scheduler_t tag_invoke(cmoon::execution::get_completion_scheduler_t<CPO>, cached_thread_pool::scheduler_t::sender_t s) noexcept
					{
						return {s.pool_};
					}
				};

				cached_thread_pool* pool_;

				[[nodiscard]] friend sender_t tag_invoke(cmoon::execution::schedule_t, cached_thread_pool::scheduler_t t) noexcept
				{
					return {t.pool_};
				}

				[[nodiscard]] friend cmoon::execution::forward_progress_guarantee tag_invoke(cmoon::execution::get_forward_progress_guarantee_t, cached_thread_pool::scheduler_t) noexcept
				{
					return cmoon::execution::forward_progress_guarantee::parallel;
				}

				[[nodiscard]] friend bool operator==(const scheduler_t& lhs, const scheduler_t& rhs) noexcept = default;
				[[nodiscard]] friend bool operator!=(const scheduler_t&, const scheduler_t&) noexcept = default;
			};

			cached_thread_pool(const std::chrono::nanoseconds& timeout = std::chrono::seconds{60}) noexcept
				: timeout_{timeout} {}

			cached_thread_pool(const cached_thread_pool&) = delete;
			cached_thread_pool(cached_thread_pool&&) = default;

			cached_thread_pool& operator=(const cached_thread_pool&) = delete;
			cached_thread_pool& operator=(cached_thread_pool&&) = default;

			~cached_thread_pool() noexcept
			{
				request_stop();
				join();
			}

			void request_stop() noexcept
			{
				std::lock_guard s {jobs_mut_};
				stop_requested_ = true;
				jobs_cv_.notify_all();
			}

			void join() noexcept
			{
				auto n {num_threads.load(std::memory_order_relaxed)};
				while (n != 0)
				{
					num_threads.wait(n);
					n = num_threads.load(std::memory_order_relaxed);
				}
			}

			[[nodiscard]] scheduler_t get_scheduler() noexcept
			{
				return {this};
			}

			const std::chrono::nanoseconds timeout() const noexcept
			{
				return timeout_.load(std::memory_order_relaxed);
			}

			void timeout(const std::chrono::nanoseconds& t) noexcept
			{
				timeout_.store(t, std::memory_order_relaxed);
			}
		private:
			std::atomic<std::chrono::nanoseconds> timeout_;
			std::mutex jobs_mut_;
			std::condition_variable jobs_cv_;
			cmoon::intrusive_queue<&task_base::next> queue_;
			std::atomic<std::size_t> num_threads{0};
			std::atomic<std::size_t> busy_threads{0};
			bool stop_requested_{false};

			template<cmoon::execution::receiver R>
			friend struct operation;

			[[nodiscard]] std::size_t available_threads() const noexcept
			{
				return num_threads - busy_threads;
			}

			void enqueue(task_base* task) noexcept
			{
				if (available_threads() == 0)
				{
					++num_threads;
					std::thread{&cached_thread_pool::thread_loop, this, task}.detach();
					return;
				}

				std::lock_guard l{jobs_mut_};
				const auto was_empty {queue_.empty()};
				queue_.push_back(task);
				if (was_empty)
				{
					jobs_cv_.notify_one();
				}
			}

			task_base* dequeue()
			{
				std::unique_lock l{jobs_mut_};
				while (queue_.empty())
				{
					if (stop_requested_)
					{
						return nullptr;
					}

					if (jobs_cv_.wait_for(l, timeout_.load(std::memory_order_relaxed)) == std::cv_status::timeout)
					{
						return nullptr;
					}
				}

				return queue_.pop_front();
			}

			inline void thread_loop(task_base* task)
			{
				do
				{
					++busy_threads;
					task->execute(task);
					--busy_threads;

					task = dequeue();
				}
				while (task != nullptr);

				if (--num_threads == 0)
				{
					num_threads.notify_all();
				}
			}
	};

	template<cmoon::execution::receiver R>
	struct operation : public task_base
	{
		public:
			operation(cached_thread_pool* pool, R&& r)
				: pool_{pool}, r_{std::forward<R>(r)}
			{
				this->execute = [](task_base* t) noexcept
				{
					auto& op = *static_cast<operation*>(t);
					if (cmoon::execution::get_stop_token(op.r_).stop_requested())
					{
						cmoon::execution::set_done(std::move(op.r_));
					}
					else
					{
						cmoon::execution::set_value(std::move(op.r_));
					}
				};
			}

			friend void tag_invoke(cmoon::execution::start_t, operation& o) noexcept
			{
				o.start_helper(std::addressof(o));
			}
		private:
			cached_thread_pool* pool_;
			R r_;

			void start_helper(task_base* task) noexcept
			{
				pool_->enqueue(task);
			}
	};
}