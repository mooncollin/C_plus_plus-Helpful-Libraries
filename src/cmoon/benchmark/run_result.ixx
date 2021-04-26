export module cmoon.benchmark.run_result;

import <vector>;
import <chrono>;
import <cstddef>;

namespace cmoon::benchmark
{
	export
	class run_result
	{
		public:
			run_result() noexcept = default;

			[[nodiscard]] const std::vector<std::chrono::nanoseconds>& iterations() const noexcept
			{
				return iterations_;
			}

			[[nodiscard]] std::size_t amount_iterations() const noexcept
			{
				return iterations_.size();
			}

			[[nodiscard]] std::chrono::nanoseconds fastest() const noexcept
			{
				return fastest_;
			}

			[[nodiscard]] std::chrono::nanoseconds slowest() const noexcept
			{
				return slowest_;
			}

			[[nodiscard]] std::chrono::nanoseconds average() const noexcept
			{
				return average_;
			}

			void add_iteration(const std::chrono::nanoseconds& i)
			{
				iterations_.push_back(i);
				if (std::size(iterations_) == 1)
				{
					fastest_ = i;
					slowest_ = i;
				}
				else if (i < fastest_)
				{
					fastest_ = i;
				}
				else if (i > slowest_)
				{
					slowest_ = i;
				}

				running_sum += i;
				average_ = running_sum / std::size(iterations_);
			}

			[[nodiscard]] std::chrono::nanoseconds total_time() const noexcept
			{
				return running_sum;
			}
		private:
			std::vector<std::chrono::nanoseconds> iterations_;
			std::chrono::nanoseconds fastest_ {std::chrono::nanoseconds::zero()};
			std::chrono::nanoseconds slowest_ {std::chrono::nanoseconds::zero()};
			std::chrono::nanoseconds average_ {std::chrono::nanoseconds::zero()};

			std::chrono::nanoseconds running_sum {std::chrono::nanoseconds::zero()};
	};
}