module;

#include "cmoon/string/literals.hpp"

export module cmoon.format.chrono;

import <functional>;
import <string_view>;
import <iterator>;
import <locale>;

import cmoon.format.format_error;
import cmoon.format.basic_format_context;
import cmoon.format.base_formatter;
import cmoon.string;

namespace cmoon
{
	export
	enum class numeric_system
	{
		standard,
		alternative
	};

	export
	template<class T, class CharT, class OutputIt>
	struct base_chrono_handler
	{
		public:
			base_chrono_handler(const base_chrono_handler&) = delete;
			base_chrono_handler& operator=(const base_chrono_handler&) = delete;

			base_chrono_handler(const T& value, basic_format_context<OutputIt, CharT>& ctx)
				: value{value}, ctx{ctx} {}

			void on_text(std::basic_string_view<CharT> text)
			{
				buf.append(text);
			}

			void on_text(CharT ch)
			{
				buf.push_back(ch);
			}

			void on_abbr_weekday()
			{
				throw_not_supported();
			}

			void on_full_weekday()
			{
				throw_not_supported();
			}

			void on_dec0_weekday(const numeric_system)
			{
				throw_not_supported();
			}

			void on_dec1_weekday(const numeric_system)
			{
				throw_not_supported();
			}

			void on_abbr_month()
			{
				throw_not_supported();
			}

			void on_full_month()
			{
				throw_not_supported();
			}

			void on_24_hour(const numeric_system)
			{
				throw_not_supported();
			}

			void on_12_hour(const numeric_system)
			{
				throw_not_supported();
			}

			void on_minute(const numeric_system)
			{
				throw_not_supported();
			}

			void on_second(const numeric_system)
			{
				throw_not_supported();
			}

			void on_datetime(const numeric_system)
			{
				throw_not_supported();
			}

			void on_loc_date(const numeric_system)
			{
				throw_not_supported();
			}

			void on_loc_time(const numeric_system)
			{
				throw_not_supported();
			}

			void on_us_date()
			{
				throw_not_supported();
			}

			void on_iso_date()
			{
				throw_not_supported();
			}

			void on_12_hour_time()
			{
				throw_not_supported();
			}

			void on_24_hour_time()
			{
				throw_not_supported();
			}

			void on_iso_time()
			{
				throw_not_supported();
			}

			void on_am_pm()
			{
				throw_not_supported();
			}

			void on_duration_value()
			{
				throw_not_supported();
			}

			void on_duration_unit()
			{
				throw_not_supported();
			}

			void on_utc_offset(const numeric_system)
			{
				throw_not_supported();
			}

			void on_tz_name()
			{
				throw_not_supported();
			}

			void on_century(const numeric_system)
			{
				throw_not_supported();
			}

			void on_abbr_year(const numeric_system)
			{
				throw_not_supported();
			}

			void on_offset_year()
			{
				throw_not_supported();
			}

			void on_year(const numeric_system)
			{
				throw_not_supported();
			}

			void on_dec_month(const numeric_system)
			{
				throw_not_supported();
			}

			void on_day_of_month_zero(const numeric_system)
			{
				throw_not_supported();
			}

			void on_day_of_month_space(const numeric_system)
			{
				throw_not_supported();
			}

			void on_iso_abbr_year()
			{
				throw_not_supported();
			}

			void on_iso_year()
			{
				throw_not_supported();
			}

			void on_iso_week(const numeric_system)
			{
				throw_not_supported();
			}

			void on_day_of_year()
			{
				throw_not_supported();
			}

			void on_week_sunday(const numeric_system)
			{
				throw_not_supported();
			}

			void on_week_monday(const numeric_system)
			{
				throw_not_supported();
			}

			[[nodiscard]] std::basic_string_view<CharT> str() const noexcept
			{
				return std::basic_string_view<CharT>{buf};
			}
		protected:
			std::reference_wrapper<const T> value;
			basic_format_context<OutputIt, CharT>& ctx;
			std::basic_string<CharT> buf;

			void throw_not_supported() const
			{
				throw format_error{"operation not supported"};
			}
	};

	export
	template<class CharT, class OutputIt>
	struct tm_handler : public base_chrono_handler<std::tm, CharT, OutputIt>
	{
		public:
			tm_handler(const tm_handler&) = delete;
			tm_handler& operator=(const tm_handler&) = delete;

			tm_handler(const std::tm& value, basic_format_context<OutputIt, CharT>& ctx)
				: base_chrono_handler<std::tm, CharT, OutputIt>{value, ctx} {}

