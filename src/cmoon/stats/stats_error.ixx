export module cmoon.stats.stats_error;

import <exception>;

namespace cmoon::stats
{
	class stats_error : public std::exception
	{
		public:
			stats_error() noexcept = default;
			stats_error(const stats_error& other) noexcept = default;
			stats_error& operator=(const stats_error& other) noexcept = default;
			virtual const char* what() const noexcept
			{
				return "stats error";
			}
	};
}