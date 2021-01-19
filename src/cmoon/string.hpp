#pragma once

#include <algorithm>
#include <functional>
#include <type_traits>
#include <string>
#include <regex>
#include <cctype>
#include <sstream>
#include <string_view>
#include <array>
#include <locale>
#include <utility>
#include <initializer_list>
#include <charconv>
#include <utility>
#include <system_error>
#include <concepts>
#include <limits>
#include <iterator>
#include <ranges>
#include <locale>
#include <bit>

#include "cmoon/platform.hpp"
#include "cmoon/utility.hpp"
#include "cmoon/concepts.hpp"

namespace cmoon
{
    template<class T, cmoon::string_literal... Args>
    constexpr auto choose_str_literal(Args&&... args) noexcept
    {
        using proper_type = std::add_pointer_t<std::add_const_t<std::remove_cvref_t<std::remove_pointer_t<T>>>>;
        return choose_on_type<proper_type>(std::forward<Args>(args)...);
    }

    template<class T, cmoon::char_literal... Args>
    constexpr auto choose_char_literal(Args&&... args) noexcept
    {
        return choose_on_type<T>(std::forward<Args>(args)...);
    }
    
    #define STR_LITERALS(...) __VA_ARGS__, L##__VA_ARGS__, u8##__VA_ARGS__, u##__VA_ARGS__, U##__VA_ARGS__
    #define CHAR_LITERALS(...) STR_LITERALS(__VA_ARGS__)

    template<class CharT = char>
    constexpr std::basic_string_view<CharT> line_terminator = cmoon::win32_api ? choose_str_literal<CharT>(STR_LITERALS("\r\n"))
                                                                               : choose_str_literal<CharT>(STR_LITERALS("\n"));

