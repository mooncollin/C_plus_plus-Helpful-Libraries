#pragma once

#include <unordered_set>
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

#include "collin/platform.hpp"
#include "collin/type_traits.hpp"
#include "collin/utility.hpp"
#include "collin/concepts.hpp"

namespace collin
{
    namespace strings
    {
        constexpr std::string_view line_terminator = collin::platform::win32_api ? "\r\n"
                                                                                 : "\n";

        template<class CharT>
        constexpr bool is_vowel(CharT c) noexcept
        {
            switch (std::tolower(c))
            {
                case static_cast<CharT>('a'):
                case static_cast<CharT>('e'):
                case static_cast<CharT>('i'):
                case static_cast<CharT>('o'):
                case static_cast<CharT>('u'):
                    return true;
                default:
                    return false;
            }
        }

        template<class CharT>
        constexpr bool is_consonant(CharT c) noexcept
        {
            return !is_vowel(c);
        }

        template<class InputIterator>
        constexpr bool contains_vowel(InputIterator first, InputIterator last) noexcept
        {
            return std::any_of(first, last, [&](auto& val){return is_vowel(val);});
        }

        template<class InputIterator>
        constexpr bool contains_consonant(InputIterator first, InputIterator last) noexcept
        {
            return std::any_of(first, last, [&](auto& val){return is_consonant(val);});
        }

        template<class CharT, class Traits>
        constexpr bool contains_vowel(std::basic_string_view<CharT, Traits> str) noexcept
        {
            return contains_vowel(std::begin(str), std::end(str));
        }

        template<class CharT, class Traits>
        constexpr bool contains_consonant(std::basic_string_view<CharT, Traits> str) noexcept
        {
            return contains_consonant(std::begin(str), std::end(str));
        }

