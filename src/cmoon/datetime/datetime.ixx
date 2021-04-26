module;
#include "cmoon/string/literals.hpp"

export module cmoon.datetime;

import <chrono>;
import <concepts>;

import cmoon.format;
import cmoon.format.chrono;
import cmoon.format.write_string_view;
import cmoon.measures.time;

export import cmoon.datetime.sys_time;
export import cmoon.datetime.last_spec;
export import cmoon.datetime.hh_mm_ss;
export import cmoon.datetime.utility;
export import cmoon.datetime.year;
export import cmoon.datetime.day_of_month;
export import cmoon.datetime.day_of_year;
export import cmoon.datetime.month;
export import cmoon.datetime.weekday;
export import cmoon.datetime.weekday_indexed;
export import cmoon.datetime.weekday_last;
export import cmoon.datetime.month_day;
export import cmoon.datetime.month_day_last;
export import cmoon.datetime.month_weekday;
export import cmoon.datetime.month_weekday_last;
export import cmoon.datetime.year_month;
export import cmoon.datetime.year_month_day_last;
export import cmoon.datetime.year_month_day;
export import cmoon.datetime.year_month_weekday;
export import cmoon.datetime.year_month_weekday_last;
export import cmoon.datetime.sys_info;
export import cmoon.datetime.time_zone;
export import cmoon.datetime.time_zone_link;
export import cmoon.datetime.tzdb;
export import cmoon.datetime.tzdb_list;

namespace cmoon
{
	export
	template<typename CharT>
	struct formatter<std::tm, CharT> : public base_chrono_formatter<std::tm, CharT>
	{
		public:
			template<class OutputIt>
			constexpr auto format(const std::tm& t, basic_format_context<OutputIt, CharT>& ctx)
			{
				tm_handler h {t, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};

	export
	template<typename Duration, typename CharT>
	struct formatter<std::chrono::time_point<std::chrono::system_clock, Duration>, CharT> : public base_chrono_formatter<std::chrono::time_point<std::chrono::system_clock, Duration>, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				handler(const handler&) = delete;
				handler& operator=(const handler&) = delete;

				handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
					: tm_handler<CharT, OutputIt>{value, ctx} {}

				void on_tz_name()
				{
					this->on_text(cmoon::choose_str_literal<CharT>(STR_LITERALS("UTC")));
				}

				void on_utc_offset(const details::numeric_system s)
				{
					if (s == details::numeric_system::standard)
					{
						this->on_text(cmoon::choose_str_literal<CharT>(STR_LITERALS("+0000")));
					}
					else
					{
						this->on_text(cmoon::choose_str_literal<CharT>(STR_LITERALS("+00:00")));
					}
				}
			};
		public:
			template<class OutputIt>
			auto format(const std::chrono::time_point<std::chrono::system_clock, Duration>& val, basic_format_context<OutputIt, CharT>& ctx)
			{
				const auto time1 = std::chrono::system_clock::to_time_t(val);
				std::tm time2;
				::localtime_s(std::addressof(time2), std::addressof(time1));

				handler h{time2, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};

	export
	template<class Rep, class Period, typename CharT>
	struct formatter<std::chrono::duration<Rep, Period>, CharT> : public base_chrono_formatter<std::chrono::duration<Rep, Period>, CharT>
	{
		private:
			template<class OutputIt>
			struct handler : public tm_handler<CharT, OutputIt>
			{
				public:
					handler(const handler&) = delete;
					handler& operator=(const handler&) = delete;

					handler(const std::tm& value, const std::chrono::duration<Rep, Period>& dur, basic_format_context<OutputIt, CharT>& ctx)
						: tm_handler<CharT, OutputIt>{value, ctx}, dur{cmoon::measures::from_chrono(dur)} {}

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
					void on_duration_value()
					{
						cmoon::format_to(std::back_inserter(this->buf), cmoon::choose_str_literal<CharT>(STR_LITERALS("{}")), dur.count());
					}
					void on_duration_unit()
					{
						std::basic_stringstream<CharT> ss;
						cmoon::measures::details::output_unit_details<decltype(dur)>(ss);
						this->buf += ss.str();
					}
				private:
					cmoon::measures::time<Rep, cmoon::measures::metric_system, Period, 1> dur;
			};
		public:
			template<class OutputIt>
			auto format(const std::chrono::duration<Rep, Period>& d, basic_format_context<OutputIt, CharT>& ctx)
			{
				std::tm time;
				const datetime::hh_mm_ss<cmoon::measures::time<Rep, measures::metric_system, Period>> hhmmss {measures::from_chrono(d)};
				using duration_t = cmoon::measures::time<Rep, cmoon::measures::metric_system, Period, 1>;

				if constexpr (std::same_as<duration_t, cmoon::measures::basic_hours<Rep>>)
				{
					time.tm_hour = static_cast<int>(hhmmss.hours().count());
				}
				else if constexpr (std::same_as<duration_t, cmoon::measures::basic_minutes<Rep>>)
				{
					time.tm_hour = static_cast<int>(hhmmss.hours().count());
					time.tm_min = static_cast<int>(hhmmss.minutes().count());
				}
				else
				{
					time.tm_hour = static_cast<int>(hhmmss.hours().count());
					time.tm_min = static_cast<int>(hhmmss.minutes().count());
					time.tm_sec = static_cast<int>(hhmmss.seconds().count());
				}

				handler h{time, d, ctx};
				parse_chrono_format(this->fmt, h);

				return write_string_view(h.str(), ctx, this->parser);
			}
	};
}