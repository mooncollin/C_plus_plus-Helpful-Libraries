export module cmoon.executors.static_thread_pool;

import <cstddef>;
import <atomic>;
import <vector>;
import <deque>;
import <functional>;
import <memory>;
import <type_traits>;
import <utility>;
import <exception>;
import <thread>;
import <mutex>;
import <shared_mutex>;
import <algorithm>;
import <ranges>;
import <condition_variable>;

import cmoon.execution;

namespace cmoon::executors
{
	export
	class static_thread_pool
	{
		template<auto B = cmoon::execution::blocking.possibly,
				 auto R = cmoon::execution::relationship.fork,
				 auto O = cmoon::execution::outstanding_work.untracked,
				 typename A = std::allocator<void>>
		class sender_t
		{
			template<class Receiver>
			struct op
			{
				public:
					op(static_thread_pool& p, const A& alloc, Receiver&& r)
						: pool_{p}, alloc{alloc}, r_{std::forward<Receiver>(r)} {}

					void start() noexcept
					{
						std::unique_lock l {pool_.get().job_access};
						if (!pool_.get().stopped)
						{
							if constexpr (B == cmoon::execution::blocking.always)
							{
								l.unlock();
								try
								{
									cmoon::execution::set_value(std::forward<Receiver>(r_));
								}
								catch (...)
								{
									cmoon::execution::set_error(std::forward<Receiver>(r_), std::current_exception());
								}
							}
							else
							{
								pool_.get().jobs.push_back(
									std::allocate_shared<scheduler_job<Receiver>>(alloc, std::forward<Receiver>(r_))
								);
								++pool_.get().outstanding_work_;
								pool_.get().job_notify.notify_one();
							}
						}
					}
				private:
					std::reference_wrapper<static_thread_pool> pool_;
					A alloc;
					Receiver r_;
			};

			public:
				sender_t(const sender_t&) noexcept = default;
				sender_t(sender_t&&) noexcept = default;

				template<template<class...> class Tuple, template<class...> class Variant>
				using value_types = Variant<Tuple<>>;

				template<template<class...> class Variant>
				using error_types = Variant<>;

				static constexpr bool sends_done {true};

				template<class Receiver>
				op<Receiver> connect(Receiver&& r) const
				{
					return {pool_, a_, std::forward<Receiver>(r)};
				}

				template<class B2>
					requires(std::same_as<std::remove_cvref_t<B2>, cmoon::execution::blocking_t::never_t> ||
							 std::same_as<std::remove_cvref_t<B2>, cmoon::execution::blocking_t::possibly_t> ||
							 std::same_as<std::remove_cvref_t<B2>, cmoon::execution::blocking_t::always_t>)
				auto require(const B2) const
				{
					return sender_t<B2{}, R, O, A>{pool_, a_};
				}

				template<class R2>
					requires(std::same_as<std::remove_cvref_t<R2>, cmoon::execution::relationship_t::continuation_t> ||
							 std::same_as<std::remove_cvref_t<R2>, cmoon::execution::relationship_t::fork_t>)
				auto require(const R2) const
				{
					return sender_t<B, R2{}, O, A>{pool_, a_};
				}

				template<class O2>
					requires(std::same_as<std::remove_cvref_t<O2>, cmoon::execution::outstanding_work_t::tracked_t> ||
							 std::same_as<std::remove_cvref_t<O2>, cmoon::execution::outstanding_work_t::untracked_t>)
				auto require(const O2) const
				{
					return sender_t<B, R, O2{}, A>{pool_, a_};
				}

				template<class ProtoAllocator>
				auto require(const cmoon::execution::allocator_t<ProtoAllocator>& a) const
				{
					return sender_t<B, R, O, ProtoAllocator>{pool_, a.value()};
				}

				auto require(const cmoon::execution::allocator_t<void>) const
				{
					return require(cmoon::execution::allocator(std::allocator<void>()));
				}

				[[nodiscard]] static constexpr cmoon::execution::bulk_guarantee_t query(const cmoon::execution::bulk_guarantee_t) noexcept
				{
					return cmoon::execution::bulk_guarantee.parallel;
				}

				[[nodiscard]] static constexpr cmoon::execution::mapping_t query(const cmoon::execution::mapping_t) noexcept
				{
					return cmoon::execution::mapping.thread;
				}

				[[nodiscard]] static constexpr execution::blocking_t query(const cmoon::execution::blocking_t) noexcept
				{
					return B;
				}

				[[nodiscard]] static constexpr execution::relationship_t query(const cmoon::execution::relationship_t) noexcept
				{
					return R;
				}

