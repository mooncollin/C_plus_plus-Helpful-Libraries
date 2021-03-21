module;
#include <string>
#include <variant>
#include <string_view>
#include <stdexcept>
#include <type_traits>
#include <concepts>
#include <array>
#include <span>
#include <iterator>
#include <optional>
#include <cctype>
#include <locale>
#include <charconv>
#include <utility>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <ranges>
#include <iostream>
#include <time.h>

#include "cmoon/string_literals.hpp"
#include "cmoon/format/forward_declarations.hpp"

export module cmoon.format;
import cmoon.string;
import cmoon.platform;
import cmoon.utility;
export import cmoon.format.format_error;
export import cmoon.format.basic_format_context;
export import cmoon.format.basic_format_arg;
export import cmoon.format.visit_format_arg;
export import cmoon.format.basic_format_args;
export import cmoon.format.basic_format_parse_context;
import cmoon.format.is_formatter;
import cmoon.format.format_specs;

namespace cmoon
{
	export
	using format_context = basic_format_context<std::back_insert_iterator<std::string>, char>;

	export
	using wformat_context = basic_format_context<std::back_insert_iterator<std::wstring>, wchar_t>;

	template<typename CharT>
	[[nodiscard]] constexpr bool at_end(const basic_format_parse_context<CharT>& parse_ctx)
	{
		return parse_ctx.begin() == parse_ctx.end();
	}

	template<typename CharT>
	[[nodiscard]] constexpr CharT get_char(const basic_format_parse_context<CharT>& parse_ctx)
	{
		return *parse_ctx.begin();
	}

	template<class OutputIt, typename CharT>
	[[nodiscard]] constexpr void set_char(basic_format_context<OutputIt, CharT>& format_ctx, CharT c)
	{
		*format_ctx.out() = c;
	}

	template<typename CharT>
	[[nodiscard]] constexpr void advance_one(basic_format_parse_context<CharT>& parse_ctx)
	{
		parse_ctx.advance_to(parse_ctx.begin() + 1);
	}

	template<class OutputIt, typename CharT>
	[[nodiscard]] constexpr void advance_one(basic_format_context<OutputIt, CharT>& format_ctx)
	{
		format_ctx.advance_to(++format_ctx.out());
	}

	template<typename CharT>
	[[nodiscard]] constexpr int parse_nonnegative_int(basic_format_parse_context<CharT>& parse_ctx)
	{
		unsigned value = 0;
		constexpr unsigned max_int = std::numeric_limits<int>::max();
		constexpr unsigned big = max_int / 10;

		do
		{
			if (value > big)
			{
				value = max_int + 1;
				break;
			}

			value = value * 10 + static_cast<unsigned>(get_char(parse_ctx) - '0');
			advance_one(parse_ctx);
		} while (!at_end(parse_ctx) && cmoon::is_digit(get_char(parse_ctx)));

		if (value > max_int)
		{
			throw format_error{"number is too big"};
		}

		return static_cast<int>(value);
	}

