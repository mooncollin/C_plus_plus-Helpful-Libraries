export module cmoon.utility.stopwatch;

import <chrono>;

namespace cmoon
{
	export
	class stopwatch
	{
		public:
			using clock_t = std::chrono::system_clock;
			using duration_t = typename clock_t::duration;

			stopwatch() noexcept
				: start_time{clock_t::now()} {}

			[[nodiscard]] duration_t get_elapsed_time() const noexcept
			{
				return clock_t::now() - start_time;
			}

			[[nodiscard]] const typename clock_t::time_point& get_start_time() const noexcept
			{
				return start_time;
			}

			void reset() noexcept
			{
				start_time = clock_t::now();
			}
		private:
			typename clock_t::time_point start_time;
	};
}