    template<std::input_iterator InputIterator>
    auto remove_punctuation(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::ispunct(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_punctuation(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::ispunct(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_punctuation(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_punctuation(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_spaces(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isspace(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_spaces(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isspace(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_spaces(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_spaces(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_alphanumerics(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isalnum(ch, loc);});
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_alphanumerics(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isalnum(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_alphanumerics(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_alphanumerics(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_alphas(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch){ return std::isalpha(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_alphas(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch){ return std::isalpha(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_alphas(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_alphas(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_digits(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isdigit(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_digits(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isdigit(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_digits(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_digits(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_blanks(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isblank(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_blanks(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isblank(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_blanks(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_blanks(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_cntrls(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::iscntrl(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_cntrls(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::iscntrl(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_cntrls(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(remove_cntrls(container, loc, proj), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_graphicals(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isgraph(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_graphicals(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isgraph(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_graphicals(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(remove_graphicals(container, loc, proj), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_printables(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isprint(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_printables(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isprint(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_printables(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(remove_printables(container, loc, proj), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_hexdigits(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isxdigit(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_hexdigits(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isxdigit(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_hexdigits(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_hexdigits(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_upper(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isupper(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_upper(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isupper(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_upper(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_upper(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator>
    auto remove_lower(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::islower(ch, loc); });
    }

    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_lower(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::islower(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void erase_lower(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_lower(container, loc, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator, class CharT>
    constexpr auto remove_character(InputIterator first, InputIterator last, CharT c)
    {
        return std::remove_if(first, last, [c](const auto ch) { return ch == c; });
    }

    template<std::ranges::input_range Range, class CharT, class Proj = std::identity>
    constexpr auto remove_character(Range&& r, CharT c, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [c](const auto ch) { return ch == c; }, proj);
    }

    template<class Container, class CharT, class Proj = std::identity>
    constexpr void erase_character(Container& container, CharT c, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_character(container, c, proj)), std::end(container));
    }

    template<std::input_iterator InputIterator, std::output_iterator<std::iter_value_t<InputIterator>> OutputIterator>
    auto uppercase(InputIterator first, InputIterator last, OutputIterator first2, const std::locale& loc = {})
    {
        return std::transform(first, last, first2, [&loc](const auto ch) { return std::toupper(ch, loc); });
    }

    template<std::ranges::input_range Range1, class O, class Proj = std::identity>
    auto uppercase(Range1&& r1, O out, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(std::forward<Range1>(r1), out, [&loc](const auto ch) { return std::toupper(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void uppercase(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        uppercase(container, std::begin(container), loc, proj);
    }

    template<std::input_iterator InputIterator, std::output_iterator<std::iter_value_t<InputIterator>> OutputIterator>
    auto lowercase(InputIterator first, InputIterator last, OutputIterator first2, const std::locale& loc = {})
    {
        return std::transform(first, last, first2, [&loc](const auto ch) { return std::tolower(ch, loc); });
    }

    template<std::ranges::input_range Range1, class O, class Proj = std::identity>
    auto lowercase(Range1&& r1, O out, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(std::forward<Range1>(r1), out, [&loc](const auto ch) { return std::tolower(ch, loc); }, proj);
    }

    template<class Container, class Proj = std::identity>
    void lowercase(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        lowercase(container, std::begin(container), loc, proj);
    }

    template<class CharT, class Traits>
    constexpr std::basic_string_view<CharT, Traits> trim_front(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
    {
        const auto start_position = s.find_first_not_of(trim_str);
        if (start_position == std::string_view::npos)
        {
            return "";
        }

        return s.substr(start_position);
    }

    template<class CharT, class Traits>
    void trim_front(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
    {
        const auto start_position = s.find_first_not_of(trim_str);
        if (start_position == std::string::npos)
        {
            s = {};
        }
        s.erase(std::begin(s), std::begin(s) + start_position);
    }

    template<class CharT, class Traits>
    constexpr std::basic_string_view<CharT, Traits> trim_back(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
    {
        const auto end_position = s.find_last_not_of(trim_str);
        if (end_position == std::string_view::npos)
        {
            return "";
        }

        return s.substr(0, end_position);
    }

    template<class CharT, class Traits>
    void trim_back(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
    {
        const auto end_position = s.find_last_not_of(trim_str);
        if (end_position == std::string::npos)
        {
            s = "";
        }
        s.erase(end_position);
    }

    template<class CharT, class Traits>
    constexpr std::string_view trim(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
    {
        return trim_back(trim_front(s, trim_str), trim_str);
    }

    template<class CharT, class Traits>
    void trim(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
    {
        trim_front(s, trim_str);
        trim_back(s, trim_str);
    }

    namespace details
    {
        template<class T>
        concept from_chars_valid = 
            requires(T& t, const char* begin, const char* end)
        {
            std::from_chars(begin, end, t);
        };
    }

    struct bad_string_conversion : std::bad_cast
    {
        const char* what() const override
        {
            return "bad cast from string";
        }
    };

    template<class T>
    T from_string(std::string_view str)
    {
        if constexpr(details::from_chars_valid<T>)
        {
            T value;
            const auto result = std::from_chars(str.data(), str.data() + str.size(), value);
            if (result.ec != std::errc{})
            {
                throw bad_string_conversion{};
            }
            return value;
        }
        else if constexpr (std::same_as<T, std::string>)
        {
            return std::string{str};
        }
        else if constexpr (std::same_as<T, std::string_view>)
        {
            return str;
        }
        else if constexpr (std::same_as<T, char>)
        {
            return str.front();
        }
        else
        {
            T value;
            std::istringstream ss(str.data());
            if (!(ss >> value))
            {
                throw bad_string_conversion{};
            }
            return value;
        }

    }

    template<class OutputIterator, class CharT, class Traits>
        requires(std::output_iterator<OutputIterator, std::basic_string_view<CharT, Traits>>)
    constexpr void split(std::basic_string_view<CharT, Traits> str, std::basic_string_view<CharT, Traits> delim, OutputIterator it, std::size_t amount=-1) noexcept
    {
        std::size_t last_index {0};
        std::size_t index {str.find(delim, last_index)};

        while (index != std::basic_string_view<CharT, Traits>::npos && amount != 0)
        {
            *it = str.substr(last_index, index - last_index);
            it++;
            last_index = index + 1;
            index = str.find(delim, last_index);
            amount--;
        }

        *it = str.substr(last_index);
    }

    template<class CharT, class Traits>
    void find_and_replace(std::basic_string<CharT, Traits>& input, std::basic_string_view<CharT, Traits> find, std::basic_string_view<CharT, Traits> replace)
    {
        std::size_t current_location {input.find(find)};

        while (current_location != std::basic_string<CharT, Traits>::npos)
        {
            input.replace(current_location, std::size(find), replace);
            current_location = input.find(find, current_location + 1);
        }
    }

    template<class CharT, class Traits>
    std::vector<std::basic_string_view<CharT, Traits>> split(std::basic_string_view<CharT, Traits> str, std::basic_string_view<CharT, Traits> delim, std::size_t amount=-1)
    {
        std::vector<std::basic_string_view<CharT, Traits>> results;
        split(str, delim, std::back_inserter(results), amount);
        return results;
    }

    template<std::input_iterator InputIterator, class CharT, class Traits>
    std::string join(InputIterator begin, InputIterator end, std::basic_string_view<CharT, Traits> join_string)
    {
        std::stringstream ss;
        while(begin != end)
        {
            ss << *begin;
            if(begin + 1 != end)
            {
                ss << join_string;
            }

            ++begin;
        }

        return ss.str();
    }

    template<std::ranges::input_range Range, class CharT, class Traits>
    std::basic_string<CharT, Traits> join(Range&& r, std::basic_string_view<CharT, Traits> join_string)
    {
        return join(std::ranges::begin(r), std::ranges::end(r), join_string);
    }

    int hex_to_base10(int ch) noexcept
    {
        const auto uppered = std::toupper(ch);
        return uppered >= 'A' ? uppered - 'A' + 10 : uppered - '0';
    }

    template<std::integral CharT>
    [[nodiscard]] constexpr int code_point_length(const CharT p) noexcept
    {
        if constexpr (sizeof(CharT) != 1)
        {
            return 1;
        }
        else
        {
            constexpr std::array lengths = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                                            0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 4, 0};

            const auto len = lengths[static_cast<unsigned char>(p) >> 3];
            return len + !len;
        }
    }

    template<std::integral CharT>
    [[nodiscard]] constexpr bool is_digit(CharT c) noexcept
    {
        return choose_char_literal<CharT>(CHAR_LITERALS('0')) <= c && c <= choose_char_literal<CharT>(CHAR_LITERALS('9'));
    }

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

    template<typename T, printf_conversion_specifier S, class CharT>
    struct printf_format_specifier;

    template<class CharT>
    struct printf_format_specifier<char, printf_conversion_specifier::character, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%c"));
    };

    template<class CharT>
    struct printf_format_specifier<int, printf_conversion_specifier::character, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%c"));
    };

    template<class CharT>
    struct printf_format_specifier<short, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hd"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hu"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%ho"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hx"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned short, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%hX"));
    };

    template<class CharT>
    struct printf_format_specifier<int, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%d"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%u"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%o"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%x"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned int, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%X"));
    };

    template<class CharT>
    struct printf_format_specifier<long, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%ld"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lu"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lo"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lx"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lX"));
    };

    template<class CharT>
    struct printf_format_specifier<long long, printf_conversion_specifier::integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%lld"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::unsigned_integer, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llu"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::octal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llo"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llx"));
    };

    template<class CharT>
    struct printf_format_specifier<unsigned long long, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%llX"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_decimal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%f"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_decimal, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%fL"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%e"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%eL"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%E"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%EL"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%a"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_small, CharT>
    {
        static constexpr std::basic_string_view value = choose_str_literal<CharT>(STR_LITERALS("%aL"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%A"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%AL"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%g"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_small, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%Lg"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%G"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_big, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%LG"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_decimal_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*f"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_decimal_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*Lf"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*e"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*Le"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_exponent_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*E"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_exponent_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*LE"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*a"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*La"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::hex_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*A"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::hex_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*LA"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*g"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_small_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*Lg"));
    };

    template<class CharT>
    struct printf_format_specifier<double, printf_conversion_specifier::floating_general_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*G"));
    };

    template<class CharT>
    struct printf_format_specifier<long double, printf_conversion_specifier::floating_general_big_precision, CharT>
    {
        static constexpr std::basic_string_view<CharT> value = choose_str_literal<CharT>(STR_LITERALS("%.*LG"));
    };

    template<typename T, printf_conversion_specifier S, typename CharT>
        requires(requires {
            typename printf_format_specifier<T, S, CharT>;
        })
    constexpr auto printf_format_specifier_v = printf_format_specifier<T, S, CharT>::value;

    template<typename T, printf_conversion_specifier S, typename CharT>
        requires(requires {
            typename printf_format_specifier<T, S, CharT>;
        })
    [[nodiscard]] constexpr auto printf_format_specifier_value() noexcept
    {
        return printf_format_specifier<T, S, CharT>::value;
    }

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