	template<typename CharT>
	constexpr std::size_t parse_arg_id(basic_format_parse_context<CharT>& parse_ctx)
	{
		constexpr auto open_brace = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('}'));
		constexpr auto colon = cmoon::choose_char_literal<CharT>(CHAR_LITERALS(':'));
		constexpr auto zero_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('0'));

		auto c = get_char(parse_ctx);
		if (c == open_brace || c == colon)
		{
			return parse_ctx.next_arg_id();
		}

		if (cmoon::is_digit(c))
		{
			auto index = 0;
			if (c != zero_char)
			{
				index = parse_nonnegative_int(parse_ctx);
			}
			else
			{
				advance_one(parse_ctx);
			}

			if (c = get_char(parse_ctx); at_end(parse_ctx) || (c != open_brace && c != colon))
			{
				throw format_error{"invalid format string"};
			}
			else
			{
				parse_ctx.check_arg_id(index);
			}

			return index;
		}
		else
		{
			throw format_error{"invalid format string"};
		}
	}

	template<typename T, typename CharT>
	class format_parser
	{
		using context_t = basic_format_parse_context<CharT>;

		public:
			constexpr format_parser() noexcept = default;
			constexpr format_parser(context_t& parse_ctx)
			{
				parse(parse_ctx);
			}

			constexpr void parse(context_t& parse_ctx)
			{
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_align(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_sign(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_hash(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_zero_flag(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_width(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_precision(parse_ctx);
				if (at_end(parse_ctx))
				{
					return;
				}

				parse_type(parse_ctx);
			}

			[[nodiscard]] constexpr const basic_format_specs<CharT>& specifications() const noexcept
			{
				return specs;
			}

			[[nodiscard]] constexpr const dynamic_format_specs<CharT>& dynamic_specifications() const noexcept
			{
				return dynamic_specs;
			}

			[[nodiscard]] constexpr bool is_width_dynamic() const noexcept
			{
				return dynamic_specs.width.has_value();
			}

			[[nodiscard]] constexpr bool is_precision_dynamic() const noexcept
			{
				return dynamic_specs.precision.has_value();
			}

			template<class OutputIt>
			[[nodiscard]] constexpr int get_width(const basic_format_context<OutputIt, CharT>& ctx) const noexcept
			{
				struct f
				{
					int operator()(int v) const noexcept
					{
						return v;
					}

					int operator()(const auto&) const noexcept
					{
						return 0;
					}
				};

				if (is_width_dynamic())
				{
					std::visit(f{}, ctx.arg(dynamic_specs.width.value()).value);
					//return visit_format_arg(cmoon::overloaded {
					//	[](int v) { return v; },
					//	[](const auto&) { return 0; },
					//}, ctx.arg(dynamic_specs.width.value()));
				}

				return specs.width;
			}

			template<class OutputIt>
			[[nodiscard]] constexpr int get_precision(const basic_format_context<OutputIt, CharT>& ctx) const noexcept
			{
				if (is_precision_dynamic())
				{
					return visit_format_arg(cmoon::overloaded {
						[](int v) { return v; },
						[](const auto&) { return -1; },
					}, ctx.arg(dynamic_specs.precision.value()));
				}

				return specs.precision;
			}
		private:
			basic_format_specs<CharT> specs;
			dynamic_format_specs<CharT> dynamic_specs;

			static constexpr auto open_brace_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('{'));
			static constexpr auto close_brace_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('}'));
			static constexpr auto left_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('<'));
			static constexpr auto right_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('>'));
			static constexpr auto numeric_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('='));
			static constexpr auto center_align_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('^'));
			static constexpr auto plus_sign_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('+'));
			static constexpr auto minus_sign_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('-'));
			static constexpr auto space_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS(' '));
			static constexpr auto hash_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('#'));
			static constexpr auto zero_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('0'));
			static constexpr auto precision_char = cmoon::choose_char_literal<CharT>(CHAR_LITERALS('.'));

			constexpr void parse_align(context_t& parse_ctx)
			{
				auto align = align_t::none;

				auto p = parse_ctx.begin() + cmoon::code_point_length(get_char(parse_ctx));
				if (p >= parse_ctx.end())
				{
					p = parse_ctx.begin();
				}

				while (true)
				{
					switch (*p)
					{
						case left_align_char:
							align = align_t::left;
							break;
						case right_align_char:
							align = align_t::right;
							break;
						case numeric_align_char:
							align = align_t::numeric;
							break;
						case center_align_char:
							align = align_t::center;
							break;
					}

					if (align != align_t::none)
					{
						if (p != parse_ctx.begin())
						{
							if (get_char(parse_ctx) == open_brace_char)
							{
								throw format_error{"invalid fill character '{'"};
							}

							specs.fill = get_char(parse_ctx);
							parse_ctx.advance_to(p + 1);
						}
						else
						{
							advance_one(parse_ctx);
						}

						specs.align = align;
						break;
					}
					else if (p == parse_ctx.begin())
					{
						break;
					}

					p = parse_ctx.begin();
				}
			}

			constexpr void parse_sign(context_t& parse_ctx) noexcept
			{
				switch (get_char(parse_ctx))
				{
					case plus_sign_char:
						specs.sign = sign_t::plus;
						advance_one(parse_ctx);
						break;
					case minus_sign_char:
						specs.sign = sign_t::minus;
						advance_one(parse_ctx);
						break;
					case space_char:
						specs.sign = sign_t::space;
						advance_one(parse_ctx);
						break;
				}
			}

			constexpr void parse_hash(context_t& parse_ctx) noexcept
			{
				if (get_char(parse_ctx) == hash_char)
				{
					specs.alt = true;
					advance_one(parse_ctx);
				}
			}

			constexpr void parse_zero_flag(context_t& parse_ctx) noexcept
			{
				if (get_char(parse_ctx) == zero_char)
				{
					specs.align = align_t::numeric;
					specs.fill = zero_char;
					advance_one(parse_ctx);
				}
			}

			constexpr void parse_width(context_t& parse_ctx)
			{
				if (cmoon::is_digit(get_char(parse_ctx)))
				{
					specs.width = parse_nonnegative_int(parse_ctx);
				}
				else if (get_char(parse_ctx) == open_brace_char)
				{
					advance_one(parse_ctx);
					if (!at_end(parse_ctx))
					{
						dynamic_specs.width = parse_arg_id(parse_ctx);
					}

					if (at_end(parse_ctx) || get_char(parse_ctx) != close_brace_char)
					{
						throw format_error{"invalid format string"};
					}

					advance_one(parse_ctx);
				}
			}

			constexpr void parse_precision(context_t& parse_ctx)
			{
				if (get_char(parse_ctx) == precision_char)
				{
					advance_one(parse_ctx);
					auto c = !at_end(parse_ctx) ? get_char(parse_ctx) : CharT{};
					if (cmoon::is_digit(c))
					{
						specs.precision = parse_nonnegative_int(parse_ctx);
					}
					else if (c == open_brace_char)
					{
						advance_one(parse_ctx);
						if (!at_end(parse_ctx))
						{
							dynamic_specs.precision = parse_arg_id(parse_ctx);
						}

						if (at_end(parse_ctx) || get_char(parse_ctx) != close_brace_char)
						{
							throw format_error{"invalid format string"};
						}
						else
						{
							throw format_error{"missing precision specifier"};
						}
					}
				}
			}

			constexpr void parse_type(context_t& parse_ctx)
			{
				// Can compile out type parsing step if the type that is being
				// formatted is not expecting a type.
				if constexpr (std::integral<T> || 
								std::floating_point<T> || 
								std::same_as<std::decay_t<T>, std::basic_string_view<CharT>> ||
								std::same_as<std::decay_t<T>, void*>)
				{
					if (!at_end(parse_ctx) && get_char(parse_ctx) != close_brace_char)
					{
						const auto ch = get_char(parse_ctx);
						if constexpr (std::same_as<T, CharT> || std::same_as<T, char>)
						{
							switch (ch)
							{
								case CharT('b'):
								case CharT('B'):
								case CharT('d'):
								case CharT('o'):
								case CharT('x'):
								case CharT('X'):
								case CharT('L'):
								case CharT('c'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::same_as<std::decay_t<T>, std::basic_string_view<CharT>>)
						{
							switch (ch)
							{
								case CharT('s'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::integral<T> /* and boolean */)
						{
							switch (ch)
							{
								case CharT('b'):
								case CharT('B'):
								case CharT('d'):
								case CharT('o'):
								case CharT('x'):
								case CharT('X'):
								case CharT('L'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::floating_point<T>)
						{
							switch (ch)
							{
								case CharT('a'):
								case CharT('A'):
								case CharT('e'):
								case CharT('E'):
								case CharT('f'):
								case CharT('F'):
								case CharT('g'):
								case CharT('G'):
								case CharT('L'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
						else if constexpr (std::same_as<std::decay_t<T>, void*>)
						{
							switch (ch)
							{
								case CharT('p'):
									specs.type = ch;
									advance_one(parse_ctx);
									break;
							}
						}
					}
				}
			}
	};

	enum class numeric_system
	{
		standard,
		alternative
	};

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

	template<class T, class CharT>
	struct base_formatter
	{
		protected:
			format_parser<T, CharT> parser;
		public:
			constexpr typename basic_format_parse_context<CharT>::iterator parse(basic_format_parse_context<CharT>& ctx)
			{
				parser = {};
				parser.parse(ctx);
				return ctx.begin();
			}
	};

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

	template<class T, class OutputIt, class CharT, class Traits = std::char_traits<CharT>>
	typename basic_format_context<OutputIt, CharT>::iterator write_string_view(std::basic_string_view<CharT, Traits> str, basic_format_context<OutputIt, CharT>& ctx, const format_parser<T, CharT>& spec_info)
	{
		const auto width = spec_info.get_width(ctx);
		const auto alignment = spec_info.specifications().align == align_t::none ? align_t::left : spec_info.specifications().align;
		const auto fill = spec_info.specifications().fill;

		const auto fill_amount = width == 0 ? 0 : width - std::size(str);

		if (alignment == align_t::left)
		{
			ctx.advance_to(std::copy(std::begin(str), std::end(str), ctx.out()));
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount, fill));
		}
		else if (alignment == align_t::center)
		{
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount / 2, fill));
			ctx.advance_to(std::copy(std::begin(str), std::end(str), ctx.out()));
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount - fill_amount / 2, fill));
		}
		else if (alignment == align_t::right)
		{
			ctx.advance_to(std::fill_n(ctx.out(), fill_amount, fill));
			ctx.advance_to(std::copy(std::begin(str), std::end(str), ctx.out()));
		}

		return ctx.out();
	}

	template<std::integral T, class T2, class OutputIt, class CharT>
	typename basic_format_context<OutputIt, CharT>::iterator write_integer(T value, basic_format_context<OutputIt, CharT>& ctx, const format_parser<T2, CharT>& spec_info)
	{
		const auto sign = spec_info.specifications().sign == sign_t::none ? sign_t::minus : spec_info.specifications().sign;
		const auto alternative = spec_info.specifications().alt;
		const auto type = spec_info.specifications().type;

		constexpr auto is_regular_char = std::same_as<CharT, char>;

		// Overestimating static size in case user needs base 2 conversion,
		// which is the most demanding (plus one for possible sign, and plus two for possible representation).
		constexpr std::size_t max_size = std::numeric_limits<T>::digits + 1 + 2;

		std::array<CharT, max_size> buf {};
		std::basic_string<CharT> dyn_buf;

		std::basic_string_view<CharT> str;

		CharT* start {buf.data()};

		const auto extra_size = [&start, &buf] {
			return static_cast<std::size_t>(start - buf.data());
		};

		const auto do_sign = [sign, value] (auto it) {
			switch (sign)
			{
				case sign_t::plus:
					if constexpr (std::signed_integral<T>)
					{
						if (value > 0)
						{
							*it++ = CharT('+');
						}
					}
					else
					{
						*it++ = CharT('+');
					}
					break;
				case sign_t::space:
					if constexpr (std::signed_integral<T>)
					{
						if (value > 0)
						{
							*it++ = CharT(' ');
						}
					}
					else
					{
						*it++ = CharT(' ');
					}
					break;
			}
			return it;
		};

		switch (type)
		{
			case CharT('b'):
			case CharT('B'):
				if (alternative)
				{
					*start++ = CharT('0');
					*start++ = CharT(type);
				}
				start = do_sign(start);

				if constexpr (is_regular_char)
				{
					const auto r = std::to_chars(start, buf.data() + buf.size(), value, 2);
					str = {buf.data(), r.ptr};
				}
				else
				{
					if constexpr (std::signed_integral<T>)
					{
						if (value < 0)
						{
							*start++ = CharT('-');
							value = -value;
						}

						const auto r = cmoon::to_binary_string(start, buf.data() + buf.size(), value);
						str = {buf.data(), r};
					}
					else
					{
						const auto r = cmoon::to_binary_string(start, buf.data() + buf.size(), value);
						str = {buf.data(), r};
					}
				}
				break;
			case CharT('o'):
				if (alternative)
				{
					*start++ = CharT('0');
				}
				start = do_sign(start);

				if constexpr (is_regular_char)
				{
					const auto r = std::to_chars(start, buf.data() + buf.size(), value, 8);
					str = {buf.data(), r.ptr};
				}
				else
				{
					if constexpr (std::signed_integral<T>)
					{
						if (value < 0)
						{
							*start++ = CharT('-');
							value = -value;
						}
					}

					const auto r = std::swprintf(start, buf.size() - extra_size(), cmoon::printf_format_specifier_value<std::make_unsigned_t<T>, cmoon::printf_conversion_specifier::octal, wchar_t>().data(), value);
					str = {buf.data(), static_cast<std::size_t>(r) + extra_size()};
				}
				break;
			case CharT('x'):
			case CharT('X'):
				if (alternative)
				{
					*start++ = CharT('0');
					*start++ = CharT(type);
				}
				start = do_sign(start);

				if constexpr (is_regular_char)
				{
					const auto r = std::to_chars(start, buf.data() + buf.size(), value, 16);
					str = {buf.data(), r.ptr};
					if (type == CharT('X'))
					{
						std::transform(std::begin(buf), std::begin(buf) + str.size(), std::begin(buf), [](unsigned char c) { return static_cast<CharT>(std::toupper(c)); });
					}
				}
				else
				{
					if constexpr (std::signed_integral<T>)
					{
						if (value < 0)
						{
							*start++ = CharT('-');
							value = -value;
						}
					}

					const auto format = type == CharT('x') ? cmoon::printf_format_specifier_value<std::make_unsigned_t<T>, cmoon::printf_conversion_specifier::hex_small, wchar_t>().data() : cmoon::printf_format_specifier_value<std::make_unsigned_t<T>, cmoon::printf_conversion_specifier::hex_big, wchar_t>().data();
					const auto r = std::swprintf(start, buf.size() - extra_size(), format, value);
					str = {buf.data(), static_cast<std::size_t>(r) + extra_size()};
				}
				break;
			case CharT('L'):
				{
					std::basic_stringstream<CharT> ss;
					ss.imbue(ctx.locale());
					ss << value;
					dyn_buf = ss.str();
					do_sign(std::inserter(dyn_buf, dyn_buf.begin()));
					str = {dyn_buf.data(), dyn_buf.size()};
					break;
				}
			default:
				start = do_sign(start);
				if constexpr (is_regular_char)
				{
					const auto r = std::to_chars(start, buf.data() + buf.size(), value);
					str = {buf.data(), r.ptr};
				}
				else
				{
					const auto r = std::swprintf(start, buf.size(), cmoon::printf_format_specifier_value<T, wchar_t>().data(), value);
					str = {buf.data(), static_cast<std::size_t>(r) + extra_size()};
				}
				break;
		}

		return write_string_view(str, ctx, spec_info);
	}

	template<class OutputIt, class CharT>
	typename basic_format_context<OutputIt, CharT>::iterator write_boolean(bool value, basic_format_context<OutputIt, CharT>& ctx, const format_parser<bool, CharT>& spec_info)
	{
		const auto type = spec_info.specifications().type;
			
		switch (type)
		{
			case CharT('b'):
			case CharT('B'):
			case CharT('d'):
			case CharT('o'):
			case CharT('x'):
			case CharT('X'):
				return write_integer(static_cast<int>(value), ctx, spec_info);
		}

		std::basic_string<CharT> dyn_boolean_string;
		std::basic_string_view<CharT> boolean_string;

		if (type == CharT('L'))
		{
			if (value)
			{
				dyn_boolean_string = std::use_facet<std::numpunct<CharT>>(ctx.locale()).truename();
			}
			else
			{
				dyn_boolean_string = std::use_facet<std::numpunct<CharT>>(ctx.locale()).falsename();
			}

			boolean_string = dyn_boolean_string;
		}
		else
		{
			if (value)
			{
				boolean_string = cmoon::choose_str_literal<CharT>(STR_LITERALS("true"));
			}
			else
			{
				boolean_string = cmoon::choose_str_literal<CharT>(STR_LITERALS("false"));
			}
		}

		return write_string_view(boolean_string, ctx, spec_info);
	}

	template<class OutputIt, class CharT>
	typename basic_format_context<OutputIt, CharT>::iterator write_pointer(const void* value, basic_format_context<OutputIt, CharT>& ctx, const format_parser<const void*, CharT>& spec_info)
	{
		constexpr auto is_regular_char = std::same_as<CharT, char>;
		constexpr std::size_t max_size = std::numeric_limits<std::uintptr_t>::digits10 + 3;
		std::array<CharT, max_size> buf {};

		std::basic_string_view<CharT> str;
		CharT* start {buf.data()};

		const auto extra_size = [&start, &buf] {
			return static_cast<std::size_t>(start - buf.data());
		};

		*start++ = CharT('0');
		*start++ = CharT('x');

		if constexpr (is_regular_char)
		{
			const auto r = std::to_chars(start, buf.data() + buf.size(), reinterpret_cast<std::uintptr_t>(value), 16);
			str = {buf.data(), r.ptr};
		}
		else
		{
			const auto r = std::swprintf(start, buf.size() - extra_size(), cmoon::printf_format_specifier_value<std::uintptr_t, cmoon::printf_conversion_specifier::hex_small, wchar_t>().data(), reinterpret_cast<std::uintptr_t>(value));
			str = {buf.data(), static_cast<std::size_t>(r) + extra_size()};
		}

		return write_string_view(str, ctx, spec_info);
	}

	template<class T, class OutputIt, class CharT>
		requires(std::floating_point<T>)
	typename basic_format_context<OutputIt, CharT>::iterator write_floating(T value, basic_format_context<OutputIt, CharT>& ctx, const format_parser<T, CharT>& spec_info)
	{
		const auto sign = spec_info.specifications().sign == sign_t::none ? sign_t::minus : spec_info.specifications().sign;
		const auto precision = spec_info.get_precision(ctx);
		const auto alternative = spec_info.specifications().alt;
		const auto type = spec_info.specifications().type;

		constexpr auto is_regular_char = std::same_as<CharT, char>;

		constexpr std::size_t max_size = std::numeric_limits<T>::digits;

		std::array<CharT, max_size> buf {};
		std::basic_string<CharT> dyn_buf;

		std::basic_string_view<CharT> str;
		CharT* start {buf.data()};

		const auto extra_size = [&start, &buf] {
			return static_cast<std::size_t>(start - buf.data());
		};

		const auto do_sign = [sign, value] (auto it) {
			switch (sign)
			{
				case sign_t::plus:
					if (value > 0 || std::isnan(value))
					{
						*it++ = CharT('+');
					}
					break;
				case sign_t::space:
					if (value > 0 || std::isnan(value))
					{
						*it++ = CharT(' ');
					}
					break;
			}
			return it;
		};

		switch (type)
		{
			case CharT('a'):
			case CharT('A'):
				start = do_sign(start);
				if constexpr (is_regular_char)
				{
					std::to_chars_result r;
					if (precision > 0)
					{
						r = std::to_chars(start, buf.data() + buf.size(), value, std::chars_format::hex, precision);
					}
					else
					{
						r = std::to_chars(start, buf.data() + buf.size(), value, std::chars_format::hex);
					}

					str = {buf.data(), r.ptr};
					if (type == CharT('A'))
					{
						std::transform(std::begin(buf), std::begin(buf) + str.size(), std::begin(buf), [](unsigned char c) { return static_cast<CharT>(std::toupper(c)); });
					}
				}
				else
				{
					int result;
					if (precision > 0)
					{
						const auto format = type == 'a' ? cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::hex_small_precision, wchar_t>().data() : cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::hex_big_precision, wchar_t>().data();
						result = std::swprintf(start, buf.size() - extra_size(), format, precision, value);
					}
					else
					{
						const auto format = type == 'a' ? cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::hex_small, wchar_t>().data() : cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::hex_big, wchar_t>().data();
						result = std::swprintf(start, buf.size() - extra_size(), format, value);
					}

					str = {buf.data(), static_cast<std::size_t>(result) + extra_size()};
				}
				break;
			case CharT('e'):
			case CharT('E'):
				start = do_sign(start);
				if constexpr (is_regular_char)
				{
					const auto forced_precision = precision < 0 ? 6 : precision;
					const auto r = std::to_chars(start, buf.data() + buf.size(), value, std::chars_format::scientific, precision);
					str = {buf.data(), r.ptr};

					if (type == CharT('E'))
					{
						auto e_it = std::find(buf.begin(), buf.end(), CharT('e'));
						if (e_it != buf.end())
						{
							*e_it = CharT('E');
						}
					}
				}
				else
				{
					const auto forced_precision = precision < 0 ? 6 : precision;
					const auto format = type == 'e' ? cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_exponent_small_precision, wchar_t>().data() : cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_exponent_big_precision, wchar_t>().data();
					const auto result = std::swprintf(start, buf.size() - extra_size(), format, precision, value);

					str = {buf.data(), static_cast<std::size_t>(result) + extra_size()};
				}
				break;
			case CharT('f'):
			case CharT('F'):
				start = do_sign(start);
				if constexpr (is_regular_char)
				{
					const auto forced_precision = precision < 0 ? 6 : precision;
					const auto r = std::to_chars(start, buf.data() + buf.size(), value, std::chars_format::fixed, precision);
					str = {buf.data(), r.ptr};
				}
				else
				{
					const auto forced_precision = precision < 0 ? 6 : precision;
					const auto format = type == 'f' ? cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_decimal_precision, wchar_t>().data() : cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_decimal_precision, wchar_t>().data();
					const auto result = std::swprintf(start, buf.size() - extra_size(), format, precision, value);

					str = {buf.data(), static_cast<std::size_t>(result) + extra_size()};
				}
				break;
			case CharT('g'):
			case CharT('G'):
				start = do_sign(start);
				if constexpr (is_regular_char)
				{
					const auto forced_precision = precision < 0 ? 6 : precision;
					const auto r = std::to_chars(start, buf.data() + buf.size(), value, std::chars_format::general, precision);
					str = {buf.data(), r.ptr};

					if (type == CharT('G'))
					{
						auto e_it = std::find(buf.begin(), buf.end(), CharT('e'));
						if (e_it != buf.end())
						{
							*e_it = CharT('E');
						}
					}
				}
				else
				{
					const auto forced_precision = precision < 0 ? 6 : precision;
					const auto format = type == 'g' ? cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_general_small_precision, wchar_t>().data() : cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_general_big_precision, wchar_t>().data();
					const auto result = std::swprintf(start, buf.size() - extra_size(), format, precision, value);

					str = {buf.data(), static_cast<std::size_t>(result) + extra_size()};
				}
				break;
			case CharT('L'):
				{
					std::basic_stringstream<CharT> ss;
					ss.imbue(ctx.locale());
					ss << std::defaultfloat;
					if (precision > 0)
					{
						ss << std::setprecision(precision);
					}
					ss << value;
					dyn_buf = ss.str();
					do_sign(std::inserter(dyn_buf, dyn_buf.begin()));
					str = {dyn_buf.data(), dyn_buf.size()};
					break;
				}
			default:
				start = do_sign(start);
				if constexpr (is_regular_char)
				{
					std::to_chars_result r;
					if (precision > 0)
					{
						r = std::to_chars(start, buf.data() + buf.size(), value, std::chars_format::general, precision);
					}
					else
					{
						r = std::to_chars(start, buf.data() + buf.size(), value);
					}

					str = {buf.data(), r.ptr};
				}
				else
				{
					int result;
					if (precision > 0)
					{
						const auto format = cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_general_small_precision, wchar_t>().data();
						result = std::swprintf(start, buf.size() - extra_size(), format, precision, value);
					}
					else
					{
						const auto format = cmoon::printf_format_specifier_value<T, cmoon::printf_conversion_specifier::floating_general_small, wchar_t>().data();
						result = std::swprintf(start, buf.size() - extra_size(), format, value);
					}

					str = {buf.data(), static_cast<std::size_t>(result) + extra_size()};
				}
				break;
		}

		return write_string_view(str, ctx, spec_info);
	}

	export
	template<typename CharT, class Traits>
	struct formatter<std::basic_string_view<CharT, Traits>, CharT> : public base_formatter<std::basic_string_view<CharT, Traits>, CharT>
	{
		template<class OutputIt>
		auto format(std::basic_string_view<CharT, Traits> str, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_string_view(str, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<CharT, CharT> : public base_formatter<CharT, CharT>
	{
		template<class OutputIt>
		auto format(CharT c, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_string_view(std::basic_string_view<CharT>{std::addressof(c), 1}, ctx, this->parser);
		}
	};

	export
	template<>
	struct formatter<char, wchar_t> : public base_formatter<char, wchar_t>
	{
		template<class OutputIt>
		auto format(char c, basic_format_context<OutputIt, wchar_t>& ctx)
		{
			wchar_t wide;
			std::mbtowc(std::addressof(wide), std::addressof(c), 1);
			return write_string_view(std::wstring_view{std::addressof(wide), 1}, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<const CharT*, CharT> : public formatter<std::basic_string_view<CharT>, CharT>
	{
		template<class OutputIt>
		auto format(const CharT* c, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>::format(c, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<CharT*, CharT> : public formatter<std::basic_string_view<CharT>, CharT>
	{
		template<class OutputIt>
		auto format(CharT* c, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>(c, ctx);
		}
	};

	export
	template<typename CharT, std::size_t N>
	struct formatter<const CharT[N], CharT> : public formatter<std::basic_string_view<CharT>, CharT>
	{
		template<class OutputIt>
		auto format(const CharT c[N], basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>::format({c, N}, ctx);
		}
	};

	export
	template<typename CharT, class Traits, class Alloc>
	struct formatter<std::basic_string<CharT, Traits, Alloc>, CharT> : public formatter<std::basic_string_view<CharT, Traits>, CharT>
	{
		template<class OutputIt>
		auto format(const std::basic_string<CharT, Traits, Alloc>& str, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<std::basic_string_view<CharT>, CharT>::format(str, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<int, CharT> : public base_formatter<int, CharT>
	{
		template<class OutputIt>
		auto format(int value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<short, CharT> : public base_formatter<short, CharT>
	{
		template<class OutputIt>
		auto format(short value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned short, CharT> : public base_formatter<unsigned short, CharT>
	{
		template<class OutputIt>
		auto format(unsigned short value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned int, CharT> : public base_formatter<unsigned int, CharT>
	{
		template<class OutputIt>
		auto format(unsigned int value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<long, CharT> : public base_formatter<long, CharT>
	{
		template<class OutputIt>
		auto format(long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned long, CharT> : public base_formatter<unsigned long, CharT>
	{
		template<class OutputIt>
		auto format(unsigned long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<long long, CharT> : public base_formatter<long long, CharT>
	{
		template<class OutputIt>
		auto format(long long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<unsigned long long, CharT> : public base_formatter<unsigned long long, CharT>
	{
		template<class OutputIt>
		auto format(unsigned long long value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_integer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<bool, CharT> : public base_formatter<bool, CharT>
	{
		template<class OutputIt>
		auto format(bool value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_boolean(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<const void*, CharT> : public base_formatter<const void*, CharT>
	{
		template<class OutputIt>
		auto format(const void* value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_pointer(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<std::nullptr_t, CharT> : public formatter<const void*, CharT>
	{
		template<class OutputIt>
		auto format(std::nullptr_t value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<const void*, CharT>::format(value, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<void*, CharT> : public formatter<const void*, CharT>
	{
		template<class OutputIt>
		auto format(void* value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<const void*, CharT>::format(value, ctx);
		}
	};

	export
	template<typename CharT>
	struct formatter<double, CharT> : public base_formatter<double, CharT>
	{
		template<class OutputIt>
		auto format(double value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_floating(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<long double, CharT> : public base_formatter<long double, CharT>
	{
		template<class OutputIt>
		auto format(long double value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return write_floating(value, ctx, this->parser);
		}
	};

	export
	template<typename CharT>
	struct formatter<float, CharT> : public formatter<double, CharT>
	{
		template<class OutputIt>
		auto format(float value, basic_format_context<OutputIt, CharT>& ctx)
		{
			return formatter<double, CharT>::format(value, ctx);
		}
	};

	export
	using format_args = basic_format_args<format_context>;

	export
	using wformat_args = basic_format_args<wformat_context>;

	export
	template<class Ctx, class... Args>
	format_arg_store<Ctx, Args...> make_format_args(const Args&... args)
	{
		return {args...};
	}

	export
	template<class... Args>
	format_arg_store<wformat_context, Args...> make_wformat_args(const Args&... args)
	{
		return {args...};
	}

	namespace details
	{
		template<typename CharT, class Out>
		Out vformat_to(Out out, const std::locale& loc, std::basic_string_view<CharT> fmt, format_args_t<Out, CharT>&& args)
		{
			basic_format_parse_context<CharT> parse_context {fmt, args.data_.size()};
			basic_format_context<Out, CharT> format_context {std::move(args), out, loc};

			constexpr auto open_curly = CharT('{');
			constexpr auto close_curly = CharT('}');
			constexpr auto colon = CharT(':');

			bool in_open_curly {false};
			bool in_close_curly {false};
			while (!at_end(parse_context))
			{
				const auto ch = get_char(parse_context);
				if (in_open_curly)
				{
					if (ch == open_curly)
					{
						set_char(format_context, open_curly);
						advance_one(format_context);
						advance_one(parse_context);
					}
					else
					{
						const auto id = parse_arg_id(parse_context);
						if (get_char(parse_context) == colon)
						{
							advance_one(parse_context);
						}

						// TODO: Replace with 'visit_format_arg' at some point.
						// MSVC won't let me and is giving me error C1126.
						std::visit(cmoon::overloaded (
							[](std::monostate) {
								throw format_error{"Invalid format argument"};
							},
							[&parse_context, &format_context](const auto& v) {
								// Cannot have as another overload, because MSVC will try to generate
								// a formatter with type of basic_format_arg::handle, which does not exist.
								// So I need to constexpr that out.
								if constexpr (std::same_as<std::decay_t<decltype(v)>, typename basic_format_arg<decltype(format_context)>::handle>)
								{
									v.format(parse_context, format_context);
								}
								else
								{
									formatter<std::decay_t<decltype(v)>, CharT> form;
									parse_context.advance_to(form.parse(parse_context));
									format_context.advance_to(form.format(v, format_context));
								}
							}
						), format_context.arg(id).value);

						if (at_end(parse_context))
						{
							throw format_error{"invalid format string"};
						}
						else
						{
							advance_one(parse_context);
						}
					}

					in_open_curly = false;
				}
				else if (in_close_curly)
				{
					if (ch == close_curly)
					{
						set_char(format_context, close_curly);
						advance_one(format_context);
						advance_one(parse_context);
					}
					else
					{
						throw format_error{"invalid format string"};
					}

					in_close_curly = false;
				}
				else if (ch == open_curly)
				{
					in_open_curly = true;
					advance_one(parse_context);
				}
				else if (ch == close_curly)
				{
					in_close_curly = true;
					advance_one(parse_context);
				}
				else
				{
					set_char(format_context, ch);
					advance_one(format_context);
					advance_one(parse_context);
				}
			}

			return format_context.out();
		}
	}

	export
	template<class T, class Context = format_context>
	concept can_format = 
		requires()
	{
		typename Context::template formatter_type<std::decay_t<T>>;
		sizeof(typename Context::template formatter_type<std::decay_t<T>>);
	};

	export
	template<std::output_iterator<char> Out>
	Out vformat_to(Out out, const std::locale& loc, std::string_view fmt, format_args_t<std::type_identity_t<Out>, char> args)
	{
		return details::vformat_to(out, loc, fmt, std::move(args));
	}

	export
	template<std::output_iterator<wchar_t> Out>
	Out vformat_to(Out out, const std::locale& loc, std::wstring_view fmt, format_args_t<std::type_identity_t<Out>, char> args)
	{
		return details::vformat_to(out, loc, fmt, std::move(args));
	}

	export
	template<std::output_iterator<char> Out>
	Out vformat_to(Out out, std::string_view fmt, format_args_t<Out, char> args)
	{
		return details::vformat_to(out, std::locale{}, fmt, std::move(args));
	}

	export
	template<std::output_iterator<wchar_t> Out>
	Out vformat_to(Out out, std::wstring_view fmt, format_args_t<Out, wchar_t> args)
	{
		return details::vformat_to(out, std::locale{}, fmt, std::move(args));
	}

	export
	std::string vformat(const std::locale& loc, std::string_view fmt, format_args args)
	{
		std::string result;
		details::vformat_to(std::back_inserter(result), loc, fmt, std::move(args));
		return result;
	}

	export
	std::wstring vformat(const std::locale& loc, std::wstring_view fmt, wformat_args args)
	{
		std::wstring result;
		details::vformat_to(std::back_inserter(result), loc, fmt, std::move(args));
		return result;
	}

	export
	std::string vformat(std::string_view fmt, format_args args)
	{
		std::string result;
		details::vformat_to(std::back_inserter(result), std::locale{}, fmt, std::move(args));
		return result;
	}

	export
	std::wstring vformat(std::wstring_view fmt, wformat_args args)
	{
		std::wstring result;
		details::vformat_to(std::back_inserter(result), std::locale{}, fmt, std::move(args));
		return result;
	}

	export
	template<class... Args>
		requires(can_format<Args, format_context> && ...)
	std::string format(const std::locale& l, std::string_view fmt, const Args&... args)
	{
		return vformat(l, fmt, make_format_args<format_context>(args...));
	}

	export
	template<class... Args>
		requires(can_format<Args, wformat_context> && ...)
	std::wstring format(const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		return vformat(l, fmt, make_wformat_args(args...));
	}

	export
	template<class... Args>
		requires(can_format<Args, format_context> && ...)
	std::string format(std::string_view fmt, const Args&... args)
	{
		return vformat(fmt, make_format_args<format_context>(args...));
	}

	export
	template<class... Args>
		requires(can_format<Args, wformat_context> && ...)
	std::wstring format(std::wstring_view fmt, const Args&... args)
	{
		return vformat(fmt, make_wformat_args(args...));
	}

	export
	template<std::output_iterator<char> Out, class... Args>
	Out format_to(Out out, const std::locale& l, std::string_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, char>;
		return vformat_to(out, l, fmt, {make_format_args<context>(args...)});
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	Out format_to(Out out, const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, wchar_t>;
		return vformat_to(out, l, fmt, {make_wformat_args<context>(args...)});
	}

	export
	template<std::output_iterator<char> Out, class... Args>
	Out format_to(Out out, std::string_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, char>;
		return vformat_to(out, fmt, {make_format_args<context>(args...)});
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	Out format_to(Out out, std::wstring_view fmt, const Args&... args)
	{
		using context = basic_format_context<Out, wchar_t>;
		return vformat_to(out, fmt, {make_format_args<context>(args...)});
	}

	export
	template<class Out>
	struct format_to_n_result
	{
		Out out;
		std::iter_difference_t<Out> size;
	};

	template<class CharT>
	class format_size_counter
	{
		public:
			using iterator_category = std::output_iterator_tag;
			using value_type = CharT;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

			constexpr format_size_counter() noexcept = default;

			constexpr reference operator*() noexcept
			{
				++count_;
				return dummy_;
			}

			constexpr format_size_counter& operator++() noexcept
			{
				return *this;
			}

			constexpr format_size_counter operator++(int) noexcept
			{
				auto copy = *this;
				++*this;
				return copy;
			}

			[[nodiscard]] constexpr std::size_t count() const noexcept
			{
				return count_;
			}
		private:
			std::size_t count_ {};
			value_type dummy_;
	};

	template<class Out>
	class format_to_n_tracker
	{
		public:
			using iterator_category = std::output_iterator_tag;
			using value_type = std::iter_value_t<Out>;
			using difference_type = std::iter_difference_t<Out>;
			using pointer = value_type*;
			using reference = std::iter_reference_t<Out>;

			constexpr format_to_n_tracker() noexcept(std::is_nothrow_default_constructible_v<Out>) = default;

			constexpr format_to_n_tracker(Out out, difference_type limit)
				: out_{out}, limit_{limit} {}

			[[nodiscard]] constexpr Out out() noexcept
			{
				return out_;
			}

			constexpr reference operator*() noexcept(noexcept(*std::declval<Out>()))
			{
				if (limit_ == 0)
				{
					return dummy_;
				}

				--limit_;
				return *out_;
			}

			constexpr format_to_n_tracker& operator++() noexcept(noexcept(++std::declval<Out>()))
			{
				++out_;
				return *this;
			}

			constexpr format_to_n_tracker operator++(int)  noexcept(noexcept(std::declval<Out>()++))
			{
				auto copy = *this;
				++*this;
				return copy;
			}

			[[nodiscard]] constexpr difference_type limit() const noexcept
			{
				return limit_;
			}
		private:
			Out out_ {};
			difference_type limit_ {};
			value_type dummy_ {};
	};

	export
	template<std::output_iterator<char> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, const std::locale& l, std::string_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, l, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, l, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<std::output_iterator<char> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, std::string_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<std::output_iterator<wchar_t> Out, class... Args>
	format_to_n_result<Out> format_to_n(Out out, std::iter_difference_t<Out> n, std::wstring_view fmt, const Args&... args)
	{
		format_to_n_tracker tracker{out, n};
		tracker = format_to(tracker, fmt, args...);
		return {tracker.out(), n - tracker.limit()};
	}

	export
	template<class... Args>
	std::size_t formatted_size(const std::locale& l, std::string_view fmt, const Args&... args)
	{
		format_size_counter<char> c;
		c = format_to(c, l, fmt, args...);
		return c.count();
	}

	export
	template<class... Args>
	std::size_t formatted_size(const std::locale& l, std::wstring_view fmt, const Args&... args)
	{
		format_size_counter<wchar_t> c;
		c = format_to(c, l, fmt, args...);
		return c.count();
	}

	export
	template<class... Args>
	std::size_t formatted_size(std::string_view fmt, const Args&... args)
	{
		format_size_counter<char> c;
		c = format_to(c, fmt, args...);
		return c.count();
	}

	export
	template<class... Args>
	std::size_t formatted_size(std::wstring_view fmt, const Args&... args)
	{
		format_size_counter<wchar_t> c;
		c = format_to(c, fmt, args...);
		return c.count();
	}
}