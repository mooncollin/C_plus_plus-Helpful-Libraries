#include "cmoon/string/literals.hpp"

export module cmoon.string.utility;

import <string_view>;
import <iterator>;
import <array>;
import <algorithm>;
import <bit>;
import <type_traits>;

import cmoon.string.literals;
import cmoon.platform;

namespace cmoon
{
	export
    enum class printf_conversion_specifier
    {
        character,
        string,
        integer,
        unsigned_integer,
        octal,
        hex_small,
        hex_big,
        hex_small_precision,
        hex_big_precision,
        floating_decimal,
        floating_exponent_small,
        floating_exponent_big,
        floating_general_small,
        floating_general_big,
        floating_decimal_precision,
        floating_exponent_small_precision,
        floating_exponent_big_precision,
        floating_general_small_precision,
        floating_general_big_precision,
    };

    export
    template<typename T, printf_conversion_specifier S, class CharT>
    struct printf_format_specifier;

    export
    template<class CharT>
    struct printf_format_specifier<char, printf_conversion_specifier::character, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%c"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<int, printf_conversion_specifier::character, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%c"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<short, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hd"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hu"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%ho"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hx"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hX"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<int, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%d"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%u"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%o"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%x"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%X"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%ld"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lu"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lo"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lx"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lX"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long long, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lld"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llu"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llo"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llx"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llX"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_decimal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%f"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_decimal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%fL"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%e"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%eL"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%E"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%EL"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%a"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view value = choose_str_literal<CharT>(STR_LITERALS("%aL"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%A"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%AL"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%g"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%Lg"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%G"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%LG"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_decimal_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*f"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_decimal_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*Lf"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*e"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*Le"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*E"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*LE"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*a"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*La"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*A"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*LA"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*g"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*Lg"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*G"));
    };

    export
    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*LG"));
    };

    export
    template<typename T, printf_conversion_specifier S, typename CharT>
        requires(requires {
            typename printf_format_specifier<T, S, CharT>;
        })
    inline constexpr auto printf_format_specifier_v = printf_format_specifier<T, S, CharT>::value;

    export
    template<typename T, printf_conversion_specifier S, typename CharT>
        requires(requires {
            typename printf_format_specifier<T, S, CharT>;
        })
    [[nodiscard]] constexpr auto printf_format_specifier_value() noexcept
    {
        return printf_format_specifier<T, S, CharT>::value;
    }

    export
    template<typename T, typename CharT>
    [[nodiscard]] constexpr auto printf_format_specifier_value() noexcept
    {
        constexpr printf_conversion_specifier spec = std::conditional_t<std::signed_integral<T>,
                                                        std::integral_constant<printf_conversion_specifier, printf_conversion_specifier::integer>,
                                                        std::conditional_t<std::unsigned_integral<T>,
                                                            std::integral_constant<printf_conversion_specifier, printf_conversion_specifier::unsigned_integer>,
                                                            std::conditional_t<std::floating_point<T>,
                                                                std::integral_constant<printf_conversion_specifier, printf_conversion_specifier::floating_decimal>,
                                                                std::conditional_t<std::same_as<T, char>, 
                                                                    std::integral_constant<printf_conversion_specifier, printf_conversion_specifier::character>,
                                                                    std::integral_constant<printf_conversion_specifier, printf_conversion_specifier::string>
                                                                >
                                                            >
                                                        >
                                                    >::value;

        return printf_format_specifier_value<T, spec, CharT>();
    }

    export
    template<std::input_or_output_iterator OutputIt, std::integral T>
    constexpr OutputIt to_binary_string(OutputIt begin, OutputIt end, T value)
    {
        if constexpr (std::bidirectional_iterator<OutputIt>)
        {
            std::array<std::iter_value_t<OutputIt>, cmoon::bits_in_type<T>> buf;
            auto next = buf.end();

            do
            {
                *--next = std::iter_value_t<OutputIt>('0' + (value & 0b1));
                value >>= 1;
            } while (value != 0);

            return std::copy_n(next, static_cast<std::size_t>(buf.end() - next), begin);
        }
        else
        {
            const std::size_t b_end = cmoon::bits_in_type<T> - std::countl_zero(value);

            std::size_t mask = static_cast<std::size_t>(1) << (b_end - 1);
            for (std::size_t i {0}; i < b_end && begin != end; ++i, ++begin)
            {
                *begin = std::iter_value_t<OutputIt>('0' + ((value & mask) >> (b_end - i - 1)));
                mask >>= 1;
            }

            return begin;
        }
    }
}