				[[nodiscard]] static constexpr execution::outstanding_work_t query(const cmoon::execution::outstanding_work_t) noexcept
				{
					return O;
				}

				auto query(const cmoon::execution::allocator_t<void>) const noexcept
				{
					return std::allocator<void>{};
				}

				template<class ProtoAllocator>
				auto query(const cmoon::execution::allocator_t<ProtoAllocator>) const noexcept
				{
					return a_;
				}

				[[nodiscard]] friend bool operator==(const sender_t& lhs, const sender_t& rhs) noexcept
				{
					return std::addressof(lhs.pool_.get()) == std::addressof(rhs.pool_.get()) &&
							lhs.a_ == rhs.a_;
				}

				[[nodiscard]] friend bool operator!=(const sender_t&, const sender_t&) noexcept = default;

				template<auto B2, auto R2, auto O2, typename A2>
				[[nodiscard]] friend bool operator==(const sender_t& lhs, const sender_t<B2, R2, O2, A2>& rhs) noexcept
				{
					return false;
				}

				template<auto B2, auto R2, auto O2, typename A2>
				[[nodiscard]] friend bool operator!=(const sender_t& lhs, const sender_t<B2, R2, O2, A2>& rhs) noexcept
				{
					return true;
				}

				bool running_in_this_thread() const noexcept
				{
					return false;
				}
			private:
				sender_t(static_thread_pool& p, const A& a = {})
					: pool_{p}, a_{a} {}
						
				std::reference_wrapper<static_thread_pool> pool_;
				A a_;

				template<class Allocator>
				friend class scheduler_t;

				template<auto B2, auto R2, auto O2, typename A2>
				friend class executor_t;

				template<class Receiver>
				struct op;
		};

		template<class Allocator = std::allocator<void>>
		class scheduler_t
		{
			public:
				using sender_type = sender_t<>;

				scheduler_t(const scheduler_t&) noexcept = default;
				scheduler_t(scheduler_t&&) noexcept = default;

				template<class ProtoAllocator>
				scheduler_t<ProtoAllocator> require(const cmoon::execution::allocator_t<ProtoAllocator>& a) const
				{
					return {pool_.get(), a};
				}

				auto require(const cmoon::execution::allocator_t<void>& a) const
				{
					return require(cmoon::execution::allocator(std::allocator<void>{}));
				}

				static_thread_pool& query(const cmoon::execution::context_t) const noexcept
				{
					return pool_.get();
				}

				auto query(const cmoon::execution::allocator_t<void>) const noexcept
				{
					return std::allocator<void>{};
				}

				template<class ProtoAllocator>
				auto query(const cmoon::execution::allocator_t<ProtoAllocator>) const noexcept
				{
					return a_;
				}

				bool running_in_this_thread() const noexcept
				{
					return false;
				}

				[[nodiscard]] friend bool operator==(const scheduler_t& lhs, const scheduler_t& rhs) noexcept
				{
					return std::addressof(lhs.pool_.get()) == std::addressof(rhs.pool_.get()) &&
						   lhs.a_ == rhs.a_;
				}

				[[nodiscard]] friend bool operator!=(const scheduler_t& lhs, const scheduler_t& rhs) noexcept = default;

				template<class Allocator2>
				[[nodiscard]] friend bool operator==(const scheduler_t& lhs, const scheduler_t<Allocator>& rhs) noexcept
				{
					return false;
				}

				template<class Allocator2>
				[[nodiscard]] friend bool operator!=(const scheduler_t& lhs, const scheduler_t<Allocator>& rhs) noexcept
				{
					return true;
				}

				sender_type schedule() noexcept
				{
					return {};
				}
			private:
				scheduler_t(static_thread_pool& pool, const Allocator& a = {})
					: pool_{pool}, a_{a} {}

				std::reference_wrapper<static_thread_pool> pool_;
				Allocator a_;

				friend class static_thread_pool;
		};

		template<auto B = cmoon::execution::blocking.possibly,
				 auto R = cmoon::execution::relationship.fork,
				 auto O = cmoon::execution::outstanding_work.untracked,
				 typename A = std::allocator<void>>
		class executor_t
		{
			public:
				using shape_type = std::size_t;
				using index_type = std::size_t;

				executor_t() noexcept = default;

				executor_t() noexcept requires(O == cmoon::execution::outstanding_work.tracked)
				{
					std::scoped_lock s {pool_.get().job_access};
					++pool_.get().outstanding_work_;
				}

				~executor_t() noexcept = default;
				~executor_t() noexcept requires (O == cmoon::execution::outstanding_work.tracked)
				{
					std::scoped_lock s {pool_.get().job_access};
					--pool_.get().outstanding_work_;
				}

