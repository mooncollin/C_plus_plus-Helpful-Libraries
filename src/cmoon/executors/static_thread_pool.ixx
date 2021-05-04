export module cmoon.executors.static_thread_pool;

import <cstddef>;
import <mutex>;
import <atomic>;
import <queue>;
import <functional>;
import <memory>;
import <condition_variable>;
import <shared_mutex>;
import <algorithm>;
import <execution>;
import <thread>;
import <concepts>;
import <variant>;
import <tuple>;
import <future>;

import cmoon.memory;
import cmoon.mutex;
import cmoon.execution;

namespace cmoon::executors
{
	class static_thread_pool
	{
		private:
			template<template<class, class, class, class> class Base,
						class Blocking,
						class Relationship,
						class Outstanding,
						class AllocatorType>
			class properties_impl
			{
				public:
					Base<cmoon::execution::blocking_t::never_t,
							Relationship,
							Outstanding,
							AllocatorType>
					require(const cmoon::execution::blocking_t::never_t) const
					{
						return {*this};
					}

					Base<cmoon::execution::blocking_t::possibly_t,
							Relationship,
							Outstanding,
							AllocatorType>
					require(const cmoon::execution::blocking_t::possibly_t) const
					{
						return {*this};
					}

					Base<cmoon::execution::blocking_t::always_t,
							Relationship,
							Outstanding,
							AllocatorType>
					require(const cmoon::execution::blocking_t::always_t) const
					{
						return {*this};
					}

					Base<Blocking,
							cmoon::execution::relationship_t::continuation_t,
							Outstanding,
							AllocatorType>
					require(const cmoon::execution::relationship_t::continuation_t) const
					{
						return {*this};
					}

					Base<Blocking,
							cmoon::execution::relationship_t::fork_t,
							Outstanding,
							AllocatorType>
					require(const cmoon::execution::relationship_t::fork_t) const
					{
						return {*this};
					}

					Base<Blocking,
							Relationship,
							cmoon::execution::outstanding_work_t::tracked_t,
							AllocatorType>
					require(const cmoon::execution::outstanding_work_t::tracked_t) const
					{
						return {*this};
					}

					Base<Blocking,
							Relationship,
							cmoon::execution::outstanding_work_t::untracked_t,
							AllocatorType>
					require(const cmoon::execution::outstanding_work_t::untracked_t) const
					{
						return {*this};
					}

					template<class ProtoAllocator>
					Base<Blocking,
							Relationship,
							Outstanding,
							ProtoAllocator>
					require(const cmoon::execution::allocator_t<ProtoAllocator>& other) const
					{
						return {*this, other.value()};
					}

					auto require(const cmoon::execution::allocator_t<void>&) const
					{
						return require(cmoon::execution::allocator(std::allocator<void>{}));
					}

					[[nodiscard]] static constexpr cmoon::execution::bulk_guarentee_t query(const cmoon::execution::bulk_guarentee_t) noexcept
					{
						return cmoon::execution::bulk_guarentee.parallel;
					}

					[[nodiscard]] static constexpr cmoon::execution::mapping_t query(const cmoon::execution::mapping_t) noexcept
					{
						return cmoon::execution::mapping.thread;
					}

					[[nodiscard]] static constexpr cmoon::execution::blocking_t query(const cmoon::execution::blocking_t) noexcept
					{
						return Blocking{};
					}

					[[nodiscard]] static constexpr cmoon::execution::relationship_t query(const cmoon::execution::relationship_t) noexcept
					{
						return Relationship{};
					}

					[[nodiscard]] static constexpr cmoon::execution::outstanding_work_t query(const cmoon::execution::outstanding_work_t) noexcept
					{
						return Outstanding{};
					}

					auto query(const cmoon::execution::allocator_t<void>&) const noexcept
					{
						return std::allocator<void>{};
					}

					template<class ProtoAllocator>
					auto query(const cmoon::execution::allocator_t<ProtoAllocator>&) const noexcept
					{
						return alloc;
					}
				protected:
					properties_impl(static_thread_pool& p, const AllocatorType& a = AllocatorType{})
						: pool{p}, alloc{a} {}

					static_thread_pool& pool;
					mutable AllocatorType alloc;
			};

