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

#include "type_traits.hpp"
#include "utility.hpp"

namespace collin
{
    namespace strings
    {
        const std::unordered_set vowels = {'a', 'e', 'i', 'o', 'u'};
        const std::unordered_set consonants = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k',
                'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};

        constexpr std::string_view line_terminator =
        #ifdef _WIN32
        "\r\n";
        #else
        "\n";
        #endif

        template<class CharT>
        constexpr bool is_vowel(CharT c)
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
        constexpr bool is_consonant(CharT c)
        {
            return !is_vowel(c);
        }

        template<class InputIterator>
        constexpr bool contains_vowel(InputIterator first, InputIterator last)
        {
            return std::any_of(first, last, [&](auto& val){return is_vowel(val);});
        }

        template<class InputIterator>
        constexpr bool contains_consonant(InputIterator first, InputIterator last)
        {
            return std::any_of(first, last, [&](auto& val){return is_consonant(val);});
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

        constexpr std::string_view trim_front(std::string_view s, std::string_view trim_str)
        {
            const auto start_position = s.find_first_not_of(trim_str);
            return s.substr(start_position);
        }

        constexpr std::string_view trim_back(std::string_view s, std::string_view trim_str)
        {
            const auto end_position = s.find_last_not_of(trim_str);
            return s.substr(0, end_position);
        }

        constexpr std::string_view trim(std::string_view s, std::string_view trim_str)
        {
            return trim_back(trim_front(s, trim_str), trim_str);
        }

        template<class T>
        T from_string(std::string_view str)
        {
            T value;

            if constexpr (std::is_arithmetic_v<T> && !std::is_same_v<T, bool>)
            {
                auto [p, ec] = std::from_chars(str.data(), str.data() + std::size(str), value);
                collin::throw_on_error te{"collin::strings::from_string"};
                te.ec = std::make_error_code(ec);
            }
            else
            {
                std::istringstream ss(str.data());
                ss.exceptions(std::istringstream::failbit | std::istringstream::badbit);
                ss >> value;
            }

            return value;
        }

        template<>
        std::string from_string(std::string_view str)
        {
            return std::string(str);
        }


        template<>
        std::string_view from_string(std::string_view str)
        {
            return str;
        }

        template<>
        char from_string(std::string_view str)
        {
            return str.front();
        }

        template<class T>
        std::string to_string(const T& value)
        {
            std::ostringstream ss;
            ss << value;
            return ss.str();
        }

        template<class OutputIterator, typename = std::enable_if_t<
/* requires */  type_traits::is_iterator_v<OutputIterator>
        >>
        void split(std::string_view str, std::string_view delim, OutputIterator it, std::size_t amount=-1)
        {
            std::size_t last_index {0};
            std::size_t index {str.find(delim, last_index)};

            while (index != std::string_view::npos && amount != 0)
            {
                *it = str.substr(last_index, index - last_index);
                it++;
                last_index = index + 1;
                index = str.find(delim, last_index);
                amount--;
            }

            *it = str.substr(last_index);
        }

        std::vector<std::string_view> split(std::string_view str, std::string_view delim, std::size_t amount=-1)
        {
            std::vector<std::string_view> results;
            split(str, delim, std::back_inserter(results), amount);
            return results;
        }

        template<class InputIterator>
        std::string join(InputIterator begin, InputIterator end, std::string_view join_string)
        {
            std::stringstream ss;
            while(begin != end)
            {
                ss << *begin;
                if(begin + 1 != end)
                {
                    ss << join_string;
                }

                begin++;
            }

            return ss.str();
        }

        template<class Container>
        std::string join(const Container& container, std::string_view join_string)
        {
            return join(std::cbegin(container), std::cend(container), join_string);
        }

        template<class CharT>
        constexpr int hex_char_to_number(CharT ch) noexcept
        {
            const auto uppered = std::toupper(ch, std::locale{});
            return uppered >= 'A' ? uppered - 'A' + 10 : uppered - '0';
        }
    }
}