				executor_t(const executor_t&) noexcept = default;
				executor_t(executor_t&&) noexcept = default;

				template<class Function>
				void execute(Function&& f) const
				{
					std::unique_lock l {pool_.get().job_access};
					if (!pool_.get().stopped)
					{
						if constexpr (B == cmoon::execution::blocking.always)
						{
							l.unlock();
							try
							{
								std::invoke(std::forward<Function>(f));
							}
							catch (...)
							{
								std::terminate();
							}
						}
						else
						{
							pool_.get().jobs.push_back(std::allocate_shared<executor_job<Function>>(a_, std::forward<Function>(f)));
							++pool_.get().outstanding_work_;
							pool_.get().job_notify.notify_one();
						}
					}
				}

				template<class Function>
				void bulk_execute(Function&& f, std::size_t n) const
				{
					std::unique_lock l {pool_.get().job_access};
					if (!pool_.get().stopped)
					{
						if constexpr (B == cmoon::execution::blocking.always)
						{
							l.unlock();
							try
							{
								for (std::size_t i {0}; i != n; ++i)
								{
									std::invoke(f, i);
								}
							}
							catch (...)
							{
								std::terminate();
							}
						}
						else
						{
							pool_.get().outstanding_work_ += n;
							for (std::size_t i {0}; i != n; ++i)
							{
								pool_.get().jobs.push_back(std::allocate_shared<bulk_executor_job<Function>>(a_, std::forward<Function>(f), i));
							}

							if (n == 1)
							{
								pool_.get().job_notify.notify_one();
							}
							else
							{
								pool_.get().job_notify.notify_all();
							}
						}
					}
				}

				template<class B2>
				requires(std::same_as<std::remove_cvref_t<B2>, cmoon::execution::blocking_t::never_t> ||
						 std::same_as<std::remove_cvref_t<B2>, cmoon::execution::blocking_t::possibly_t> ||
						 std::same_as<std::remove_cvref_t<B2>, cmoon::execution::blocking_t::always_t>)
				auto require(const B2) const
				{
					return executor_t<B2{}, R, O, A>{pool_, a_};
				}

				template<class R2>
					requires(std::same_as<std::remove_cvref_t<R2>, cmoon::execution::relationship_t::continuation_t> ||
							 std::same_as<std::remove_cvref_t<R2>, cmoon::execution::relationship_t::fork_t>)
				auto require(const R2) const
				{
					return executor_t<B, R2{}, O, A>{pool_, a_};
				}

				template<class O2>
					requires(std::same_as<std::remove_cvref_t<O2>, cmoon::execution::outstanding_work_t::tracked_t> ||
							 std::same_as<std::remove_cvref_t<O2>, cmoon::execution::outstanding_work_t::untracked_t>)
				auto require(const O2) const
				{
					return executor_t<B, R, O2{}, A>{pool_, a_};
				}

				template<class ProtoAllocator>
				auto require(const cmoon::execution::allocator_t<ProtoAllocator>& a) const
				{
					return executor_t<B, R, O, ProtoAllocator>{pool_, a.value()};
				}

				auto require(const cmoon::execution::allocator_t<void>) const
				{
					return require(cmoon::execution::allocator(std::allocator<void>()));
				}

				[[nodiscard]] static constexpr cmoon::execution::bulk_guarantee_t query(const cmoon::execution::bulk_guarantee_t) noexcept
				{
					return cmoon::execution::bulk_guarantee.parallel;
				}

				[[nodiscard]] static constexpr cmoon::execution::mapping_t query(const cmoon::execution::mapping_t) noexcept
				{
					return cmoon::execution::mapping.thread;
				}

				[[nodiscard]] static constexpr execution::blocking_t query(const cmoon::execution::blocking_t) noexcept
				{
					return B;
				}

				[[nodiscard]] static constexpr execution::relationship_t query(const cmoon::execution::relationship_t) noexcept
				{
					return R;
				}

				[[nodiscard]] static constexpr execution::outstanding_work_t query(const cmoon::execution::outstanding_work_t) noexcept
				{
					return O;
				}

				auto query(const cmoon::execution::allocator_t<void>) const noexcept
				{
					return std::allocator<void>{};
				}

				template<class ProtoAllocator>
				auto query(const cmoon::execution::allocator_t<ProtoAllocator>) const noexcept
				{
					return a_;
				}

				[[nodiscard]] friend bool operator==(const executor_t& lhs, const executor_t& rhs) noexcept
				{
					return std::addressof(lhs.pool_.get()) == std::addressof(rhs.pool_.get());
				}

