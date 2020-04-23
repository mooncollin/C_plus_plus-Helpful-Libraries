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

namespace collin
{
    namespace strings
    {
        const std::unordered_set vowels = {'a', 'e', 'i', 'o', 'u'};
        const std::unordered_set consonants = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k',
                'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};

        template<class charT>
        bool is_vowel(charT t, const std::locale& loc = {})
        {
            return vowels.find(std::tolower(t, loc)) != std::cend(vowels);
        }

        template<class charT>
        bool is_consonant(charT t, const std::locale& loc = {})
        {
            return consonants.find(std::tolower(t, loc)) != std::cend(consonants);
        }

        template<class InputIterator>
        bool contains_vowel(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::any_of(first, last, [&](auto& val){return is_vowel(val, loc);});
        }

        template<class Container>
        bool contains_vowel(const Container& container, const std::locale& loc = {})
        {
            return contains_vowel(std::begin(container), std::end(container), loc);
        }

        template<class InputIterator>
        bool contains_consonant(InputIterator first, InputIterator last, const std::locale& loc = {})
        {
            return std::any_of(first, last, [&](auto& val){return is_consonant(val, loc);});
        }

        template<class Container>
        bool contains_consonant(const Container& container, const std::locale& loc = {})
        {
            return contains_consonant(std::begin(container), std::end(container), loc);
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

        template<class InputIterator, class charT>
        InputIterator remove_character(InputIterator first, InputIterator last, charT c)
        {
            return std::remove_if(first, last, [=](auto ch) {return ch == c;});
        }

        template<class Container, class charT>
        void remove_character(Container& container, charT c)
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

        void trim_front(std::string& s, std::string_view trim_str)
        {
            const auto start_position = s.find_first_not_of(trim_str);
            if (start_position != std::string::npos)
            {
                s.erase(std::begin(s), std::begin(s) + start_position);
            }
        }

        void trim_back(std::string& s, std::string_view trim_str)
        {
            const auto end_position = s.find_last_not_of(trim_str);
            if (end_position != std::string::npos)
            {
                s.erase(std::begin(s) + end_position + 1, std::end(s));
            }
        }

        void trim(std::string& s, std::string_view trim_str)
        {
            trim_front(s, trim_str);
            trim_back(s, trim_str);
        }

        template<class T>
        T from_string(std::string_view str, const std::locale& loc = {})
        {
            std::istringstream ss(str.data());
            ss.imbue(loc);
            
            T value;
            ss >> value;
            return value;
        }

        template<>
        std::string from_string(std::string_view str, const std::locale& loc)
        {
            return std::string(str);
        }

        template<class T>
        std::string to_string(const T& value)
        {
            std::ostringstream ss;
            ss << value;
            return ss.str();
        }

        template<class T, class Container, class Function>
        std::vector<T> split_impl(const Container& container, std::string_view s, Function f)
        {
            std::vector<T> tokens;
            std::regex split_regex(s.data());
            std::regex_token_iterator<typename Container::const_iterator> it(std::cbegin(container), std::cend(container), split_regex, -1);
            std::regex_token_iterator<typename Container::const_iterator> end;

            while(it != end)
            {
                tokens.push_back(f(it->str()));
                it++;
            }

            return tokens;
        }

        template<class Function>
        std::vector<std::string> split_impl(std::string_view str, std::string_view s, std::size_t n, Function f)
        {
            std::vector<std::string> tokens;
            std::regex split_regex(s.data());
            std::regex_token_iterator<decltype(str)::const_iterator> it(std::cbegin(str), std::cend(str), split_regex, -1);
            std::regex_token_iterator<decltype(str)::const_iterator> end;

            while(it != end && n != 0)
            {
                tokens.push_back(f(it->str()));
                it++;
                n--;
            }

            std::string rest;

            while(it != end)
            {
                rest += *it;
                rest += s;
                it++;
            }

            if(!rest.empty())
            {
                rest.erase(std::end(rest) - 1);
                tokens.push_back(std::move(rest));
            }

            return tokens;
        }

        template<class T, class Container, class Function>
        std::vector<T> split(const Container& container, std::string_view s, Function fn)
        {
            return split_impl<T>(container, s, fn);
        }

        template<class T = std::string, class Container>
        std::vector<T> split(const Container& container, std::string_view s=" ", std::size_t n=-1)
        {
            if constexpr(std::is_convertible_v<T, std::string>)
            {
                return split_impl(container, s, n, [](auto s){return from_string<std::string>(s);});
            }
            else
            {
                return split<T>(container, s, [](auto s) {return from_string<T>(s);});
            }
        }

        template<class InputIterator>
        std::string join(InputIterator begin, InputIterator end, std::string_view join_string)
        {
            std::ostringstream s;
            while(begin != end)
            {
                s << *begin;
                if(begin + 1 != end)
                {
                    s << join_string;
                }

                begin++;
            }

            return s.str();
        }

        template<class Container>
        std::string join(const Container& container, std::string_view join_string)
        {
            return join(std::cbegin(container), std::cend(container), join_string);
        }
    }
}