        template<class InputIterator>
        InputIterator remove_punctuation(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::ispunct(ch, loc); });
        }

        template<class Container>
        void remove_punctuation(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_punctuation(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_spaces(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isspace(ch, loc);});
        }

        template<class Container>
        void remove_spaces(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_spaces(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_alphanumerics(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isalnum(ch, loc);});
        }

        template<class Container>
        void remove_alphanumerics(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_alphanumerics(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_alphas(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch){return std::isalpha(ch, loc);});
        }

        template<class Container>
        void remove_alphas(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_alphas(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_digits(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isdigit(ch, loc);});
        }

        template<class Container>
        void remove_digits(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_digits(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_blanks(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isblank(ch, loc);});
        }

        template<class Container>
        void remove_blanks(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_blanks(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_cntrls(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::iscntrl(ch, loc);});
        }

        template<class Container>
        void remove_cntrls(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_cntrls(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_graphicals(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isgraph(ch, loc);});
        }

        template<class Container>
        void remove_graphicals(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_graphicals(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_printables(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isprint(ch, loc);});
        }

        template<class Container>
        void remove_printables(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_printables(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_hexdigits(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isxdigit(ch, loc);});
        }

        template<class Container>
        void remove_hexdigits(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_hexdigits(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_upper(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::isupper(ch, loc);});
        }

        template<class Container>
        void remove_upper(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_upper(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator>
        InputIterator remove_lower(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::remove_if(first, last, [&](auto ch) {return std::islower(ch, loc);});
        }

        template<class Container>
        void remove_lower(Container& container, const std::locale& loc = {})
        {
            container.erase(remove_lower(std::begin(container), std::end(container), loc), std::end(container));
        }

        template<class InputIterator, class CharT>
        InputIterator remove_character(InputIterator first, InputIterator last, CharT c)
        {
            return std::remove_if(first, last, [=](auto ch) {return ch == c;});
        }

        template<class Container, class CharT>
        void remove_character(Container& container, CharT c)
        {
            container.erase(remove_character(std::begin(container), std::end(container), c), std::end(container));
        }

        template<class InputIterator, class OutputIterator>
        OutputIterator uppercase(InputIterator first, InputIterator last, OutputIterator first2, const std::locale& loc = {})
        {
            return std::transform(first, last, first2, [&](auto ch) {return std::toupper(ch, loc);});
        }

        template<class Container>
        void uppercase(Container& container, const std::locale& loc = {})
        {
            uppercase(std::begin(container), std::end(container), std::begin(container), loc);
        }

        template<class InputIterator, class OutputIterator>
        OutputIterator lowercase(InputIterator first, InputIterator last, OutputIterator first2, const std::locale& loc = {})
        {
            return std::transform(first, last, first2, [&](auto ch) {return std::tolower(ch, loc);});
        }

        template<class Container>
        void lowercase(Container& container, const std::locale& loc = {})
        {
            lowercase(std::begin(container), std::end(container), std::begin(container), loc);
        }

        template<class CharT, class Traits>
        constexpr std::basic_string_view<CharT, Traits> trim_front(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
        {
            const auto start_position = s.find_first_not_of(trim_str);
            if (start_position == std::basic_string_view<CharT, Traits>::npos)
            {
                return "";
            }

            return s.substr(start_position);
        }

        template<class CharT, class Traits>
        void trim_front(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
        {
            const auto start_position = s.find_first_not_of(trim_str);
            if (start_position == std::basic_string<CharT, Traits>::npos)
            {
                s = "";
            }
            s.erase(std::begin(s), std::begin(s) + start_position);
        }

        template<class CharT, class Traits>
        constexpr std::basic_string_view<CharT, Traits> trim_back(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
        {
            const auto end_position = s.find_last_not_of(trim_str);
            if (end_position == std::basic_string_view<CharT, Traits>::npos)
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
        constexpr std::basic_string_view<CharT, Traits> trim(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
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

        template<class T, class CharT, class Traits>
        T from_string(std::basic_string_view<CharT, Traits> str)
        {
            if constexpr(std::same_as<CharT, char> && details::from_chars_valid<T>)
            {
                T value;
                const auto result = std::from_chars(str.data(), str.data() + std::size(str), value);
                if (result.ec != std::errc{})
                {
                    throw bad_string_conversion{};
                }
                return value;
            }
            else if constexpr (std::same_as<T, std::basic_string<CharT, Traits>>)
            {
                return std::basic_string<CharT, Traits>{str};
            }
            else if constexpr (std::same_as<T, std::basic_string_view<CharT, Traits>>)
            {
                return str;
            }
            else if constexpr (std::same_as<T, CharT>)
            {
                return str.front();
            }
            else
            {
                T value;
                std::istringstream ss(str.data());
                ss >> value;
                if (!ss)
                {
                    throw bad_string_conversion{};
                }
                return value;
            }

        }

        namespace details
        {
            template<class T>
            concept to_chars_valid =
                requires(T v)
            {
                std::numeric_limits<T>::digits10;
                std::to_chars(std::declval<char*>(), std::declval<char*>(), v);
            };
        }

        template<class T, class CharT = char, class Traits = std::char_traits<CharT>>
        std::basic_string<CharT, Traits> to_string(const T& value)
        {
            if constexpr (std::same_as<CharT, char> && details::to_chars_valid<T>)
            {
                constexpr std::size_t one_plus_digits10 = 1;
                constexpr std::size_t minus_sign = 1;
                constexpr std::size_t max_size = std::numeric_limits<T>::digits10 + one_plus_digits10 + minus_sign;

                std::basic_string<CharT, Traits> str;
                str.resize(max_size);
                const auto result = std::to_chars(std::begin(str), std::end(str), value);
                if (result.ec != std::errc{})
                {
                    throw bad_string_conversion{};
                }

                str.shrink_to_fit();
                return str;
            }
            else
            {
                std::basic_ostringstream<CharT, Traits> ss;
                ss << value;
                if (!ss)
                {
                    throw bad_string_conversion{};
                }
                return ss.str();
            }
        }

        template<class OutputIterator, class CharT, class Traits>
            requires(requires(OutputIterator& it, std::basic_string_view<CharT, Traits> c) { *it = c; })
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

        template<class InputIterator, class CharT, class Traits>
        std::basic_string<CharT, Traits> join(InputIterator begin, InputIterator end, std::basic_string_view<CharT, Traits> join_string)
        {
            std::basic_stringstream<CharT, Traits> ss;
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

        template<class Container, class CharT, class Traits>
        std::basic_string<CharT, Traits> join(const Container& container, std::basic_string_view<CharT, Traits> join_string)
        {
            return join(std::cbegin(container), std::cend(container), join_string);
        }

        template<collin::concepts::integral CharT>
        constexpr int hex_to_base10(CharT ch) noexcept
        {
            const auto uppered = std::toupper(ch);
            return uppered >= 'A' ? uppered - 'A' + 10 : uppered - '0';
        }
    }
}