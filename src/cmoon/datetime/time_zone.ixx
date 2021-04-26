export module cmoon.datetime.time_zone;

import <string_view>;
import <string>;
import <vector>;

import cmoon.datetime.sys_time;
import cmoon.datetime.sys_info;

namespace cmoon::datetime
{
	export
	class time_zone
	{
		public:
			std::string_view name() const noexcept
			{
				return name_;
			}

			template<class Duration>
			sys_info get_info(const sys_time<Duration>& tp) const;

			//template<class Duration>
			//sys_time<std::common_type_t<Duration, std::chrono::seconds>>
			//to_sys(const local_time<Duration>& tp) const;

			[[nodiscard]] friend bool operator==(const time_zone& x, const time_zone& y) noexcept
			{
				return x.name() == y.name();
			}

			[[nodiscard]] friend bool operator!=(const time_zone& x, const time_zone& y) noexcept
			{
				return !(x == y);
			}
		private:
			std::string name_;
			std::vector<sys_info> info_;
	};
}