			template<class Blocking = blocking_t::possibly_t, 
						class Relationship = relationship_t::fork_t, 
						class Outstanding = outstanding_work_t::untracked_t, 
						class AllocatorType = std::allocator<void>>
			class executor_impl : public properties_impl<executor_impl,
															Blocking,
															Relationship,
															Outstanding,
															AllocatorType>
			{
				using base = properties_impl<executor_impl,
												Blocking,
												Relationship,
												Outstanding,
												AllocatorType>;
				public:
					using shape_type = std::size_t;
					using index_type = std::size_t;

					executor_impl(const executor_impl&) noexcept = default;
					executor_impl(executor_impl&&) noexcept = default;

					executor_impl& operator=(const executor_impl&) noexcept = default;
					executor_impl& operator=(executor_impl&&) noexcept = default;

					[[nodiscard]] bool running_in_this_thread() const noexcept
					{
						return this->pool.running_in_this_thread();
					}

					template<class OtherBlocking, class OtherRelationship, class OtherOutstanding, class OtherAllocatorType>
					[[nodiscard]] friend bool operator==(const executor_impl& lhs, const executor_impl<OtherBlocking, OtherRelationship, OtherOutstanding, OtherAllocatorType>& rhs) noexcept
					{
						if constexpr (std::same_as<Blocking, OtherBlocking> &&
										std::same_as<Relationship, OtherRelationship> &&
										std::same_as<Outstanding, OtherOutstanding> &&
										std::same_as<AllocatorType, OtherAllocatorType>)
						{
							return std::addressof(lhs.pool) == std::addressof(rhs.pool);
						}
						else
						{
							return false;
						}
					}

					template<class OtherBlocking, class OtherRelationship, class OtherOutstanding, class OtherAllocatorType>
					[[nodiscard]] friend bool operator!=(const executor_impl& lhs, const executor_impl<OtherBlocking, OtherRelationship, OtherOutstanding, OtherAllocatorType>& rhs) noexcept
					{
						return !(lhs == rhs);
					}

					template<class Function>
					void execute(Function&& f) const
					{
						if (this->pool.is_stopped)
						{
							return;
						}

						if constexpr (is_tracking)
						{
							++this->pool.outstanding_work;
						}

						if constexpr (is_blocking)
						{
							try
							{
								(void) std::forward<Function>(f)();
							}
							catch (...)
							{
								std::terminate();
							}

							if constexpr (is_tracking)
							{
								--this->pool.outstanding_work;
								this->pool.outstanding_work.notify_all();
							}
						}
						else
						{
							auto task = [func = std::forward<Function>(f), this] {
								try
								{
									func();
								}
								catch (...)
								{
									std::terminate();
								}
								if constexpr (is_tracking)
								{
									--this->pool.outstanding_work;
									this->pool.outstanding_work.notify_all();
								}
							};

							auto job = std::make_unique<static_thread_pool::any_job<decltype(task)>>(std::move(task));

							std::scoped_lock l {this->pool.jobs_m};
							this->pool.jobs.push(std::move(job));
						}
					}

					template<class Function>
					void bulk_execute(Function&& f, std::size_t n) const
					{
						if (this->pool.is_stopped)
						{
							return;
						}

						if constexpr (is_blocking)
						{
							std::atomic<std::size_t> amount_left {n};
							std::unique_lock l {this->pool.jobs_m};

							if constexpr (is_tracking)
							{
								this->pool.outstanding_work += n;
							}
							for (std::size_t i {0}; i < n; ++i)
							{
								this->pool.jobs.push([func = cmoon::allocate_unique<Function>(this->alloc, f), this, i, &amount_left] {
									try
									{
										(*func)(i);
									}
									catch (...)
									{
										std::terminate();
									}

									--amount_left;
									amount_left.notify_one();
									if constexpr (is_tracking)
									{
										--this->pool.outstanding_work;
										this->pool.outstanding_work.notify_all();
									}
								});
							}

							l.unlock();
							amount_left.wait(0);
						}
						else
						{
							std::scoped_lock l {this->pool.jobs_m};

							if constexpr (is_tracking)
							{
								this->pool.outstanding_work += n;
							}

							for (std::size_t i {0}; i < n; ++i)
							{
								this->pool.jobs.push([func = cmoon::allocate_unique<Function>(this->alloc, f), this, i] {
									try
									{
										(*func)(i);
									}
									catch (...)
									{
										std::terminate();
									}

									if constexpr (is_tracking)
									{
										--this->pool.outstanding_work;
										this->pool.outstanding_work.notify_all();
									}
								});
							}
						}
					}
				private:
					static constexpr bool is_blocking = std::same_as<Blocking, cmoon::execution::blocking_t::always_t>;
					static constexpr bool is_tracking = std::same_as<Outstanding, cmoon::execution::outstanding_work_t::tracked_t>;

					executor_impl(static_thread_pool& pool, const AllocatorType& alloc = AllocatorType{})
						: base{pool, alloc} {}

					template<class OtherBlocking, class OtherRelationship, class OtherOutstanding>
					executor_impl(const executor_impl<OtherBlocking, OtherRelationship, OtherOutstanding, AllocatorType>& other)
						: base{other.pool, other.alloc} {}

					template<class OtherBlocking, class OtherRelationship, class OtherOutstanding, class OtherAllocatorType>
					executor_impl(const executor_impl<OtherBlocking, OtherRelationship, OtherOutstanding, OtherAllocatorType>& other, const AllocatorType& alloc)
						: base{other.pool, alloc} {}

					friend static_thread_pool;
			};

