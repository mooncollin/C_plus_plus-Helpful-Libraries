#ifndef COLLIN_STRING
#define COLLIN_STRING

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
#include <cstring>
#include "collin_utility.hpp"
#include "collin_algorithm.hpp"
#include "collin_functional.hpp"

namespace collin
{
    constexpr std::array english_alphabet =
    {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
        'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
        's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    const std::unordered_set vowels = {'a', 'e', 'i', 'o', 'u'};
    const std::unordered_set consonants = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k',
            'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z'};

	bool is_vowel(char t)
    {
        return vowels.find(std::tolower(t)) != std::cend(vowels);
    }

    bool is_consonant(char t)
    {
        return consonants.find(std::tolower(t)) != std::cend(consonants);
    }

    template<class Container>
    bool contains_vowel(const Container& container)
    {
	    return any_of(container, is_vowel);
    }

    template<class Container>
    bool contains_consonant(const Container& container)
    {
	    return any_of(container, is_consonant);
    }

    template<class Container>
    void remove_punctuation(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::ispunct));
    }

    template<class Container>
    void remove_spaces(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isspace));
    }

    template<class Container>
    void remove_alphanumerics(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isalnum));
    }

    template<class Container>
    void remove_blanks(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isblank));
    }

    template<class Container>
    void remove_cntrls(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::iscntrl));
    }

    template<class Container>
    void remove_graphicals(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isgraph));
    }

    template<class Container>
    void remove_printables(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isprint));
    }

    template<class Container>
    void remove_hexdigits(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isxdigit));
    }

    template<class Container>
    void remove_upper(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::isupper));
    }

    template<class Container>
    void remove_lower(Container& container)
    {
        erase_if(container, static_cast<int(*)(int)>(std::islower));
    }

    template<class Container>
    void remove_character(Container& container, char c)
    {
        const auto f = std::bind(std::equal_to<char>(), std::placeholders::_1, c);
        erase_if(container, f);
    }

    template<class Container>
    void uppercase(Container& container)
    {
        transform(container, static_cast<int(*)(int)>(std::toupper));
    }

    template<class Container>
    void lowercase(Container& container)
    {
        transform(container, static_cast<int(*)(int)>(std::tolower));
    }

    void trim(std::string& s, std::string_view trim_str)
    {
        const auto start_position = s.find_first_not_of(trim_str);
        if(start_position != std::string::npos)
        {
            s.erase(std::begin(s), std::begin(s) + start_position);
        }

        const auto end_position = s.find_last_not_of(trim_str);
        if(end_position != std::string::npos)
        {
            s.erase(std::begin(s) + end_position + 1, std::end(s));
        }
    }

    template<class T>
    T from_string(std::string_view str)
    {
        using rq = std::remove_cv_t<T>;

        if constexpr(std::is_same_v<char*, rq>)
        {
            auto new_str = new char[str.size() + 1];
            std::copy(str.data(), str.data() + str.size(), new_str);
            new_str[str.size()] = '\0';

            return new_str;
        }
        else if constexpr(std::is_same_v<rq, int>)
        {
            return std::atoi(str.data());
        }
        else if constexpr(std::is_same_v<rq, long int>)
        {
            return std::strtol(str.data(), nullptr, 10);
        }
        else if constexpr(std::is_same_v<rq, long long int>)
        {
            return std::strtoll(str.data(), nullptr, 10);
        }
        else if constexpr(std::is_same_v<rq, unsigned long int>)
        {
            return std::strtoul(str.data(), nullptr, 10);
        }
        else if constexpr(std::is_same_v<rq, unsigned long long int>)
        {
            return std::strtoull(str.data(), nullptr, 10);
        }
        else if constexpr(std::is_same_v<rq, float>)
        {
            return std::strtof(str.data(), nullptr);
        }
        else if constexpr(std::is_same_v<rq, double>)
        {
            return std::strtod(str.data(), nullptr);
        }
        else if constexpr(std::is_same_v<rq, long double>)
        {
            return std::strtold(str.data(), nullptr);
        }
        else
        {
            return T(str.data());
        }
    }

    template<class T>
    std::string to_string(const T& value)
    {
        using rq = std::remove_cv_t<T>;
        if constexpr(std::is_convertible_v<T, std::string>)
        {
            return std::string(value);
        }
        else
        {
            return std::to_string(value);
        }
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
            return split_impl(container, s, n, from_string<std::string>);
        }
        else
        {
            return split<T>(container, s, from_string<T>);
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

#endif