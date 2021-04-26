export module cmoon.datetime.year_month_weekday_last;

import cmoon.measures.time;

import cmoon.datetime.sys_time;
import cmoon.datetime.year;
import cmoon.datetime.month;
import cmoon.datetime.weekday;
import cmoon.datetime.weekday_last;

namespace cmoon::datetime
{
	export
	class year_month_weekday_last
	{
		public:
			constexpr year_month_weekday_last(const datetime::year& y, const datetime::month& m, const datetime::weekday_last& w)
				: y_{y}, m_{m}, w_{w} {}

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

			[[nodiscard]] constexpr datetime::weekday_last weekday_last() const noexcept
			{
				return w_;
			}

			[[nodiscard]] constexpr operator sys_days() const noexcept
			{
				return sys_days{cmoon::measures::to_chrono(to_days())};
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return month().ok() && weekday().ok();
			}

			[[nodiscard]] friend constexpr bool operator==(const year_month_weekday_last&, const year_month_weekday_last&) noexcept = default;

			[[nodiscard]] friend constexpr year_month_weekday_last operator+(const year_month_weekday_last& ymwdl, const cmoon::measures::months& m) noexcept
			{
				const auto ym = year_month{ymwdl.year(), ymwdl.month()} + m;
				return year_month_weekday_last{ym.year(), ym.month(), ymwdl.weekday_last()};
			}

			[[nodiscard]] friend constexpr year_month_weekday_last operator+(const cmoon::measures::months& m, const year_month_weekday_last& ymwdl) noexcept
			{
				return ymwdl + m;
			}

			[[nodiscard]] friend constexpr year_month_weekday_last operator+(const year_month_weekday_last& ymwdl, const cmoon::measures::years& y) noexcept
			{
				return year_month_weekday_last{ymwdl.year() + y, ymwdl.month(), ymwdl.weekday_last()};
			}

			[[nodiscard]] friend constexpr year_month_weekday_last operator+(const cmoon::measures::years& y, const year_month_weekday_last& ymwdl) noexcept
			{
				return ymwdl + y;
			}

			[[nodiscard]] friend constexpr year_month_weekday_last operator-(const year_month_weekday_last& ymwdl, const cmoon::measures::months& m) noexcept
			{
				return ymwdl + -m;
			}

			[[nodiscard]] friend constexpr year_month_weekday_last operator-(const year_month_weekday_last& ymwdl, const cmoon::measures::years& y) noexcept
			{
				return ymwdl + -y;
			}

			constexpr year_month_weekday_last& operator+=(const cmoon::measures::years& y) noexcept
			{
				*this = *this + y;
				return *this;
			}

			constexpr year_month_weekday_last& operator+=(const cmoon::measures::months& m) noexcept
			{
				*this = *this + m;
				return *this;
			}

			constexpr year_month_weekday_last& operator-=(const cmoon::measures::years& y) noexcept
			{
				*this = *this - y;
				return *this;
			}

			constexpr year_month_weekday_last& operator-=(const cmoon::measures::months& m) noexcept
			{
				*this = *this - m;
				return *this;
			}
		private:
			datetime::year y_;
			datetime::month m_;
			datetime::weekday_last w_;

			[[nodiscard]] constexpr cmoon::measures::days to_days() const noexcept
			{
				const auto d = sys_days{year_month_day_last{y_, month_day_last{m_}}};
				return cmoon::measures::from_chrono((d - cmoon::measures::to_chrono((datetime::weekday{d} - w_.weekday()))).time_since_epoch());
			}
	};

	export
	[[nodiscard]] constexpr year_month_weekday_last operator/(const year_month& ym, const weekday_last& w) noexcept
	{
		return year_month_weekday_last{ym.year(), ym.month(), w};
	}

	export
	[[nodiscard]] constexpr year_month_weekday_last operator/(const year& y, const month_weekday_last& mwdl) noexcept
	{
		return year_month_weekday_last{y, mwdl.month(), mwdl.weekday_last()};
	}

	export
	[[nodiscard]] constexpr year_month_weekday_last operator/(const int y, const month_weekday_last& mwdl) noexcept
	{
		return year_month_weekday_last{year{y}, mwdl.month(), mwdl.weekday_last()};
	}

	export
	[[nodiscard]] constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl, const year& y) noexcept
	{
		return year_month_weekday_last{y, mwdl.month(), mwdl.weekday_last()};
	}

	export
	[[nodiscard]] constexpr year_month_weekday_last operator/(const month_weekday_last& mwdl, const int y) noexcept
	{
		return year_month_weekday_last{year{y}, mwdl.month(), mwdl.weekday_last()};
	}
}