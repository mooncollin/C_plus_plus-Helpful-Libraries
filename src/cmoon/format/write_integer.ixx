export module cmoon.format.write_integer;

import <concepts>;
import <string_view>;
import <array>;
import <string>;
import <charconv>;
import <cwchar>;

import cmoon.string;

import cmoon.format.basic_format_context;
import cmoon.format.format_parser;
import cmoon.format.format_specs;
import cmoon.format.write_string_view;

namespace cmoon
{
	export
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
}