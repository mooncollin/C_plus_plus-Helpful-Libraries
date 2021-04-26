export module cmoon.datetime.month_day;

import <compare>;

import cmoon.datetime.month;
import cmoon.datetime.day_of_month;

namespace cmoon::datetime
{
	export
	class month_day
	{
		public:
			constexpr month_day() = default;
			constexpr month_day(const datetime::month& m, const day_of_month& d)
				: m_{m}, d_{d} {}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr day_of_month day() const noexcept
			{
				return d_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return m_.ok() && (unsigned{d_} >= 1 && unsigned{d_} <= static_cast<unsigned>(days_in_month(m_).count()));
			}

			[[nodiscard]] friend constexpr bool operator==(const month_day& x, const month_day& y) noexcept = default;

			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const month_day& x, const month_day& y) noexcept
			{
				const auto r1 = x.month() <=> y.month();
				if (r1 != std::strong_ordering::equal)
				{
					return r1;
				}

				return x.day() <=> y.day();
			}
		private:
			datetime::month m_ {};
			day_of_month d_ {};
	};

	export
	[[nodiscard]] constexpr month_day operator/(const month& m, const day_of_month& d) noexcept
	{
		return month_day{m, d};
	}

	export
	[[nodiscard]] constexpr month_day operator/(const month& m, const int d) noexcept
	{
		return month_day{m, day_of_month{static_cast<unsigned>(d)}};
	}

	export
	[[nodiscard]] constexpr month_day operator/(const int m, const day_of_month& d) noexcept
	{
		return month_day{month{static_cast<unsigned>(m)}, d};
	}

	export
	[[nodiscard]] constexpr month_day operator/(const day_of_month& d, const month& m) noexcept
	{
		return month_day{m, d};
	}

	export
	[[nodiscard]] constexpr month_day operator/(const day_of_month& d, const int m) noexcept
	{
		return month_day{month{static_cast<unsigned>(m)}, d};
	}
}