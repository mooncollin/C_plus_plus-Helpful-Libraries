#ifndef COLLIN_STRING
#define COLLIN_STRING

#include <unordered_set>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <string>
#include <cctype>
#include "collin_utility.hpp"
#include "collin_algorithm.hpp"
#include "collin_functional.hpp"

namespace collin
{
	bool is_vowel(char t) noexcept
    {
	    const std::unordered_set vowels = {'a', 'e', 'i', 'o', 'u'};

        return find(vowels, std::tolower(t)) != std::cend(vowels);
    }

    bool is_consonant(char t) noexcept
    {
        const std::unordered_set consonants = {'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k',
            'l', 'm', 'n', 'p', 'q', 'r', 's', 'r', 'v', 'w', 'x', 'y', 'z'};

        return find(consonants, std::tolower(t)) != std::cend(consonants);
    }

    bool contains_vowel(const std::string& str) noexcept
    {
	    return any_of(str, is_vowel);
    }

    bool contains_consonant(const std::string& str) noexcept
    {
	    return any_of(str, is_consonant);
    }

    bool ends_with_double_consonant(const std::string& str) noexcept
    {
        if(std::size(str) < 2)
        {
            return false;
        }

        const auto last_char = last_of(str);
        const auto second_last_char = last_of(str, 2);

        return last_char == second_last_char
            && is_consonant(last_char)
            && is_consonant(second_last_char);
    }

    void remove_punctuation(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::ispunct)), std::end(str));
    }

    void remove_spaces(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isspace)), std::end(str));
    }

    void remove_alphanumerics(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isalnum)), std::end(str));
    }

    void remove_blanks(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isblank)), std::end(str));
    }

    void remove_cntrls(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::iscntrl)), std::end(str));
    }

    void remove_graphicals(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isgraph)), std::end(str));
    }

    void remove_printables(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isprint)), std::end(str));
    }

    void remove_hexdigits(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isxdigit)), std::end(str));
    }

    void remove_upper(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::isupper)), std::end(str));
    }

    void remove_lower(std::string& str)
    {
        str.erase(remove_if(str, static_cast<int(*)(int)>(std::islower)), std::end(str));
    }

    void remove_character(std::string& str, char c)
    {
        str.erase(remove_if(str, equal_bind(c)), std::end(str));
    }

    void uppercase(std::string& str)
    {
        transform(str, static_cast<int(*)(int)>(std::toupper));
    }

    void lowercase(std::string& str)
    {
        transform(str, static_cast<int(*)(int)>(std::tolower));
    }

    template<class T, class Function>
    std::vector<T> split_impl(const std::string& str, const std::string& s, Function f)
    {
        std::vector<T> tokens;
        std::regex split_regex(s);
        std::sregex_token_iterator it(std::cbegin(str), std::cend(str), split_regex, -1);
        std::sregex_token_iterator end;

        while(it != end)
        {
            tokens.push_back(f(*it));
            it++;
        }

        return tokens;
    }

    template<class Function>
    std::vector<std::string> split_impl(const std::string& str, const std::string& s, std::size_t n, Function f)
    {
        std::vector<std::string> tokens;
        std::regex split_regex(s);
        std::sregex_token_iterator it(std::cbegin(str), std::cend(str), split_regex, -1);
        std::sregex_token_iterator end;

        while(it != end && n != 0)
        {
            tokens.push_back(f(*it));
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
            tokens.push_back(std::move(rest));
        }

        return tokens;
    }

    template<class T, class Function>
    std::vector<T> split(const std::string& str, const std::string& s, Function fn)
    {
        return split_impl<T>(str, s, fn);
    }

    template<class T = std::string>
    std::vector<T> split(const std::string& str, const std::string& s, std::size_t n=-1)
    {
        if constexpr(std::is_same<T, std::string>::value)
        {
            return split_impl(str, s, n, [](const auto& in) { return in; });
        }
        else if constexpr(std::is_same<T, int>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stoi(in); });
        }
        else if constexpr(std::is_same<T, long>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stol(in); });
        }
        else if constexpr(std::is_same<T, unsigned long>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stoul(in); });
        }
        else if constexpr(std::is_same<T, long int>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::strtol(in); });
        }
        else if constexpr(std::is_same<T, float>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stof(in); });
        }
        else if constexpr(std::is_same<T, double>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stod(in); });
        }
        else if constexpr(std::is_same<T, long double>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stold(in); });
        }
        else if constexpr(std::is_same<T, long long>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stoll(in); });
        }
        else if constexpr(std::is_same<T, unsigned long long>::value)
        {
            return split<T>(str, s, [](const auto& in) { return std::stoull(in); });
        }
    }
}

#endif