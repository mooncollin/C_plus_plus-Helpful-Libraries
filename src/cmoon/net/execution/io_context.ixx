export module cmoon.net.execution.io_context;

import <memory>;
import <thread>;
import <algorithm>;
import <functional>;
import <cstdint>;
import <atomic>;
import <concepts>;
import <chrono>;

import cmoon.property;
import cmoon.execution;
import cmoon.executors;

import cmoon.net.execution.execution_context;

namespace cmoon::net
{
	export
	class io_context : public execution_context
	{
		template<class Blocking = cmoon::blocking_t::possibly_t,
				 class Relationship = cmoon::relationship_t::fork_t, 
				 class Outstanding = cmoon::outstanding_work_t::untracked_t, 
				 class AllocatorType = std::allocator<void>>
		class io_context_executor : public cmoon::executors::executor_properties<executor_impl,
																				 Blocking,
																				 Relationship,
																				 Outstanding,
																				 AllocatorType>
		{
			using base = cmoon::executors::executor_properties<executor_impl,
															   Blocking,
															   Relationship,
															   Outstanding,
															   AllocatorType>;

			public:
				io_context_executor(const io_context_executor&) noexcept = default;
				io_context_executor(io_context_executor&&) noexcept = default;

				io_context_executor& operator=(const io_context_executor&) noexcept = default;
				io_context_executor& operator=(io_context_executor&&) noexcept = default;

				io_context& query(const cmoon::execution::context_t) noexcept
				{
					return ctx_;
				}

				[[nodiscard]] bool running_in_this_thread() const noexcept
				{
					return ctx_.ctx_.running_in_this_thread();
				}

				template<class Function>
				void execute(Function&& f) const
				{
					++ctx_.outstanding_work_;
					ctx_.ctx_.execute([func = std::forward<Function>(f), this]{
						func();
						--ctx_.outstanding_work_;
					});
				}

				[[nodiscard]] friend bool operator==(const io_context_executor& lhs, const io_context_executor& rhs) noexcept
				{
					return lhs.ctx_.ctx_ == rhs.ctx_.ctx_;
				}

				[[nodiscard]] friend bool operator!=(const io_context_executor& lhs, const io_context_executor& rhs) noexcept
				{
					return !(lhs == rhs);
				}

				~io_context_executor() noexcept = default;

				~io_context_executor() requires (std::same_as<Outstanding, cmoon::execution::outstanding_work_t::tracked_t>) noexcept
				{
					--ctx_.outstanding_work;
				}
			private:
				io_context& ctx_;

				io_context_executor(io_context& ctx, const AllocatorType& alloc = AllocatorType{})
					: base{alloc}, ctx_{ctx} {}

				io_context_executor(io_context& ctx, const AllocatorType& alloc = AllocatorType{}) requires (std::same_as<Outstanding, cmoon::execution::outstanding_work_t::tracked_t>)
					: base{alloc}, ctx_{ctx}
				{
					++ctx_.outstanding_work;
				}

				friend io_context;
		};

		using executor_context = cmoon::executors::static_thread_pool;
		
		public:
			using count_type = std::uint_least32_t;
			using executor_type = io_context_executor<>;

			io_context()
				: outstanding_work_{0},
				  ctx_{std::max(std::thread::hardware_concurrency(), 1)} {}

			explicit io_context(int concurrency_hint);

			io_context(const io_context&) = delete;
			io_context& operator=(const io_context&) = delete;

			executor_type get_executor() noexcept
			{
				return executor_type{*this};
			}

			template<class Rep, class Period>
			count_type run_for(const std::chrono::duration<Rep, Period>& rel_time);

			template<class Block, class Duration>
			count_type run_until(const std::chrono::time_point<Clock, Duration>& abs_time);

			count_type run_one();

			template<class Rep, class Period>
			count_type run_one_for(const std::chrono::duration<Rep, Period>& rel_time);

			template<class Clock, class Duration>
			count_type run_one_until(const std::chrono::time_point<Clock, Duration>& abs_time);

			count_type poll();

			count_type poll_one();

			void stop();

			bool stopped() const noexcept;

			void restart();
		private:
			std::atomic<count_type> outstanding_work_;
			executor_context ctx_;

			template<class Blocking, Relationship, Outstanding, AllocatorType>
			friend io_context_executor<Blocking, Relationship, Outstanding, AllocatorType>;
	};
}