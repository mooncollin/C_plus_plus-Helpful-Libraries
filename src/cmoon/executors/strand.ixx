export module cmoon.executors.strand;

import <concepts>;
import <type_traits>;
import <utility>;
import <thread>;
import <mutex>;
import <condition_variable>;
import <deque>;
import <memory>;
import <exception>;

import cmoon.execution;
import cmoon.memory;

namespace cmoon::executors
{
	struct job
	{
		virtual void execute() noexcept = 0;
		virtual ~job() noexcept = default;
	};

	template<cmoon::execution::operation_state OP>
	struct scheduler_job : public job
	{
		scheduler_job(OP&& op)
			: op_{std::forward<OP>(op)} {}

		[[no_unique_address]] OP op_;

		void execute() noexcept final
		{
			cmoon::execution::start(op_);
		}
	};

	export
	template<cmoon::execution::scheduler Sch, class InAlloc = std::allocator<void>>
	class strand
	{
		public:
			using allocator_type = typename std::allocator_traits<InAlloc>::template rebind_alloc<job>;
		private:
			struct state_t
			{
				using deque_value_t = typename std::allocator_traits<allocator_type>::template rebind_alloc<cmoon::allocate_unique_t<job, allocator_type>>;

				state_t(const allocator_type& a)
					: alloc{a}, jobs{a} {}

				allocator_type alloc;
				std::deque<cmoon::allocate_unique_t<job, allocator_type>, deque_value_t> jobs;
				bool can_execute {true};
				std::mutex jobs_m;
				std::condition_variable jobs_cv;
			};
		public:
			template<cmoon::execution::receiver R>
			struct receiver_t
			{
				state_t* state;
				R r_;

				template<class... Args>
				friend void tag_invoke(cmoon::execution::set_value_t, strand::receiver_t<R>&& r, Args&&... args) noexcept
				{
					try
					{
						cmoon::execution::set_value(std::move(r.r_), std::forward<Args>(args)...);
					}
					catch (...)
					{
						cmoon::execution::set_error(std::move(r.r_), std::current_exception());
					}

					std::scoped_lock l{state->jobs_m};
					state->can_execute = true;
					state->jobs_cv.notify_one();
				}

				template<class E>
				friend void tag_invoke(cmoon::execution::set_error_t, strand::receiver_t<R>&& r, E&& e) noexcept
				{
					cmoon::execution::set_error(std::move(r.r_), std::current_exception());
					std::scoped_lock l{state->jobs_m};
					state->can_execute = true;
					state->jobs_cv.notify_one();
				}

				friend void tag_invoke(cmoon::execution::set_done_t, strand::receiver_t<R>&& r) noexcept
				{
					cmoon::execution::set_done(std::move(r.r_), std::current_exception());
					std::scoped_lock l{state->jobs_m};
					state->can_execute = true;
					state->jobs_cv.notify_one();
				}
			};

			struct scheduler_t
			{
				struct sender_t : public cmoon::execution::sender_base
				{
					strand* owner;

					template<cmoon::execution::operation_state OP2>
					struct op
					{
						public:
							op(state_t* state, OP2&& op2)
								: state{state},
								  op2_{std::forward<OP2>(op2)} {}

							state_t* state;
							OP2 op2_;

							friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
							{
								o.start_helper();
							}
						private:
							void start_helper() noexcept
							{
								std::scoped_lock l {state->job_m};
								state->jobs.push_back(cmoon::allocate_unique<scheduler_job>(
									state->alloc,
									std::move(op2_)
								));

								if (state->can_execute)
								{
									state->jobs_cv.notify_one();
								}
							}
					};

					template<cmoon::execution::receiver R>
					friend strand::scheduler_t::sender_t::op<R> tag_invoke(cmoon::execution::connect_t, strand::scheduler_t::sender_t s, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return s.connect_helper(std::forward<R>(r));
					}

					private:
						template<cmoon::execution::receiver R>
						strand::scheduler_t::sender_t::op<R> connect_helper(R&& r)
						{
							return {owner->state.get(),
									cmoon::execution::connect(
										cmoon::execution::schedule(owner->sch),
										strand::receiver_t<R>{owner->state.get(), std::forward<R>(r)}
									)};
						}
				};

				strand* owner;

				[[nodiscard]] friend sender_t tag_invoke(cmoon::execution::schedule_t, strand::scheduler_t t) noexcept
				{
					return {t.owner};
				}

				[[nodiscard]] friend cmoon::execution::forward_progress_guarantee tag_invoke(cmoon::execution::get_forward_progress_guarantee_t, strand::scheduler_t) noexcept
				{
					return cmoon::execution::forward_progress_guarantee::weakly_parallel;
				}

				[[nodiscard]] friend bool operator==(const strand::scheduler_t&, const strand::scheduler_t&) noexcept = default;
				[[nodiscard]] friend bool operator!=(const strand::scheduler_t&, const strand::scheduler_t&) noexcept = default;
			};

			strand(const InAlloc& alloc = {}) requires(std::default_initializable<Sch>)
				: state{create_state(alloc)}
			{
				std::thread{&strand::thread_loop, state}.detach();
			}

			explicit strand(Sch&& sch, const InAlloc& alloc = {})
				: sch{std::forward<Sch>(sch)},state{create_state(alloc)}
			{
				std::thread{&strand::thread_loop, state}.detach();
			}

			strand(const strand& other) noexcept
				: sch{other.sch}
			{
				std::scoped_lock l{other.state->jobs_m};
				state = other.state;
			}

			strand(strand&& other) noexcept
				: sch{other.sch}
			{
				std::scoped_lock l{other.state->jobs_m};
				state = std::move(other.state);
			}

			strand& operator=(const strand& other) noexcept
			{
				if (this != std::addressof(other))
				{
					std::scoped_lock l{other.state->jobs_m};
					state = other.state;
				}

				return *this;
			}

			strand& operator=(strand&& other) noexcept
			{
				if (this != std::addressof(other))
				{
					std::scoped_lock l{other.state->jobs_m};
					state = std::move(other.state);
				}

				return *this;
			}

			~strand() noexcept
			{
				std::scoped_lock l{state->jobs_m};
				state->jobs_cv.notify_one();
				state.reset();
			}

			[[nodiscard]] strand::scheduler_t get_scheduler() noexcept
			{
				return {this};
			}

			[[nodiscard]] friend bool operator==(const strand& lhs, const strand& rhs) noexcept
			{
				return lhs.state == rhs.state;
			}

			[[nodiscard]] friend bool operator!=(const strand&, const strand&) noexcept = default;
		private:
			Sch sch;
			std::shared_ptr<state_t> state;

			friend struct sender_t;

			static std::shared_ptr<state_t> create_state(const allocator_type& a)
			{
				return std::allocate_shared<state_t>(a, a);
			}

			static void thread_loop(std::shared_ptr<state_t> state)
			{
				while (true)
				{
					cmoon::allocate_unique_t<job, allocator_type> j{nullptr, state->alloc};
					{
						std::unique_lock l{state->jobs_m};
						state->jobs_cv.wait(l, [&state] {
							return (state->can_execute && !state->jobs.empty()) ||
								   (state.use_count() == 1 && state->jobs.empty());
						});

						if (state.use_count() == 1 && state->jobs.empty())
						{
							return;
						}

						j = std::move(state->jobs.front());
						state->jobs.pop_front();
						state->can_execute = false;
					}

					j->execute();
				}
			}
	};
}