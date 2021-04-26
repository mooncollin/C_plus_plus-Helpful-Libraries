export module cmoon.datetime.hh_mm_ss;

import <concepts>;

import cmoon.measures.time;
import cmoon.measures.format;
import cmoon.measures.format.write_string_view;
import cmoon.measures.chrono;

namespace cmoon::datetime
{
	export
	template<class Duration>
	class hh_mm_ss;

	template<class Duration>
	class base_hh_mm_ss
	{
		using precision = Duration;
		public:
			constexpr base_hh_mm_ss() = default;
			explicit constexpr base_hh_mm_ss(precision since_midnight)
				: value_{since_midnight} {}

			[[nodiscard]] constexpr explicit operator precision() const noexcept
			{
				return value_;
			}

			constexpr precision to_duration() const noexcept
			{
				return value_;
			}

			[[nodiscard]] constexpr cmoon::measures::hours hours() const noexcept
			{
				return cmoon::measures::unit_cast<cmoon::measures::hours>(value_);
			}

			[[nodiscard]] constexpr cmoon::measures::minutes minutes() const noexcept
				requires(!std::same_as<precision, cmoon::measures::hours>)
			{
				return cmoon::measures::unit_cast<cmoon::measures::minutes>(value_) - cmoon::measures::unit_cast<cmoon::measures::minutes>(hours());
			}

			[[nodiscard]] constexpr cmoon::measures::seconds seconds() const noexcept
				requires(!std::same_as<precision, cmoon::measures::minutes> && !std::same_as<precision, cmoon::measures::hours>)
			{
				return cmoon::measures::unit_cast<cmoon::measures::seconds>(value_) - cmoon::measures::unit_cast<cmoon::measures::seconds>(minutes()) - cmoon::measures::unit_cast<cmoon::measures::seconds>(hours());
			}

			[[nodiscard]] constexpr precision subseconds() const noexcept
				requires(!std::same_as<precision, cmoon::measures::seconds> && !std::same_as<precision, cmoon::measures::minutes> && !std::same_as<precision, cmoon::measures::hours>)
			{
				return value_ - cmoon::measures::unit_cast<precision>(seconds()) - cmoon::measures::unit_cast<precision>(minutes()) - cmoon::measures::unit_cast<precision>(hours());
			}
		private:
			precision value_;
	};

	export
	template<class Rep>
	class hh_mm_ss<cmoon::measures::basic_hours<Rep>> : public base_hh_mm_ss<cmoon::measures::basic_hours<Rep>>
	{
		using precision = cmoon::measures::basic_hours<Rep>;

		public:
			constexpr hh_mm_ss() = default;
			explicit constexpr hh_mm_ss(precision since_midnight)
				: base_hh_mm_ss<cmoon::measures::basic_hours<Rep>>{since_midnight} {}
	};

	export
	template<class Rep>
	class hh_mm_ss<cmoon::measures::basic_minutes<Rep>> : public base_hh_mm_ss<cmoon::measures::basic_minutes<Rep>>
	{
		using precision = cmoon::measures::basic_minutes<Rep>;

		public:
			constexpr hh_mm_ss() = default;
			explicit constexpr hh_mm_ss(precision since_midnight)
				: base_hh_mm_ss<cmoon::measures::basic_minutes<Rep>>{since_midnight} {}
	};

	export
	template<class Rep>
	class hh_mm_ss<cmoon::measures::basic_seconds<Rep>> : public base_hh_mm_ss<cmoon::measures::basic_seconds<Rep>>
	{
		using precision = cmoon::measures::basic_seconds<Rep>;

		public:
			constexpr hh_mm_ss() = default;
			explicit constexpr hh_mm_ss(precision since_midnight)
				: base_hh_mm_ss<cmoon::measures::basic_seconds<Rep>>{since_midnight} {}
	};

	export
	template<class Rep, class Period>
	class hh_mm_ss<cmoon::measures::time<Rep, cmoon::measures::metric_system, Period, 1>> : public base_hh_mm_ss<cmoon::measures::time<Rep, cmoon::measures::metric_system, Period, 1>>
	{
		using precision = cmoon::measures::time<Rep, cmoon::measures::metric_system, Period, 1>;

		public:
			constexpr hh_mm_ss() = default;
			explicit constexpr hh_mm_ss(precision since_midnight)
				: base_hh_mm_ss<cmoon::measures::time<Rep, cmoon::measures::metric_system, Period, 1>>{since_midnight} {}
	};
}

namespace cmoon
{
	export
	template<class Duration, typename CharT>
	struct formatter<datetime::hh_mm_ss<Duration>, CharT> : public base_chrono_formatter<datetime::hh_mm_ss<Duration>, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				public:
					handler(const handler&) = delete;
					handler& operator=(const handler&) = delete;

					handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
						: tm_handler<CharT, OutputIt>{value, ctx} {}

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
					void on_day_of_year() { this->throw_not_supported(); }
					void on_iso_week(const details::numeric_system) { this->throw_not_supported(); }
					void on_dec0_weekday(const details::numeric_system) { this->throw_not_supported(); }
					void on_dec1_weekday(const details::numeric_system) { this->throw_not_supported(); }
					void on_day_of_month_space(const details::numeric_system) { this->throw_not_supported(); }
					void on_day_of_month_zero(const details::numeric_system) { this->throw_not_supported(); }
					void on_dec_month(const details::numeric_system) { this->throw_not_supported(); }
					void on_century(const details::numeric_system) { this->throw_not_supported(); }
					void on_abbr_year(const details::numeric_system) { this->throw_not_supported(); }
					void on_offset_year() { this->throw_not_supported(); }
					void on_year(const details::numeric_system) { this->throw_not_supported(); }
			};

		public:
			template<class OutputIt>
			auto format(const datetime::hh_mm_ss<Duration>& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				std::tm time;
				if constexpr (std::same_as<Duration, measures::hours>)
				{
					time.tm_hour = static_cast<int>(val.hours().count());
				}
				else if constexpr (std::same_as<Duration, measures::minutes>)
				{
					time.tm_hour = static_cast<int>(val.hours().count());
					time.tm_min = static_cast<int>(val.minutes().count());
				}
				else
				{
					time.tm_hour = static_cast<int>(val.hours().count());
					time.tm_min = static_cast<int>(val.minutes().count());
					time.tm_sec = static_cast<int>(val.seconds().count());
				}

				handler h{time, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}