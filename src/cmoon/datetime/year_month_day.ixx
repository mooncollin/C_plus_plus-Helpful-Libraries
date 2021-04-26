export module cmoon.datetime.year_month_day;

import <compare>;
import <utility>;
import <ctime>;

import cmoon.measures.time;
import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.write_string_view;

import cmoon.datetime.sys_time;
import cmoon.datetime.year;
import cmoon.datetime.day_of_month;
import cmoon.datetime.month;
import cmoon.datetime.to_days;

namespace cmoon::datetime
{
	export
	class year_month_day
	{
		static constexpr auto epoch_year = datetime::year{1970};
		static constexpr auto epoch_month = datetime::january;
		static constexpr auto epoch_day = datetime::first;

		public:
			constexpr year_month_day() = default;
			constexpr year_month_day(const datetime::year& y, const datetime::month& m, const datetime::day_of_month& d)
				: y_{y}, m_{m}, d_{d} {}

			constexpr year_month_day(const year_month_day_last& ymdl) noexcept
				: y_{ymdl.year()}, m_{ymdl.month()}, d_{ymdl.day()} {}

			constexpr year_month_day(const sys_days& sys_days)
				: year_month_day{from_days(cmoon::measures::from_chrono(sys_days.time_since_epoch()))} {}

			[[nodiscard]] constexpr datetime::year year() const noexcept
			{
				return y_;
			}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr datetime::day_of_month day() const noexcept
			{
				return d_;
			}

			[[nodiscard]] constexpr operator sys_days() const noexcept
			{
				return sys_days{cmoon::measures::to_chrono(to_days(year(), month(), day()))};
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return month().ok() && (day() >= 1u && day() <= last_day_of_month(month(), year()));
			}

			[[nodiscard]] friend constexpr bool operator==(const year_month_day& x, const year_month_day& y) noexcept
			{
				return x.year() == y.year() &&
						x.month() == y.month() &&
						x.day() == y.day();
			}

			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year_month_day& x, const year_month_day& y) noexcept
			{
				if (const auto r = x.year() <=> y.year(); r != std::strong_ordering::equal)
				{
					return r;
				}

				if (const auto r = x.month() <=> x.month(); r != std::strong_ordering::equal)
				{
					return r;
				}

				return x.day() <=> y.day();
			}

			[[nodiscard]] friend constexpr year_month_day operator+(const year_month_day& ymd, const cmoon::measures::months& m) noexcept
			{
				const auto r = year_month{ymd.year(), ymd.month()} + m;
				return year_month_day{r.year(), r.month(), ymd.day()};
			}

			[[nodiscard]] friend constexpr year_month_day operator+(const cmoon::measures::months& m, const year_month_day& ymd) noexcept
			{
				return ymd + m;
			}

			[[nodiscard]] friend constexpr year_month_day operator+(const year_month_day& ymd, const cmoon::measures::years& y) noexcept
			{
				return year_month_day{ymd.year() + y, ymd.month(), ymd.day()};
			}

			[[nodiscard]] friend constexpr year_month_day operator+(const cmoon::measures::years& y, const year_month_day& ymd) noexcept
			{
				return ymd + y;
			}

			[[nodiscard]] friend constexpr year_month_day operator+(const year_month_day& ymd, const cmoon::measures::days& d) noexcept
			{
				const auto system_days = static_cast<sys_days>(ymd);

				return year_month_day{system_days + cmoon::measures::to_chrono(d)};
			}

			[[nodiscard]] friend constexpr year_month_day operator+(const cmoon::measures::days& d, const year_month_day& ymd) noexcept
			{
				return ymd + d;
			}

			[[nodiscard]] friend constexpr year_month_day operator-(const year_month_day& ymd, const cmoon::measures::months& m) noexcept
			{
				return ymd + -m;
			}

			[[nodiscard]] friend constexpr year_month_day operator-(const year_month_day& ymd, const cmoon::measures::years& y) noexcept
			{
				return ymd + -y;
			}

			[[nodiscard]] friend constexpr year_month_day operator-(const year_month_day& ymd, const cmoon::measures::days& d) noexcept
			{
				return ymd + -d;
			}
		private:
			datetime::year y_ {};
			datetime::month m_ {};
			datetime::day_of_month d_ {};

			[[nodiscard]] static constexpr year_month_day from_days(const cmoon::measures::days& dp) noexcept
			{
				const auto z = dp.count() + 719468;
				const auto era = (z >= 0 ? z : z - 146096) / 146097;
				const auto doe = static_cast<unsigned>(z - era * 146097);
				const auto yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
				const auto y = static_cast<typename cmoon::measures::days::rep>(yoe) + era * 400;
				const auto doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
				const auto mp = (5 * doy + 2) / 153;
				const auto d = doy - (153 * mp + 2) / 5 + 1;
				const auto m = mp < 10 ? mp + 3 : mp - 9;
				return year_month_day{datetime::year{y + (m <= 2)}, datetime::month{m}, datetime::day_of_month{d}};
			}
	};

	export
	[[nodiscard]] constexpr year_month_day operator/(const year_month& ym, const day_of_month& d) noexcept
	{
		return year_month_day{ym.year(), ym.month(), d};
	}

	export
	[[nodiscard]] constexpr year_month_day operator/(const year_month& ym, const int d) noexcept
	{
		return year_month_day{ym.year(), ym.month(), day_of_month{static_cast<unsigned>(d)}};
	}

	export
	[[nodiscard]] constexpr year_month_day operator/(const year& y, const month_day& md) noexcept
	{
		return year_month_day{y, md.month(), md.day()};
	}

	export
	[[nodiscard]] constexpr year_month_day operator/(const int y, const month_day& md) noexcept
	{
		return year_month_day{year{y}, md.month(), md.day()};
	}

	export
	[[nodiscard]] constexpr year_month_day operator/(const month_day& md, const year& y) noexcept
	{
		return year_month_day{y, md.month(), md.day()};
	}

	export
	[[nodiscard]] constexpr year_month_day operator/(const month_day& md, const int y) noexcept
	{
		return year_month_day{year{y}, md.month(), md.day()};
	}
}

namespace cmoon
{
	export
	template<class CharT>
	struct formatter<datetime::year_month_day, CharT> : public base_chrono_formatter<datetime::year_month_day, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_datetime(const details::numeric_system) { this->throw_not_supported(); }
				void on_utc_offset(const details::numeric_system) { this->throw_not_supported(); }
				void on_tz_name() { this->throw_not_supported(); }
				void on_24_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_12_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_minute(const details::numeric_system) { this->throw_not_supported(); }
				void on_second(const details::numeric_system) { this->throw_not_supported(); }
				void on_loc_time(const details::numeric_system) { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::year_month_day& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				std::tm time {.tm_mday = static_cast<int>(unsigned{val.day()}),
							  .tm_mon = static_cast<int>(unsigned{val.month()} - 1),
							  .tm_year = static_cast<int>(val.year()) - 1900};

				std::mktime(std::addressof(time));

				handler h{time, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}