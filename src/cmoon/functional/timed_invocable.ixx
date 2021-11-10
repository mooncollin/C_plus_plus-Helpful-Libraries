export module cmoon.functional.timed_invocable;

import <chrono>;
import <utility>;
import <functional>;
import <type_traits>;
import <thread>;

namespace cmoon
{
	export
	template<class F>
	class timed_invocable
	{
		public:
			using clock_t = std::chrono::system_clock;
			using duration_t = typename clock_t::duration;

			timed_invocable(F&& f, const std::chrono::time_point<clock_t, duration_t>& tp) noexcept(std::is_nothrow_constructible_v<F, std::remove_cvref_t<F>>)
				: f_{std::forward<F>(f)}, execute_time{tp} {}

			timed_invocable(F&& f, const duration_t& d) noexcept(std::is_nothrow_constructible_v<F, std::remove_cvref_t<F>>)
				: f_{std::forward<F>(f)}, execute_time{clock_t::now() + d} {}

			[[nodiscard]] const std::chrono::time_point<clock_t, duration_t>& get_execute_time() const noexcept
			{
				return execute_time;
			}

			[[nodiscard]] bool expired() const noexcept
			{
				return clock_t::now() >= execute_time;
			}

			void wait() noexcept
			{
				std::this_thread::sleep_until(execute_time);
			}

			template<class... Args>
			auto operator()(Args&&... args) noexcept(std::is_nothrow_invocable<F, Args...>)
			{
				wait();
				std::invoke(std::forward(f_), std::forward<Args>(args)...);
			}

			void cancel() noexcept
			{
				canceled = true;
			}

			[[nodiscard]] bool is_canceled() const noexcept
			{
				return canceled;
			}
		private:
			F f_;
			std::chrono::time_point<clock_t, duration_t> execute_time;
			bool canceled {false};
	};
}