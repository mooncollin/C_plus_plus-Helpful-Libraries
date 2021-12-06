export module cmoon.executors.static_thread_pool;

import <vector>;
import <functional>;
import <memory>;
import <type_traits>;
import <utility>;
import <thread>;
import <mutex>;
import <concepts>;
import <condition_variable>;

import cmoon.execution;
import cmoon.functional;
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

	class thread_state
	{
		public:
			task_base* try_pop()
			{
				std::unique_lock l{mut_, std::try_to_lock};
				if (!l || queue_.empty())
				{
					return nullptr;
				}

				return queue_.pop_front();
			}

			task_base* pop()
			{
				std::unique_lock l{mut_};
				while (queue_.empty())
				{
					if (stop_requested_)
					{
						return nullptr;
					}

					cv_.wait(l);
				}

				return queue_.pop_front();
			}

			bool try_push(task_base* task)
			{
				std::unique_lock l{mut_, std::try_to_lock};
				if (!l)
				{
					return false;
				}

				const auto was_empty {queue_.empty()};
				queue_.push_back(task);
				if (was_empty)
				{
					cv_.notify_one();
				}

				return true;
			}

			void push(task_base* task)
			{
				std::lock_guard l{mut_};
				const auto was_empty {queue_.empty()};
				queue_.push_back(task);
				if (was_empty)
				{
					cv_.notify_one();
				}
			}

			void request_stop()
			{
				std::lock_guard l{mut_};
				stop_requested_ = true;
				cv_.notify_one();
			}
		private:
			std::mutex mut_;
			std::condition_variable cv_;
			cmoon::intrusive_queue<&task_base::next> queue_;
			bool stop_requested_{false};
	};

	export
	class static_thread_pool
	{
		template<cmoon::execution::receiver R>
		friend struct operation;

		public:
			struct scheduler_t
			{
				struct sender_t
				{
					static_thread_pool& pool_;

					template<template<class...> class Tuple, template<class...> class Variant>
					using value_types = Variant<Tuple<>>;

					template<template<class...> class Variant>
					using error_types = Variant<std::exception_ptr>;

					static constexpr bool sends_done {true};

					template<cmoon::execution::receiver R>
					friend operation<std::decay_t<R>> tag_invoke(cmoon::execution::connect_t, static_thread_pool::scheduler_t::sender_t s, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return {s.pool_, std::forward<R>(r)};
					}

					template<class CPO>
					friend static_thread_pool::scheduler_t tag_invoke(cmoon::execution::get_completion_scheduler_t<CPO>, static_thread_pool::scheduler_t::sender_t s) noexcept
					{
						return {std::addressof(s.pool_)};
					}
				};

				static_thread_pool* pool_;

				[[nodiscard]] friend sender_t tag_invoke(cmoon::execution::schedule_t, static_thread_pool::scheduler_t t) noexcept
				{
					return {*t.pool_};
				}

				[[nodiscard]] friend cmoon::execution::forward_progress_guarantee tag_invoke(cmoon::execution::get_forward_progress_guarantee_t, static_thread_pool::scheduler_t) noexcept
				{
					return cmoon::execution::forward_progress_guarantee::parallel;
				}

				[[nodiscard]] friend bool operator==(const scheduler_t& lhs, const scheduler_t& rhs) noexcept = default;
				[[nodiscard]] friend bool operator!=(const scheduler_t&, const scheduler_t&) noexcept = default;
			};

			static_thread_pool(std::size_t num_threads)
				: num_threads{num_threads},
				  thread_states{num_threads},
				  next_thread{0}
			{
				threads.reserve(num_threads);

				try
				{
					for (std::size_t i {0}; i < num_threads; ++i)
					{
						threads.emplace_back(&static_thread_pool::thread_loop, this, i);
					}
				}
				catch (...)
				{
					request_stop();
					join();
					throw;
				}
			}

			static_thread_pool(const static_thread_pool&) = delete;
			static_thread_pool(static_thread_pool&&) = default;

			static_thread_pool& operator=(const static_thread_pool&) = delete;
			static_thread_pool& operator=(static_thread_pool&&) = default;

			~static_thread_pool() noexcept
			{
				request_stop();
				join();
			}

			void request_stop() noexcept
			{
				for (auto& state : thread_states)
				{
					state.request_stop();
				}
			}

			void join() noexcept
			{
				for (auto& t : threads)
				{
					t.join();
				}

				threads.clear();
			}

			[[nodiscard]] scheduler_t get_scheduler() noexcept
			{
				return {this};
			}
		private:
			std::size_t num_threads;
			std::vector<std::thread> threads;
			std::vector<thread_state> thread_states;
			std::atomic<std::size_t> next_thread;

			template<cmoon::execution::receiver R>
			friend struct operation;

			void enqueue(task_base* task) noexcept
			{
				const std::size_t start_index {next_thread.fetch_add(1, std::memory_order_relaxed) % num_threads};
				for (std::size_t i {0}; i < num_threads; ++i)
				{
					const auto index {start_index + i < num_threads
										? (start_index + i)
										: (start_index + i - num_threads)};
					if (thread_states[index].try_push(task))
					{
						return;
					}
				}

				thread_states[start_index].push(task);
			}

			void thread_loop(std::size_t index)
			{
				while (true)
				{
					task_base* task {nullptr};
					for (std::size_t i {0}; i < num_threads; ++i)
					{
						auto queue_index = (index + i) < num_threads
											? (index + i)
											: (index + i - num_threads);
						auto& state = thread_states[queue_index];
						task = state.try_pop();
						if (task != nullptr)
						{
							break;
						}
					}

					if (task == nullptr)
					{
						task = thread_states[index].pop();
						if (task == nullptr)
						{
							return;
						}
					}

					task->execute(task);
				}
			}
	};

	template<cmoon::execution::receiver R>
	struct operation : public task_base
	{
		public:
			operation(static_thread_pool& pool, R&& r)
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
			static_thread_pool& pool_;
			R r_;

			void start_helper(task_base* task) noexcept
			{
				pool_.enqueue(task);
			}
	};
}