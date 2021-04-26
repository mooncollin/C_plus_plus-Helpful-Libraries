export module cmoon.datetime.sys_time;

import <chrono>;

import cmoon.measures.time;

namespace cmoon::datetime
{
	export
	template<class Duration>
	using sys_time = std::chrono::time_point<std::chrono::system_clock, Duration>;

	export
	using sys_seconds = sys_time<std::chrono::seconds>;

	export
	using sys_days = sys_time<cmoon::measures::chrono_t<cmoon::measures::days>>;
}