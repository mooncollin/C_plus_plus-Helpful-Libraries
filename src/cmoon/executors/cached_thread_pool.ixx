export module cmoon.executors.cached_thread_pool;

import <atomic>;
import <vector>;
import <deque>;
import <type_traits>;
import <utility>;
import <exception>;
import <thread>;
import <mutex>;
import <condition_variable>;
import <chrono>;
import <algorithm>;

import cmoon.execution;

namespace cmoon::executors
{
	export
	class cached_thread_pool
	{
		public:
			struct scheduler_t
			{
				struct sender_t
				{
					template<cmoon::execution::receiver R>
					struct op
					{
						public:
							cached_thread_pool* pool_;
							R r_;

							friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
							{
								o.start_helper();
							}
						private:
							void start_helper() noexcept
							{
								std::scoped_lock l {pool_->job_access};
								if (!pool_->stopped && !pool_->complete)
								{
									if constexpr (cmoon::tag_invocable<cmoon::execution::get_allocator_t, R>)
									{
										pool_->jobs.push_back(
											std::allocate_shared<scheduler_job<R>>(cmoon::execution::get_allocator(r_), std::move(r_))
										);
									}
									else
									{
										pool_->jobs.push_back(
											std::make_shared<scheduler_job<R>>(std::move(r_))
										);
									}

									++pool_->outstanding_work_;
									std::scoped_lock l2 {pool_->thread_access};
									if (pool_->available_threads() == 0)
									{
										pool_->add_thread();
									}
									else
									{
										pool_->job_notify.notify_one();
									}
								}
							}
					};

					cached_thread_pool* pool_;

					template<template<class...> class Tuple, template<class...> class Variant>
					using value_types = Variant<Tuple<>>;

					template<template<class...> class Variant>
					using error_types = Variant<std::exception_ptr>;

					static constexpr bool sends_done {true};

					template<cmoon::execution::receiver R>
					friend cached_thread_pool::scheduler_t::sender_t::op<R> tag_invoke(cmoon::execution::connect_t, cached_thread_pool::scheduler_t::sender_t s, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return {s.pool_, std::forward<R>(r)};
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

				[[nodiscard]] friend bool operator==(const scheduler_t&, const scheduler_t&) noexcept = default;
				[[nodiscard]] friend bool operator!=(const scheduler_t&, const scheduler_t&) noexcept = default;
			};

			cached_thread_pool(const std::chrono::nanoseconds& timeout = std::chrono::seconds{60})
				: timeout_{timeout} {}

			cached_thread_pool(const cached_thread_pool&) = delete;
			cached_thread_pool(cached_thread_pool&&) = default;

			cached_thread_pool& operator=(const cached_thread_pool&) = delete;
			cached_thread_pool& operator=(cached_thread_pool&&) = default;

			~cached_thread_pool() noexcept
			{
				stop();
				wait();
			}

			void attach()
			{
				thread_loop();
				wait();
			}

			void stop() noexcept
			{
				std::scoped_lock s {job_access};
				stopped = true;
				for (auto& job : jobs)
				{
					job->cancel();
				}
				jobs.clear();
				job_notify.notify_all();
			}

			void wait() noexcept
			{
				{
					std::scoped_lock s {job_access};
					if (!stopped)
					{
						complete = true;
					}
					job_notify.notify_all();
				}

				std::scoped_lock s {thread_access};
				for (auto& t : threads)
				{
					t.join();
				}

				threads.clear();
			}

			void restart() noexcept
			{
				std::scoped_lock l{job_access};
				stopped = false;
				complete = false;
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
			struct job
			{
				virtual void execute() noexcept = 0;
				virtual void cancel() noexcept = 0;
				virtual ~job() = default;
			};

			template<class R>
			struct scheduler_job : public job
			{
				scheduler_job(R&& r)
					: r_{std::forward<R>(r)} {}

				[[no_unique_address]] R r_;

				void execute() noexcept final
				{
					if (cmoon::execution::get_stop_token(r_).stop_requested())
					{
						cancel();
					}
					else
					{
						try
						{
							cmoon::execution::set_value(std::forward<R>(r_));
						}
						catch (...)
						{
							cmoon::execution::set_error(std::forward<R>(r_), std::current_exception());
						}
					}
				}

				void cancel() noexcept final
				{
					cmoon::execution::set_done(std::forward<R>(r_));
				}
			};
			
			std::atomic<std::chrono::nanoseconds> timeout_;
			std::mutex thread_access;
			std::mutex job_access;
			std::condition_variable job_notify;
			std::vector<std::thread> threads;
			std::deque<std::shared_ptr<job>> jobs;
			bool stopped {false};
			bool complete {false};
			std::size_t outstanding_work_{0};
			std::size_t busy_threads{0};

			template<cmoon::execution::receiver R>
			friend struct op;

			void add_thread()
			{
				threads.emplace_back(&cached_thread_pool::thread_loop, this);
			}

			[[nodiscard]] std::size_t available_threads() const noexcept
			{
				return std::size(threads) - busy_threads;
			}

			void thread_loop()
			{
				bool did_work {false};
				while (true)
				{
					std::shared_ptr<job> j;
					{
						std::unique_lock l {job_access};
						if (did_work)
						{
							--outstanding_work_;
							--busy_threads;
							if (complete)
							{
								job_notify.notify_all();
							}
						}

						if (!job_notify.wait_for(l, timeout_.load(std::memory_order_relaxed), [this] {
								return stopped || !jobs.empty() || (complete && outstanding_work_ == 0);
							}) && jobs.empty())
						{
							l.unlock();
							std::scoped_lock l2{thread_access};
							auto it {std::ranges::find_if(threads, [](const auto& t) { return t.get_id() == std::this_thread::get_id(); })};
							it->detach();
							threads.erase(it);
							return;
						}

						if (stopped || (complete && outstanding_work_ == 0))
						{
							return;
						}

						++busy_threads;
						j = std::move(jobs.front());
						jobs.pop_front();
					}

					j->execute();
					did_work = true;
				}
			}
	};
}