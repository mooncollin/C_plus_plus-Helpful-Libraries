export module cmoon.datetime.day_of_year;

import <compare>;
import <ctime>;

import cmoon.measures.time;
import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.write_string_view;

namespace cmoon::datetime
{
	export
	class day_of_year
	{
		public:
			constexpr explicit day_of_year() noexcept = default;
			constexpr explicit day_of_year(unsigned v) noexcept
				: value{v} {}

			[[nodiscard]] friend constexpr day_of_year operator+(const day_of_year& d, const cmoon::measures::days& ds) noexcept
			{
				return day_of_year{static_cast<unsigned>(ds.count() + d.value)};
			}

			[[nodiscard]] friend constexpr day_of_year operator+(const cmoon::measures::days& ds, const day_of_year& d) noexcept
			{
				return day_of_year{static_cast<unsigned>(ds.count() + d.value)};
			}

			[[nodiscard]] friend constexpr day_of_year operator-(const day_of_year& d, const cmoon::measures::days& ds) noexcept
			{
				return day_of_year{static_cast<unsigned>(d.value - ds.count())};
			}

			[[nodiscard]] friend constexpr cmoon::measures::days operator-(const day_of_year& d1, const day_of_year& d2) noexcept
			{
				return cmoon::measures::days{d1.value - d2.value};
			}

			constexpr day_of_year& operator+=(const cmoon::measures::days& ds) noexcept
			{
				*this = *this + ds;
				return *this;
			}

			constexpr day_of_year& operator-=(const cmoon::measures::days& ds) noexcept
			{
				*this = *this - ds;
				return *this;
			}

			constexpr day_of_year& operator++() noexcept
			{
				*this += cmoon::measures::days{1};
				return *this;
			}

			constexpr day_of_year operator++(int) noexcept
			{
				const auto v = *this;
				++* this;
				return v;
			}

			constexpr day_of_year& operator--() noexcept
			{
				*this -= cmoon::measures::days{1};
				return *this;
			}

			constexpr day_of_year operator--(int) noexcept
			{
				const auto v = *this;
				--* this;
				return v;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return value >= 1u && value <= 365u;
			}

			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const day_of_year&, const day_of_year&) = default;

			[[nodiscard]] constexpr operator unsigned() const noexcept
			{
				return value;
			}
		private:
			unsigned value {};
	};
}

namespace cmoon
{
	export
	template<class CharT>
	struct formatter<datetime::day_of_year, CharT> : public base_chrono_formatter<datetime::day_of_year, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: tm_handler<CharT, OutputIt>{ value, ctx } {}

				void on_abbr_month() { this->throw_not_supported(); }
				void on_full_month() { this->throw_not_supported(); }
				void on_abbr_weekday() { this->throw_not_supported(); }
				void on_full_weekday() { this->throw_not_supported(); }
				void on_iso_abbr_year() { this->throw_not_supported(); }
				void on_iso_year() { this->throw_not_supported(); }
				void on_week_sunday(const details::numeric_system) { this->throw_not_supported(); }
				void on_week_monday(const details::numeric_system) { this->throw_not_supported(); }
				void on_us_date() { this->throw_not_supported(); }
				void on_iso_date() { this->throw_not_supported(); }
				void on_loc_date(const details::numeric_system) { this->throw_not_supported(); }
				void on_datetime(const details::numeric_system) { this->throw_not_supported(); }
				void on_utc_offset(const details::numeric_system) { this->throw_not_supported(); }
				void on_tz_name() { this->throw_not_supported(); }
				void on_iso_week(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec0_weekday(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec1_weekday(const details::numeric_system) { this->throw_not_supported(); }
				void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
				void on_century(const details::numeric_system) { this->throw_not_supported(); }
				void on_abbr_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_offset_year() { this->throw_not_supported(); }
				void on_year(const details::numeric_system) { this->throw_not_supported(); }
				void on_24_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_12_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_minute(const details::numeric_system) { this->throw_not_supported(); }
				void on_second(const details::numeric_system) { this->throw_not_supported(); }
				void on_loc_time(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_zero(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_space(const details::numeric_system) { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::day_of_year& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time{ .tm_yday = static_cast<int>(unsigned{val} - 1) };
				handler h{ time, ctx };
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}