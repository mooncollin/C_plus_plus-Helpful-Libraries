export module cmoon.executors.static_thread_pool;

import <vector>;
import <deque>;
import <functional>;
import <memory>;
import <type_traits>;
import <utility>;
import <exception>;
import <thread>;
import <mutex>;
import <concepts>;
import <condition_variable>;

import cmoon.execution;
import cmoon.functional;

namespace cmoon::executors
{
	export
	class static_thread_pool
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
							static_thread_pool* pool_;
							R r_;

							friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
							{
								o.start_helper();
							}
						private:
							void start_helper() noexcept
							{
								std::scoped_lock l {pool_->job_access};
								if (!pool_->stopped_ && !pool_->complete)
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
									pool_->job_notify.notify_one();
								}
							}
					};

					static_thread_pool* pool_;

					template<template<class...> class Tuple, template<class...> class Variant>
					using value_types = Variant<Tuple<>>;

					template<template<class...> class Variant>
					using error_types = Variant<std::exception_ptr>;

					static constexpr bool sends_done {true};

					template<cmoon::execution::receiver R>
					friend static_thread_pool::scheduler_t::sender_t::op<R> tag_invoke(cmoon::execution::connect_t, static_thread_pool::scheduler_t::sender_t s, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return {s.pool_, std::forward<R>(r)};
					}
				};

				static_thread_pool* pool_;

				[[nodiscard]] friend sender_t tag_invoke(cmoon::execution::schedule_t, static_thread_pool::scheduler_t t) noexcept
				{
					return {t.pool_};
				}

				[[nodiscard]] friend cmoon::execution::forward_progress_guarantee tag_invoke(cmoon::execution::get_forward_progress_guarantee_t, static_thread_pool::scheduler_t) noexcept
				{
					return cmoon::execution::forward_progress_guarantee::parallel;
				}

				[[nodiscard]] friend bool operator==(const scheduler_t&, const scheduler_t&) noexcept = default;
				[[nodiscard]] friend bool operator!=(const scheduler_t&, const scheduler_t&) noexcept = default;
			};

			static_thread_pool(std::size_t num_threads)
				: num_threads{num_threads}
			{
				threads.reserve(num_threads);

				for (std::size_t i {0}; i < num_threads; ++i)
				{
					threads.emplace_back(&static_thread_pool::thread_loop, this);
				}
			}

			static_thread_pool(const static_thread_pool&) = delete;
			static_thread_pool(static_thread_pool&&) = default;

			static_thread_pool& operator=(const static_thread_pool&) = delete;
			static_thread_pool& operator=(static_thread_pool&&) = default;

			~static_thread_pool() noexcept
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
				stopped_ = true;
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
					if (!stopped_)
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
				{
					std::scoped_lock l{job_access};
					stopped_ = false;
					complete = false;
				}

				{
					std::scoped_lock l{thread_access};
					for (std::size_t i {0}; i < num_threads; ++i)
					{
						threads.emplace_back(&static_thread_pool::thread_loop, this);
					}
				}
			}

			[[nodiscard]] scheduler_t get_scheduler() noexcept
			{
				return {this};
			}
		private:
			struct job
			{
				virtual void execute() noexcept = 0;
				virtual void cancel() noexcept = 0;
				virtual ~job() noexcept = default;
			};

			template<cmoon::execution::receiver R>
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

			std::size_t num_threads;
			std::mutex thread_access;
			std::mutex job_access;
			std::condition_variable job_notify;
			std::vector<std::thread> threads;
			std::deque<std::shared_ptr<job>> jobs;
			bool stopped_ {false};
			bool complete {false};
			std::size_t outstanding_work_{0};

			template<cmoon::execution::receiver R>
			friend struct op;

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
							if (complete)
							{
								job_notify.notify_all();
							}
						}

						job_notify.wait(l, [this] {
							return stopped_ || !jobs.empty() || (complete && outstanding_work_ == 0);
						});

						if (stopped_ || (complete && outstanding_work_ == 0))
						{
							return;
						}

						j = std::move(jobs.front());
						jobs.pop_front();
					}

					j->execute();
					did_work = true;
				}
			}
	};
}