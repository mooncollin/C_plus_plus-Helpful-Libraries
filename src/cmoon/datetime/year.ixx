export module cmoon.datetime.year;

import <cstdint>;

import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.string_view;

namespace cmoon::datetime
{
	export
	class year
	{
		public:
			constexpr explicit year() noexcept = default;
			constexpr explicit year(std::int64_t v) noexcept
				: value{v} {}

			[[nodiscard]] friend constexpr year operator+(const year& y, const cmoon::measures::years& ys) noexcept
			{
				return year{static_cast<std::int64_t>(ys.count() + y.value)};
			}

			[[nodiscard]] friend constexpr year operator+(const cmoon::measures::years& ys, const year& y) noexcept
			{
				return year{static_cast<std::int64_t>(ys.count() + y.value)};
			}

			[[nodiscard]] friend constexpr year operator-(const year& y, const cmoon::measures::years& ys) noexcept
			{
				return year{static_cast<std::int64_t>(y.value - ys.count())};
			}

			[[nodiscard]] friend constexpr cmoon::measures::years operator-(const year& y1, const year& y2) noexcept
			{
				return cmoon::measures::years{y1.value - y2.value};
			}

			constexpr year& operator+=(const cmoon::measures::years& ys) noexcept
			{
				*this = *this + ys;
				return *this;
			}

			constexpr year& operator-=(const cmoon::measures::years& ys) noexcept
			{
				*this = *this - ys;
				return *this;
			}

			constexpr year& operator++() noexcept
			{
				*this += cmoon::measures::years{1};
				return *this;
			}

			constexpr year operator++(int) noexcept
			{
				const auto v = *this;
				++*this;
				return v;
			}

			constexpr year& operator--() noexcept
			{
				*this -= cmoon::measures::years{1};
				return *this;
			}

			constexpr year operator--(int) noexcept
			{
				const auto v = *this;
				--* this;
				return v;
			}

			[[nodiscard]] constexpr bool is_leap() const noexcept
			{
				return (value % 4 == 0 && value % 100 != 0) || value % 400 == 0;
			}

			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year&, const year&) = default;

			[[nodiscard]] explicit constexpr operator std::int64_t() const noexcept
			{
				return value;
			}

			[[nodiscard]] explicit constexpr operator int() const noexcept
			{
				return static_cast<int>(value);
			}

		private:
			std::int64_t value {};
	};
}

namespace cmoon
{
	template<class CharT>
	struct formatter<datetime::year, CharT> : public base_chrono_formatter<datetime::year, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_abbr_month() { this->throw_not_supported(); }
				void on_full_month() { this->throw_not_supported(); }
				void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
				void on_abbr_weekday() { this->throw_not_supported(); }
				void on_full_weekday() { this->throw_not_supported(); }
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
				void on_24_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_12_hour(const details::numeric_system) { this->throw_not_supported(); }
				void on_minute(const details::numeric_system) { this->throw_not_supported(); }
				void on_second(const details::numeric_system) { this->throw_not_supported(); }
				void on_loc_time(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_zero(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_month_space(const details::numeric_system) { this->throw_not_supported(); }
				void on_day_of_year() { this->throw_not_supported(); }
			};
		public:
			template<class OutputIt>
			auto format(const datetime::year& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const std::tm time {.tm_year = static_cast<int>(val) - 1900};
				handler h{time, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}