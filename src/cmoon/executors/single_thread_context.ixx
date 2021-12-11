export module cmoon.executors.single_thread_context;

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

	export
	class single_thread_context
	{
		template<cmoon::execution::receiver R>
		friend struct operation;

		public:
			struct scheduler_t
			{
				struct sender_t
				{
					single_thread_context& pool_;

					template<template<class...> class Tuple, template<class...> class Variant>
					using value_types = Variant<Tuple<>>;

					template<template<class...> class Variant>
					using error_types = Variant<std::exception_ptr>;

					static constexpr bool sends_done {true};

					template<cmoon::execution::receiver R>
					friend operation<std::decay_t<R>> tag_invoke(cmoon::execution::connect_t, single_thread_context::scheduler_t::sender_t s, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return {s.pool_, std::forward<R>(r)};
					}

					template<class CPO>
					friend single_thread_context::scheduler_t tag_invoke(cmoon::execution::get_completion_scheduler_t<CPO>, single_thread_context::scheduler_t::sender_t s) noexcept
					{
						return {std::addressof(s.pool_)};
					}
				};

				single_thread_context* pool_;

				[[nodiscard]] friend sender_t tag_invoke(cmoon::execution::schedule_t, single_thread_context::scheduler_t t) noexcept
				{
					return {*t.pool_};
				}

				[[nodiscard]] friend cmoon::execution::forward_progress_guarantee tag_invoke(cmoon::execution::get_forward_progress_guarantee_t, single_thread_context::scheduler_t) noexcept
				{
					return cmoon::execution::forward_progress_guarantee::parallel;
				}

				[[nodiscard]] friend bool operator==(const scheduler_t& lhs, const scheduler_t& rhs) noexcept = default;
				[[nodiscard]] friend bool operator!=(const scheduler_t&, const scheduler_t&) noexcept = default;
			};

			single_thread_context()
				: thread_{&single_thread_context::run, this} {}

			single_thread_context(const single_thread_context&) = delete;
			single_thread_context(single_thread_context&&) = default;

			single_thread_context& operator=(const single_thread_context&) = delete;
			single_thread_context& operator=(single_thread_context&&) = default;

			~single_thread_context() noexcept
			{
				request_stop();
				join();
			}

			void request_stop() noexcept
			{
				std::lock_guard l{jobs_mut_};
				stop_requested_ = true;
				jobs_cv_.notify_one();
			}

			void join() noexcept
			{
				if (thread_.joinable())
				{
					thread_.join();
				}
			}

			[[nodiscard]] scheduler_t get_scheduler() noexcept
			{
				return {this};
			}
		private:
			std::thread thread_;
			cmoon::intrusive_queue<&task_base::next> jobs_queue_;
			std::mutex jobs_mut_;
			std::condition_variable jobs_cv_;
			bool stop_requested_{false};

			void enqueue(task_base* task) noexcept
			{
				std::lock_guard l {jobs_mut_};
				const auto was_empty {jobs_queue_.empty()};
				jobs_queue_.push_back(task);
				if (was_empty)
				{
					jobs_cv_.notify_one();
				}
			}

			void run()
			{
				while (true)
				{
					task_base* task;
					{
						std::unique_lock l{jobs_mut_};
						while (jobs_queue_.empty())
						{
							if (stop_requested_)
							{
								return;
							}

							jobs_cv_.wait(l);
						}

						task = jobs_queue_.pop_front();
					}

					task->execute(task);
				}
			}
	};

	template<cmoon::execution::receiver R>
	struct operation : public task_base
	{
		public:
			operation(single_thread_context& ctx, R&& r)
				: ctx_{ctx}, r_{std::forward<R>(r)}
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
			single_thread_context& ctx_;
			R r_;

			void start_helper(task_base* task) noexcept
			{
				ctx_.enqueue(task);
			}
	};
}