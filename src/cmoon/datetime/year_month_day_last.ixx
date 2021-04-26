export module cmoon.datetime.year_month_day_last;

import <compare>;

import cmoon.measures.time;

import cmoon.datetime.year;
import cmoon.datetime.day_of_month;
import cmoon.datetime.month;
import cmoon.datetime.month_day_last;
import cmoon.datetime.to_days;

namespace cmoon::datetime
{
	export
	class year_month_day_last
	{
		public:
			constexpr year_month_day_last(const datetime::year& y, const datetime::month_day_last& mdl) noexcept
				: y_{y}, mdl_{mdl} {}

			[[nodiscard]] constexpr datetime::year year() const noexcept
			{
				return y_;
			}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return mdl_.month();
			}

			[[nodiscard]] constexpr datetime::day_of_month day() const noexcept
			{
				return last_day_of_month(month(), year());
			}

			[[nodiscard]] constexpr datetime::month_day_last month_day_last() const noexcept
			{
				return mdl_;
			}

			[[nodiscard]] friend constexpr year_month_day_last operator+(const year_month_day_last& ymdl, const cmoon::measures::months& m) noexcept
			{
				const auto r = year_month{ymdl.year(), ymdl.month()} + m;
				return year_month_day_last{r.year(), datetime::month_day_last{r.month()}};
			}

			[[nodiscard]] friend constexpr year_month_day_last operator+(const cmoon::measures::months& m, const year_month_day_last& ymdl) noexcept
			{
				return ymdl + m;
			}

			[[nodiscard]] friend constexpr year_month_day_last operator+(const year_month_day_last& ymdl, const cmoon::measures::years& y) noexcept
			{
				return year_month_day_last{ymdl.year() + y, ymdl.month_day_last()};
			}

			[[nodiscard]] friend constexpr year_month_day_last operator+(const cmoon::measures::years& y, const year_month_day_last& ymdl) noexcept
			{
				return ymdl + y;
			}

			[[nodiscard]] friend constexpr year_month_day_last operator-(const year_month_day_last& ymdl, const cmoon::measures::months& m) noexcept
			{
				return ymdl + -m;
			}

			[[nodiscard]] friend constexpr year_month_day_last operator-(const year_month_day_last& ymdl, const cmoon::measures::years& y) noexcept
			{
				return ymdl + -y;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return month().ok();
			}

			[[nodiscard]] friend constexpr bool operator==(const year_month_day_last&, const year_month_day_last&) noexcept = default;
			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year_month_day_last& x, const year_month_day_last& y) noexcept
			{
				const auto r = x.year() <=> y.year();
				if (r != std::strong_ordering::equal)
				{
					return r;
				}

				return x.month() <=> y.month();
			}

			[[nodiscard]] constexpr operator sys_days() const noexcept
			{
				return sys_days{cmoon::measures::to_chrono(to_days(year(), month(), day()))};
			}
		private:
			datetime::year y_;
			datetime::month_day_last mdl_;
	};

	export
	[[nodiscard]] constexpr year_month_day_last operator/(const year_month& ym, last_spec) noexcept
	{
		return year_month_day_last{ym.year(), month_day_last{ym.month()}};
	}

	export
	[[nodiscard]] constexpr year_month_day_last operator/(const year& y, const month_day_last& mdl) noexcept
	{
		return year_month_day_last{y, mdl};
	}

	export
	[[nodiscard]] constexpr year_month_day_last operator/(const int y, const month_day_last& mdl) noexcept
	{
		return year_month_day_last{year{y}, mdl};
	}

	export
	[[nodiscard]] constexpr year_month_day_last operator/(const month_day_last& mdl, const year& y) noexcept
	{
		return year_month_day_last{y, mdl};
	}

	export
	[[nodiscard]] constexpr year_month_day_last operator/(const month_day_last& mdl, const int y) noexcept
	{
		return year_month_day_last{year{y}, mdl};
	}
}