			template<class AllocatorType = std::allocator<void>>
			class scheduler_impl
			{
				private:
					template<class Blocking = blocking_t::possibly_t,
								class Relationship = relationship_t::fork_t,
								class Outstanding = outstanding_work_t::untracked_t,
								class AllocatorType = std::allocator<void>>
					class sender_impl : public properties_impl<sender_impl,
																Blocking,
																Relationship,
																Outstanding,
																AllocatorType>
					{
						private:
							using base = properties_impl<sender_impl,
															Blocking,
															Relationship,
															Outstanding,
															AllocatorType>;

							template<class R>
							class operation_state_impl
							{
								void start()
								{
									executor_impl<Blocking, Relationship, Outstanding, AllocatorType> exec {pool, alloc};
									exec.execute([func = std::move(r)] {
										try
										{
											if constexpr (std::is_void_v<std::invoke_result_t<R>>)
											{
												func();
												cmoon::execution::set_done(std::move(func));
											}
											else
											{
												auto&& v = func();
												cmoon::execution::set_value(std::move(func), std::move(v));
											}
										}
										catch (...)
										{
											cmoon::execution::set_error(std::move(func), std::current_exception());
										}
									});
								}

								private:
									operation_state_impl(R&& r, static_thread_pool& p, const AllocatorType alloc)
										: pool{p}, alloc{alloc}, r{std::forward<R>(r)} {}

									static_thread_pool& pool;
									AllocatorType alloc;
									R r;
									
									template<class, class, class, class>
									friend class sender_impl;
							};
						public:
							template<template<class...> class Tuple, template<class...> class Variant>
							using value_types = Variant<Tuple<>>;

							template<template<class...> class Variant>
							using error_types = Variant<std::exception_ptr>;

							static constexpr bool sends_done = true;

							sender_impl(const sender_impl&) noexcept = default;
							sender_impl(sender_impl&&) noexcept = default;

							sender_impl& operator=(const sender_impl&) noexcept = default;
							sender_impl& operator=(sender_impl&&) noexcept = default;

							[[nodiscard]] bool running_in_this_thread() const noexcept
							{
								return this->pool.running_in_this_thread();
							}

							template<class OtherBlocking, class OtherRelationship, class OtherOutstanding, class OtherAllocatorType>
							[[nodiscard]] friend bool operator==(const sender_impl& lhs, const sender_impl<OtherBlocking, OtherRelationship, OtherOutstanding, OtherAllocatorType>& rhs)
							{
								if constexpr (std::same_as<Blocking, OtherBlocking> &&
												std::same_as<Relationship, OtherRelationship> &&
												std::same_as<Outstanding, OtherOutstanding> &&
												std::same_as<AllocatorType, OtherAllocatorType>)
								{
									return std::addressof(lhs.pool) == std::addressof(rhs.pool);
								}
								else
								{
									return false;
								}
							}

							template<class OtherBlocking, class OtherRelationship, class OtherOutstanding, class OtherAllocatorType>
							[[nodiscard]] friend bool operator!=(const sender_impl& lhs, const sender_impl<OtherBlocking, OtherRelationship, OtherOutstanding, OtherAllocatorType>& rhs)
							{
								return !(lhs == rhs);
							}

							template<receiver_of<> R>
							auto connect(R&& r) const
							{
								return operation_state_impl{std::forward<R>(r), this->pool, this->alloc};
							}
						private:
							sender_impl(static_thread_pool& pool)
								: base{pool} {}

							template<class OtherBlocking, class OtherRelationship, class OtherOutstanding>
							sender_impl(const sender_impl<OtherBlocking, OtherRelationship, OtherOutstanding, AllocatorType>& other)
								: base{other.pool, other.alloc} {}

							template<class OtherBlocking, class OtherRelationship, class OtherOutstanding, class OtherAllocatorType>
							sender_impl(const sender_impl<OtherBlocking, OtherRelationship, OtherOutstanding, OtherAllocatorType>& other, const AllocatorType& alloc)
								: base{other.pool, other.alloc} {}

							template<class>
							friend class scheduler_impl;
					};
				public:
					using sender_type = sender_impl<>;
					//static_assert(cmoon::execution::typed_sender<sender_type>);

					scheduler_impl(const scheduler_impl&) noexcept = default;
					scheduler_impl(scheduler_impl&&) noexcept = default;

					scheduler_impl& operator=(const scheduler_impl&) noexcept = default;
					scheduler_impl& operator=(scheduler_impl&&) noexcept = default;

					template<class OtherAllocatorType>
					scheduler_impl<OtherAllocatorType> require(const cmoon::execution::allocator_t<OtherAllocatorType>& a) const
					{
						return {*this, a.value()};
					}

