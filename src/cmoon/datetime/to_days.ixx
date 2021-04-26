export module cmoon.datetime.to_days;

import <cstdint>;

import cmoon.datetime.year;
import cmoon.datetime.day_of_month;
import cmoon.datetime.month;

import cmoon.measures.time;

namespace cmoon::datetime
{
	export
	[[nodiscard]] constexpr cmoon::measures::days to_days(const datetime::year& y_, const datetime::month& m_, const datetime::day_of_month& d_) noexcept
	{
		const auto y = static_cast<std::int64_t>(y_) - (m_ <= datetime::february);
		const auto m = static_cast<unsigned>(m_);
		const auto d = static_cast<unsigned>(d_);
		const auto era = (y >= 0 ? y : y - 399) / 400;
		const auto yoe = static_cast<unsigned>(y - era * 400);
		const auto doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1;
		const auto doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
		return cmoon::measures::days{era * 146097 + static_cast<std::int64_t>(doe) - 719468};
	}
}