export module cmoon.format.write_pointer;

import <array>;
import <string_view>;
import <limits>;
import <charconv>;
import <cwchar>;

import cmoon.string;

import cmoon.format.basic_format_context;
import cmoon.format.format_parser;
import cmoon.format.write_string_view;

namespace cmoon
{
	export
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
}