					auto require(const cmoon::execution::allocator_t<void>& a) const
					{
						return require(cmoon::execution::allocator(std::allocator<void>{}));
					}

					static_thread_pool& query(const cmoon::execution::context_t) const noexcept
					{
						return pool;
					}

					auto query(const cmoon::execution::allocator_t<void>&) const noexcept
					{
						return std::allocator<void>{};
					}

					template<class OtherAllocatorType>
					auto query(const cmoon::execution::allocator_t<OtherAllocatorType>&) const noexcept
					{
						return alloc;
					}

					bool running_in_this_thread() const noexcept
					{
						return pool.running_in_this_thread();
					}

					template<class OtherAllocatorType>
					[[nodiscard]] friend bool operator==(const scheduler_impl& a, const scheduler_impl<OtherAllocatorType>& b) noexcept
					{
						if constexpr (std::same_as<AllocatorType, OtherAllocatorType>)
						{
							return std::addressof(a.pool) == std::addressof(b.pool);
						}
						else
						{
							return false;
						}
					}

					template<class OtherAllocatorType>
					[[nodiscard]] friend bool operator!=(const scheduler_impl& a, const scheduler_impl<OtherAllocatorType>& b) noexcept
					{
						return !(a == b);
					}

					sender_type schedule() noexcept
					{

					}
				private:
					scheduler_impl(static_thread_pool& pool)
						: pool{pool} {}

					template<class OtherAllocatorType>
					scheduler_impl(const scheduler_impl<OtherAllocatorType>& other, const AllocatorType& alloc)
						: pool{other.pool}, alloc{alloc} {}

					static_thread_pool& pool;
					AllocatorType alloc;

					friend static_thread_pool;
			};
		public:
			using scheduler_type = scheduler_impl<>;
			using executor_type = executor_impl<>;

			explicit static_thread_pool(std::size_t num_threads)
				: outstanding_work{0}, is_stopped{false}
			{
				for (std::size_t i {0}; i < num_threads; ++i)
				{
					thread_ids.push_back(threads.emplace_back(&static_thread_pool::run, std::ref(*this)).get_id());
				}
			}

			static_thread_pool(const static_thread_pool&) = delete;
			static_thread_pool& operator=(const static_thread_pool&) = delete;

			~static_thread_pool()
			{
				wait();
			}

			void attach()
			{
				if (is_stopped)
				{
					return;
				}

				{
					std::scoped_lock l {thread_ids_m};
					thread_ids.push_back(std::this_thread::get_id());
				}

				++outstanding_work;
				run();
				--outstanding_work;
				outstanding_work.notify_all();
				wait();
			}

			void stop()
			{
				std::scoped_lock l {jobs_m};
				is_stopped = true;
				jobs_cv.notify_all();
			}

			void wait()
			{
				if (!is_stopped)
				{
					stop();
				}
					
				{
					std::scoped_lock l {threads_m};
					for (auto& t : threads)
					{
						t.join();
					}
					threads.clear();
				}
					
				{
					std::scoped_lock l{thread_ids_m};
					thread_ids.clear();
				}

					
				std::size_t work_left {outstanding_work.load()};
				while (work_left != 0)
				{
					outstanding_work.wait(work_left);
					work_left = outstanding_work.load();
				}
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
			class job
			{
				public:
					virtual ~job() {}
					virtual void execute() = 0;
			};

			template<class F>
			class any_job : public job
			{
				public:
					any_job(F&& f)
						: func{std::forward<F>(f)} {}

					void execute() override
					{
						func();
					}
				private:
					F func;
			};

			std::atomic<std::size_t> outstanding_work;

			std::vector<std::thread> threads;
			std::mutex threads_m;

			std::vector<std::thread::id> thread_ids;
			mutable std::shared_mutex thread_ids_m;

			bool is_stopped;

			std::queue<std::unique_ptr<job>> jobs;
			std::mutex jobs_m;
			std::condition_variable_any jobs_cv;

			void run()
			{
				while (true)
				{
					std::unique_ptr<job> j;
					{
						std::unique_lock l {jobs_m};
						jobs_cv.wait(l, [this]{ return is_stopped || !jobs.empty(); });
							
						if (jobs.empty() && is_stopped)
						{
							return;
						}

						j = std::move(jobs.front());
						jobs.pop();
					}

					j->execute();
				}
			}

			[[nodiscard]] bool running_in_this_thread() const noexcept
			{
				std::shared_lock l {thread_ids_m};
				return std::find(std::begin(thread_ids),
									std::end(thread_ids), std::this_thread::get_id()) != std::end(thread_ids);
			}

			template<class, class, class, class>
			friend class executor_impl;

			template<class>
			friend class scheduler_impl;
	};
}