export module cmoon.format.write_floating;

import <concepts>;
import <array>;
import <string>;
import <string_view>;
import <charconv>;

import cmoon.format.basic_format_context;
import cmoon.format.format_parser;
import cmoon.format.format_specs;
import cmoon.format.write_string_view;

namespace cmoon
{
	export
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
}