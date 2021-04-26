export module cmoon.datetime.year_month_weekday;

import cmoon.measures.time;

import cmoon.datetime.sys_time;
import cmoon.datetime.year;
import cmoon.datetime.month;
import cmoon.datetime.weekday_indexed;
import cmoon.datetime.year_month_day;

namespace cmoon::datetime
{
	export
	class year_month_weekday
	{
		public:
			constexpr year_month_weekday() = default;
			constexpr year_month_weekday(const datetime::year& y, const datetime::month& m, const datetime::weekday_indexed& w)
				: y_{y}, m_{m}, w_{w} {}

			constexpr year_month_weekday(const sys_days& sys_days)
				: year_month_weekday{from_days(sys_days)} {}


			[[nodiscard]] constexpr datetime::year year() const noexcept
			{
				return y_;
			}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
			{
				return w_.weekday();
			}

			[[nodiscard]] constexpr unsigned index() const noexcept
			{
				return w_.index();
			}

			[[nodiscard]] constexpr datetime::weekday_indexed weekday_indexed() const noexcept
			{
				return w_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return month().ok() && weekday_indexed().ok();
			}

			[[nodiscard]] constexpr operator sys_days() const noexcept
			{
				return sys_days{cmoon::measures::to_chrono(to_days())};
			}

			[[nodiscard]] friend constexpr bool operator==(const year_month_weekday& x, const year_month_weekday& y) noexcept = default;

			[[nodiscard]] friend constexpr year_month_weekday operator+(const year_month_weekday& ymwd, const cmoon::measures::months& m) noexcept
			{
				const auto ym = year_month{ymwd.year(), ymwd.month()} + m;
				return {ym.year(), ym.month(), ymwd.weekday_indexed()};
			}

			[[nodiscard]] friend constexpr year_month_weekday operator+(const cmoon::measures::months& m, const year_month_weekday& ymwd) noexcept
			{
				return ymwd + m;
			}

			[[nodiscard]] friend constexpr year_month_weekday operator+(const year_month_weekday& ymwd, const cmoon::measures::years& y) noexcept
			{
				return {ymwd.year() + y, ymwd.month(), ymwd.weekday_indexed()};
			}

			[[nodiscard]] friend constexpr year_month_weekday operator+(const cmoon::measures::years& y, const year_month_weekday& ymwd) noexcept
			{
				return ymwd + y;
			}

			[[nodiscard]] friend constexpr year_month_weekday operator-(const year_month_weekday& ymwd, const cmoon::measures::months& m) noexcept
			{
				return ymwd + -m;
			}

			[[nodiscard]] friend constexpr year_month_weekday operator-(const year_month_weekday& ymwd, const cmoon::measures::years& y) noexcept
			{
				return ymwd + -y;
			}

			constexpr year_month_weekday& operator+=(const cmoon::measures::years& y) noexcept
			{
				*this = *this + y;
				return *this;
			}

			constexpr year_month_weekday& operator+=(const cmoon::measures::months& m) noexcept
			{
				*this = *this + m;
				return *this;
			}

			constexpr year_month_weekday& operator-=(const cmoon::measures::years& y) noexcept
			{
				*this = *this - y;
				return *this;
			}

			constexpr year_month_weekday& operator-=(const cmoon::measures::months& m) noexcept
			{
				*this = *this - m;
				return *this;
			}
		private:
			datetime::year y_ {};
			datetime::month m_ {};
			datetime::weekday_indexed w_ {};

			[[nodiscard]] static constexpr year_month_weekday from_days(const std::chrono::time_point<std::chrono::system_clock, cmoon::measures::chrono_t<cmoon::measures::days>>& sys_days)
			{
				const auto wd = datetime::weekday{sys_days};
				const auto ymd = datetime::year_month_day{sys_days};
				return {ymd.year(), ymd.month(), wd[(static_cast<unsigned>(ymd.day()) - 1) / 7 + 1]};
			}

			[[nodiscard]] constexpr cmoon::measures::days to_days() const noexcept
			{
				const auto d = sys_days{datetime::year_month_day{year(), month(), first}};
				return cmoon::measures::from_chrono((d + cmoon::measures::to_chrono((w_.weekday() - datetime::weekday{d} + cmoon::measures::days{(w_.index() - 1) * 7}))).time_since_epoch());
			}
	};

	export
	[[nodiscard]] constexpr year_month_weekday operator/(const year_month& ym, const weekday_indexed& w) noexcept
	{
		return year_month_weekday{ym.year(), ym.month(), w};
	}

	export
	[[nodiscard]] constexpr year_month_weekday operator/(const year& y, const month_weekday& mwd) noexcept
	{
		return year_month_weekday{y, mwd.month(), mwd.weekday_indexed()};
	}

	export
	[[nodiscard]] constexpr year_month_weekday operator/(const int y, const month_weekday& mwd) noexcept
	{
		return year_month_weekday{year{y}, mwd.month(), mwd.weekday_indexed()};
	}

	export
	[[nodiscard]] constexpr year_month_weekday operator/(const month_weekday& mwd, const year& y) noexcept
	{
		return year_month_weekday{y, mwd.month(), mwd.weekday_indexed()};
	}

	export
	[[nodiscard]] constexpr year_month_weekday operator/(const month_weekday& mwd, const int y) noexcept
	{
		return year_month_weekday{year{y}, mwd.month(), mwd.weekday_indexed()};
	}
}