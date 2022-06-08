export module cmoon.net.execution.system_context;

import <thread>;
import <utility>;
import <concepts>;
import <type_traits>;
import <functional>;
import <exception>;
import <mutex>;
import <condition_variable>;

import cmoon.execution;
import cmoon.executors;
import cmoon.scope;

import cmoon.net.execution.service_context;

namespace cmoon::net
{
	export
	class system_context : public service_context
	{
		public:
			struct scheduler_t
			{
				scheduler_t() noexcept = default;

				struct sender_t
				{
					template<cmoon::execution::receiver R>
					struct op
					{
						public:
							[[no_unique_address]] R r_;

							friend void tag_invoke(cmoon::execution::start_t, op& o) noexcept
							{
								o.start_helper();
							}
						private:
							void start_helper() noexcept
							{
								auto& ctx = system_context::get_context();
								std::unique_lock l{ctx.join_m};
								if (ctx.stopped_)
								{
									l.unlock();
									cmoon::execution::set_error(std::move(r_), std::make_exception_ptr(cmoon::executors::context_stopped{}));
								}
								else
								{
									++ctx.outstanding_work;
									l.unlock();
									ctx.add_job(std::move(r_));
								}
							}
					};

					template<template<class...> class Tuple, template<class...> class Variant>
					using value_types = Variant<Tuple<>>;

					template<template<class...> class Variant>
					using error_types = Variant<std::exception_ptr>;

					static constexpr bool sends_done {true};

					template<cmoon::execution::receiver R>
					friend system_context::scheduler_t::sender_t::op<R> tag_invoke(cmoon::execution::connect_t, system_context::scheduler_t::sender_t, R&& r) noexcept(std::is_nothrow_constructible_v<std::remove_cvref_t<R>, R>)
					{
						return {std::forward<R>(r)};
					}
				};

				static_assert(cmoon::execution::sender<sender_t>);

				[[nodiscard]] friend sender_t tag_invoke(cmoon::execution::schedule_t, system_context::scheduler_t) noexcept
				{
					return {};
				}

				[[nodiscard]] friend cmoon::execution::forward_progress_guarantee tag_invoke(cmoon::execution::get_forward_progress_guarantee_t, system_context::scheduler_t) noexcept
				{
					return cmoon::execution::forward_progress_guarantee::parallel;
				}

				[[nodiscard]] friend bool operator==(const scheduler_t&, const scheduler_t&) noexcept = default;
				[[nodiscard]] friend bool operator!=(const scheduler_t&, const scheduler_t&) noexcept = default;
			};

			system_context(const system_context&) = delete;
			system_context& operator=(const system_context&) = delete;
			system_context(system_context&&) = delete;
			system_context& operator=(system_context&&) = delete;

			~system_context() noexcept
			{
				stop();
				join();
			}

			scheduler_t get_scheduler() noexcept
			{
				return {};
			}

			void stop() noexcept
			{
				std::scoped_lock l{join_m};
				stopped_ = true;
			}

			[[nodiscard]] bool stopped() const noexcept
			{
				std::scoped_lock l{join_m};
				return stopped_;
			}

			void join() noexcept
			{
				std::unique_lock l{join_m};
				join_cv.wait(l, [this] { return outstanding_work == 0; });
			}

			void restart()
			{
				std::scoped_lock l{join_m};
				stopped_ = false;
			}
		private:
			system_context() noexcept = default;

			mutable std::mutex join_m;
			std::condition_variable join_cv;
			bool stopped_{false};
			std::size_t outstanding_work{0};

			template<cmoon::execution::receiver R>
			friend struct system_context::scheduler_t::sender_t::op;

			[[nodiscard]] static system_context& get_context() noexcept
			{
				static system_context ctx;
				return ctx;
			}

			template<cmoon::execution::receiver R>
			void add_job(R&& r) noexcept
			{
				std::thread{&system_context::thread_f<R>, this, std::move(r)}.detach();
			}

			template<cmoon::execution::receiver R>
			void thread_f(R&& r) noexcept
			{
				cmoon::scope_exit on_exit {[this] {
					std::scoped_lock l{join_m};
					--outstanding_work;
					join_cv.notify_all();
				}};

				if (cmoon::execution::get_stop_token(r).stop_requested())
				{
					cmoon::execution::set_done(std::move(r));
					return;
				}

				try
				{
					cmoon::execution::set_value(std::move(r));
				}
				catch (...)
				{
					cmoon::execution::set_error(std::move(r), std::current_exception());
				}
			}
	};

	export
	using system_scheduler = system_context::scheduler_t;
}