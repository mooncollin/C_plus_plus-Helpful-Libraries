export module cmoon.chrono.time_point;

import <chrono>;

namespace cmoon::chrono
{
	export
	[[nodiscard]] auto now_hh_mm_ss() noexcept
	{
		std::chrono::zoned_time zt{std::chrono::current_zone(), std::chrono::system_clock::now()};
		const auto lt = zt.get_local_time();
		const auto tod = lt - std::chrono::floor<std::chrono::days>(lt);
		return std::chrono::hh_mm_ss{tod};
	}
}