				[[nodiscard]] friend bool operator!=(const executor_t&, const executor_t&) noexcept = default;

				template<auto B2, auto R2, auto O2, typename A2>
				[[nodiscard]] friend bool operator==(const executor_t& lhs, const executor_t<B2, R2, O2, A2>& rhs) noexcept
				{
					return false;
				}

				template<auto B2, auto R2, auto O2, typename A2>
				[[nodiscard]] friend bool operator!=(const executor_t& lhs, const executor_t<B2, R2, O2, A2>& rhs) noexcept
				{
					return true;
				}

				bool running_in_this_thread() const noexcept
				{
					if constexpr (B == cmoon::execution::blocking.always)
					{
						return true;
					}
					else
					{
						std::shared_lock l {pool_.get().thread_access};
						return std::ranges::find(pool_.get().thread_ids, std::this_thread::get_id()) !=
										 std::ranges::end(pool_.get().thread_ids);
					}
				}

				auto schedule() const
				{
					return sender_t<B, R, O, A>{pool_, a_};
				}
			private:
				executor_t(static_thread_pool& pool, const A& a = {})
					: pool_{pool}, a_{a} {}

				std::reference_wrapper<static_thread_pool> pool_;
				A a_;

				friend class static_thread_pool;
		};

		public:
			using scheduler_type = scheduler_t<>;
			using executor_type = executor_t<>;

			static_thread_pool(std::size_t num_threads)
			{
				threads.reserve(num_threads);
				thread_ids.reserve(num_threads);

				for (std::size_t i {0}; i < num_threads; ++i)
				{
					threads.emplace_back(&static_thread_pool::thread_loop, this);
					thread_ids.push_back(threads.back().get_id());
				}
			}

			~static_thread_pool()
			{
				stop();
				wait();
			}

			void attach()
			{
				{
					std::scoped_lock s {thread_access};
					thread_ids.push_back(std::this_thread::get_id());
				}
				thread_loop();
				wait();
			}

			void stop()
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

			void wait()
			{
				{
					std::scoped_lock s {job_access};
					if (!stopped)
					{
						complete = true;
					}
					job_notify.notify_all();
				}

				for (auto& t : threads)
				{
					t.join();
				}

				std::scoped_lock s {thread_access};
				threads.clear();
				thread_ids.clear();
			}

			scheduler_type scheduler() noexcept
			{
				return {*this};
			}

			executor_type executor() noexcept
			{
				return {*this};
			}
		private:
			struct job
			{
				virtual void execute() noexcept = 0;
				virtual void cancel() noexcept = 0;
				virtual ~job() {}
			};

			template<class F>
			struct executor_job : public job
			{
				executor_job(F&& f)
					: f_{std::forward<F>(f)} {}

				~executor_job() final {}

				void execute() noexcept final
				{
					try
					{
						std::invoke(std::forward<F>(f_));
					}
					catch (...)
					{
						std::terminate();
					}
				}

				void cancel() noexcept final {}

				F f_;
			};

			template<class R>
			struct scheduler_job : public job
			{
				scheduler_job(R&& r)
					: r_{std::forward<R>(r)} {}

				~scheduler_job() final {}

				void execute() noexcept final
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

				void cancel() noexcept final
				{
					cmoon::execution::set_done(std::forward<R>(r_));
				}

				R r_;
			};

			template<class F>
			struct bulk_executor_job : public job
			{
				bulk_executor_job(F&& f, typename executor_type::index_type i)
					: f_{std::forward<F>(f)}, i_{i} {}

				~bulk_executor_job() final {}

				void execute() noexcept final
				{
					try
					{
						std::invoke(std::forward<F>(f_), i_);
					}
					catch (...)
					{
						std::terminate();
					}
				}

				void cancel() noexcept final {}

				F f_;
				typename executor_type::index_type i_;
			};

			std::shared_mutex thread_access;
			std::mutex job_access;
			std::condition_variable job_notify;
			std::vector<std::thread> threads;
			std::vector<std::thread::id> thread_ids;
			std::deque<std::shared_ptr<job>> jobs;
			bool stopped {false};
			bool complete {false};
			std::size_t outstanding_work_{0};

			template<class Allocator>
			friend class scheduler_t;

			template<auto B, auto R, auto O, typename A>
			friend class sender_t;

			template<auto B, auto R, auto O, typename A>
			friend class executor_t;

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
							return stopped || !jobs.empty() || (complete && outstanding_work_ == 0);
						});

						if (stopped || (complete && outstanding_work_ == 0))
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