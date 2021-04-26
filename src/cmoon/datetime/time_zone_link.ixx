export module cmoon.datetime.time_zone_link;

import <string_view>;
import <string>;

namespace cmoon::datetime
{
	export
	class time_zone_link
	{
		public:
			[[nodiscard]] std::string_view name() const noexcept
			{
				return name_;
			}

			[[nodiscard]] std::string_view target() const noexcept
			{
				return target_;
			}

			[[nodiscard]] friend bool operator==(const time_zone_link& x, const time_zone_link& y) noexcept
			{
				return x.name() == y.name();
			}

			[[nodiscard]] friend bool operator!=(const time_zone_link& x, const time_zone_link& y) noexcept
			{
				return !(x == y);
			}
		private:
			std::string name_;
			std::string target_;
	};
}