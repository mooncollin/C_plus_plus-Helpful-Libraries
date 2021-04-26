module;

#include <limits>

#include "cmoon/string_literals.hpp"

export module cmoon.format.utility;
import cmoon.string;
import cmoon.format.format_error;
import cmoon.format.basic_format_context;
import cmoon.format.basic_format_parse_context;

namespace cmoon
{
	export
	template<typename CharT>
	[[nodiscard]] constexpr bool at_end(const basic_format_parse_context<CharT>& parse_ctx)
	{
		return parse_ctx.begin() == parse_ctx.end();
	}

	export
	template<typename CharT>
	[[nodiscard]] constexpr CharT get_char(const basic_format_parse_context<CharT>& parse_ctx)
	{
		return *parse_ctx.begin();
	}

	export
	template<class OutputIt, typename CharT>
	[[nodiscard]] constexpr void set_char(basic_format_context<OutputIt, CharT>& format_ctx, CharT c)
	{
		*format_ctx.out() = c;
	}

	export
	template<typename CharT>
	[[nodiscard]] constexpr void advance_one(basic_format_parse_context<CharT>& parse_ctx)
	{
		parse_ctx.advance_to(parse_ctx.begin() + 1);
	}

	export
	template<class OutputIt, typename CharT>
	[[nodiscard]] constexpr void advance_one(basic_format_context<OutputIt, CharT>& format_ctx)
	{
		format_ctx.advance_to(++format_ctx.out());
	}

	export
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

	export
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
}