			void on_abbr_month()
			{
				write_localized('b');
			}

			void on_full_month()
			{
				write_localized('B');
			}

			void on_abbr_weekday()
			{
				write_localized('a');
			}

			void on_full_weekday()
			{
				write_localized('A');
			}

			void on_iso_abbr_year()
			{
				on_abbr_year(numeric_system::standard);
			}

			void on_iso_year()
			{
				on_year(numeric_system::standard);
			}

			void on_week_sunday(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('U');
				}
				else
				{
					write_localized('U', 'O');
				}
			}

			void on_week_monday(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('W');
				}
				else
				{
					write_localized('W', 'O');
				}
			}

			void on_us_date()
			{
				on_dec_month(numeric_system::standard);
				this->on_text(CharT('/'));
				on_day_of_month_zero(numeric_system::standard);
				this->on_text(CharT('/'));
				on_abbr_year(numeric_system::standard);
			}

			void on_iso_date()
			{
				on_year(numeric_system::standard);
				this->on_text(CharT('-'));
				on_dec_month(numeric_system::standard);
				this->on_text(CharT('-'));
				on_day_of_month_zero(numeric_system::standard);
			}
				
			void on_loc_date(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('x');
				}
				else
				{
					write_localized('x', 'E');
				}
			}

			void on_datetime(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('c');
				}
				else
				{
					write_localized('c', 'E');
				}
			}

			void on_utc_offset(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('z');
				}
				else
				{
					write_localized('z', 'E');
				}
			}

			void on_tz_name()
			{
				write_localized('Z');
			}

			void on_day_of_year()
			{
				const auto yday = get_day_of_year();
				format_to(std::back_inserter(this->buf), cmoon::choose_str_literal<CharT>(STR_LITERALS("{:0>3}")), yday);
			}

			void on_iso_week(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('V');
				}
				else
				{
					write_localized('V', 'O');
				}
			}

			void on_dec0_weekday(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					this->on_text(static_cast<CharT>(CharT('0') + get_day_of_week()));
				}
				else
				{
					write_localized('w', 'O');
				}
			}

			void on_dec1_weekday(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					auto weekday = get_day_of_week();
					if (weekday == 0)
					{
						weekday = 7;
					}

					this->on_text(static_cast<CharT>(CharT('0') + weekday));
				}
				else
				{
					write_localized('u', 'O');
				}
			}

			void on_day_of_month_space(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto mday = get_day_of_month();
					const auto ones = mday % 10;
					const auto tens = (mday / 10) % 10;

					this->on_text(tens == 0 ? CharT(' ') : static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('e', 'O');
				}
			}

			void on_day_of_month_zero(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto mday = get_day_of_month();
					const auto ones = mday % 10;
					const auto tens = (mday / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('d', 'O');
				}
			}

			void on_dec_month(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto month = get_month();
					const auto ones = month % 10;
					const auto tens = (month / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('m', 'O');
				}
			}

			void on_century(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					format_to(std::back_inserter(this->buf), cmoon::choose_str_literal<CharT>(STR_LITERALS("{:0<2}")), get_year() / 100);
				}
				else
				{
					write_localized('C', 'E');
				}
			}

			void on_abbr_year(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto year = get_year();
					const auto ones = year % 10;
					const auto tens = (year / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('y');
				}
			}

			void on_offset_year()
			{
				write_localized('y', 'E');
			}

			void on_year(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					format_to(std::back_inserter(this->buf), cmoon::choose_str_literal<CharT>(STR_LITERALS("{:0<4}")), get_year());
				}
				else
				{
					write_localized('Y');
				}
			}

			void on_24_hour(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto hour = get_hour();
					const auto ones = hour % 10;
					const auto tens = (hour / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('H', 'O');
				}
			}

			void on_12_hour(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto hour = get_hour() % 12;
					const auto ones = hour % 10;
					const auto tens = (hour / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('I', 'O');
				}
			}

			void on_minute(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto minute = get_minute();
					const auto ones = minute % 10;
					const auto tens = (minute / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('M', 'O');
				}
			}

			void on_second(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					const auto second = get_second();
					const auto ones = second % 10;
					const auto tens = (second / 10) % 10;

					this->on_text(static_cast<CharT>(CharT('0') + tens));
					this->on_text(static_cast<CharT>(CharT('0') + ones));
				}
				else
				{
					write_localized('S', 'O');
				}
			}

			void on_am_pm()
			{
				write_localized('p');
			}

			void on_24_hour_time()
			{
				on_24_hour(numeric_system::standard);
				this->on_text(CharT(':'));
				on_minute(numeric_system::standard);
			}

			void on_12_hour_time()
			{
				write_localized('r');
			}

			void on_iso_time()
			{
				on_24_hour(numeric_system::standard);
				this->on_text(CharT(':'));
				on_minute(numeric_system::standard);
				this->on_text(CharT(':'));
				on_second(numeric_system::standard);
			}

			void on_loc_time(const numeric_system s)
			{
				if (s == numeric_system::standard)
				{
					write_localized('X');
				}
				else
				{
					write_localized('X', 'E');
				}
			}
		private:
			void write_localized(char format, char modifier = 0)
			{
				const auto& facet = std::use_facet<std::time_put<CharT>>(this->ctx.locale());
				std::basic_ostringstream<CharT> os;
				os.imbue(this->ctx.locale());
				const auto& t = get_time();
				facet.put(os, os, ' ', std::addressof(t), format, modifier);
				this->on_text(os.str());
			}

			[[nodiscard]] auto get_time() const noexcept
			{
				return this->value.get();
			}

			[[nodiscard]] auto get_year() const noexcept
			{
				return get_time().tm_year + 1900;
			}

			[[nodiscard]] auto get_month() const noexcept
			{
				return get_time().tm_mon + 1;
			}

			[[nodiscard]] auto get_day_of_month() const noexcept
			{
				return get_time().tm_mday;
			}

			[[nodiscard]] auto get_day_of_week() const noexcept
			{
				return get_time().tm_wday;
			}

			[[nodiscard]] auto get_day_of_year() const noexcept
			{
				return get_time().tm_yday + 1;
			}

			[[nodiscard]] auto get_hour() const noexcept
			{
				return get_time().tm_hour;
			}

			[[nodiscard]] auto get_minute() const noexcept
			{
				return get_time().tm_min;
			}

			[[nodiscard]] auto get_second() const noexcept
			{
				return get_time().tm_sec;
			}
	};

	export
	template<class CharT, class Handler>
	constexpr auto parse_chrono_format(std::basic_string_view<CharT> fmt, Handler& handler)
	{
		auto begin = std::begin(fmt);
		const auto end = std::end(fmt);
		auto it = begin;
					
		while (it != end)
		{
			auto ch = *it;
			if (ch != CharT('%'))
			{
				++it;
				continue;
			}

			if (it != begin)
			{
				handler.on_text(std::basic_string_view<CharT>{begin, it});
			}

			++it;
			if (it == end)
			{
				throw format_error{"Invalid format string"};
			}

			ch = *it++;
			switch (ch)
			{
				case CharT('%'):
					handler.on_text(std::basic_string_view<CharT>{it - 1, it});
					break;
				case CharT('n'):
					handler.on_text(CharT('\n'));
					break;
				case CharT('t'):
					handler.on_text(CharT('\t'));
					break;
				case CharT('C'):
					handler.on_century(numeric_system::standard);
					break;
				case CharT('a'):
					handler.on_abbr_weekday();
					break;
				case CharT('A'):
					handler.on_full_weekday();
					break;
				case CharT('w'):
					handler.on_dec0_weekday(numeric_system::standard);
					break;
				case CharT('u'):
					handler.on_dec1_weekday(numeric_system::standard);
					break;
				case CharT('b'):
				case CharT('h'):
					handler.on_abbr_month();
					break;
				case CharT('B'):
					handler.on_full_month();
					break;
				case CharT('H'):
					handler.on_24_hour(numeric_system::standard);
					break;
				case CharT('I'):
					handler.on_12_hour(numeric_system::standard);
					break;
				case CharT('M'):
					handler.on_minute(numeric_system::standard);
					break;
				case CharT('S'):
					handler.on_second(numeric_system::standard);
					break;
				case CharT('c'):
					handler.on_datetime(numeric_system::standard);
					break;
				case CharT('x'):
					handler.on_loc_date(numeric_system::standard);
					break;
				case CharT('X'):
					handler.on_loc_time(numeric_system::standard);
					break;
				case CharT('D'):
					handler.on_us_date();
					break;
				case CharT('F'):
					handler.on_iso_date();
					break;
				case CharT('r'):
					handler.on_12_hour_time();
					break;
				case CharT('R'):
					handler.on_24_hour_time();
					break;
				case CharT('T'):
					handler.on_iso_time();
					break;
				case CharT('p'):
					handler.on_am_pm();
					break;
				case CharT('Q'):
					handler.on_duration_value();
					break;
				case CharT('q'):
					handler.on_duration_unit();
					break;
				case CharT('z'):
					handler.on_utc_offset(numeric_system::standard);
					break;
				case CharT('Z'):
					handler.on_tz_name();
					break;
				case CharT('y'):
					handler.on_abbr_year(numeric_system::standard);
					break;
				case CharT('Y'):
					handler.on_year(numeric_system::standard);
					break;
				case CharT('m'):
					handler.on_dec_month(numeric_system::standard);
					break;
				case CharT('d'):
					handler.on_day_of_month_zero(numeric_system::standard);
					break;
				case CharT('e'):
					handler.on_day_of_month_space(numeric_system::standard);
					break;
				case CharT('g'):
					handler.on_iso_abbr_year();
					break;
				case CharT('G'):
					handler.on_iso_year();
					break;
				case CharT('V'):
					handler.on_iso_week(numeric_system::standard);
					break;
				case CharT('j'):
					handler.on_day_of_year();
					break;
				case CharT('U'):
					handler.on_week_sunday(numeric_system::standard);
					break;
				case CharT('W'):
					handler.on_week_monday(numeric_system::standard);
					break;
				case CharT('E'):
					if (it == end)
					{
						throw format_error{"Invalid format string"};
					}
					ch = *it++;
					switch (ch)
					{
						case CharT('C'):
							handler.on_century(numeric_system::alternative);
							break;
						case CharT('c'):
							handler.on_datetime(numeric_system::alternative);
							break;
						case CharT('x'):
							handler.on_loc_date(numeric_system::alternative);
							break;
						case CharT('X'):
							handler.on_loc_time(numeric_system::alternative);
							break;
						case CharT('y'):
							handler.on_offset_year();
							break;
						case CharT('Y'):
							handler.on_year(numeric_system::alternative);
							break;
						case CharT('z'):
							handler.on_utc_offset(numeric_system::alternative);
							break;
						default:
							throw format_error{"Invalid format string"};
					}
					break;
				case CharT('O'):
					if (it == end)
					{
						throw format_error{"Invalid format string"};
					}
					ch = *it++;
					switch (ch)
					{
						case CharT('w'):
							handler.on_dec0_weekday(numeric_system::alternative);
							break;
						case CharT('u'):
							handler.on_dec1_weekday(numeric_system::alternative);
							break;
						case CharT('H'):
							handler.on_24_hour(numeric_system::alternative);
							break;
						case CharT('I'):
							handler.on_12_hour(numeric_system::alternative);
							break;
						case CharT('M'):
							handler.on_minute(numeric_system::alternative);
							break;
						case CharT('S'):
							handler.on_second(numeric_system::alternative);
							break;
						case CharT('y'):
							handler.on_abbr_year(numeric_system::alternative);
							break;
						case CharT('m'):
							handler.on_dec_month(numeric_system::alternative);
							break;
						case CharT('d'):
							handler.on_day_of_month_zero(numeric_system::alternative);
							break;
						case CharT('e'):
							handler.on_day_of_month_space(numeric_system::alternative);
							break;
						case CharT('V'):
							handler.on_iso_week(numeric_system::alternative);
							break;
						case CharT('U'):
							handler.on_week_sunday(numeric_system::alternative);
							break;
						case CharT('W'):
							handler.on_week_monday(numeric_system::alternative);
							break;
						case CharT('z'):
							handler.on_utc_offset(numeric_system::alternative);
							break;
						default:
							throw format_error{"Invalid format string"};
					}
					break;
				default:
					throw format_error{"Invalid format string"};
			}

			begin = it;
		}

		if (it != begin)
		{
			handler.on_text(std::basic_string_view<CharT>{begin, it});
		}
	}

	export
	template<class T, class CharT>
	struct base_chrono_formatter : public base_formatter<T, CharT>
	{
		public:
			constexpr auto parse(basic_format_parse_context<CharT>& ctx)
			{
				const auto start = base_formatter<T, CharT>::parse(ctx);
				auto end = start;
				while (end != ctx.end() && *end != CharT('}'))
				{
					++end;
				}

				fmt = {start, end};
				return end;
			}

			template<class OutputIt>
			constexpr auto format(const T& t, basic_format_context<OutputIt, CharT>& ctx)
			{
				base_chrono_handler handler {t, ctx};
				parse_chrono_format(fmt, handler);

				return write_string_view(handler.str(), ctx, this->parser);
			}
		protected:
			std::basic_string_view<CharT> fmt;
	};
}