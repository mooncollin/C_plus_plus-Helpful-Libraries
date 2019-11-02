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

	bool is_vowel(char t) noexcept
    {
        return vowels.find(std::tolower(t)) != std::cend(vowels);
    }

    bool is_consonant(char t) noexcept
    {
        return consonants.find(std::tolower(t)) != std::cend(consonants);
    }

    template<class Container>
    bool contains_vowel(const Container& container) noexcept
    {
	    return any_of(container, is_vowel);
    }

    template<class Container>
    bool contains_consonant(const Container& container) noexcept
    {
	    return any_of(container, is_consonant);
    }

    template<class Container, class Function>
    void remove_characters(Container& container, Function func)
    {
        container.erase(remove_if(container, func), std::end(container));
    }

    template<class Container>
    void remove_punctuation(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::ispunct));
    }

    template<class Container>
    void remove_spaces(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isspace));
    }

    template<class Container>
    void remove_alphanumerics(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isalnum));
    }

    template<class Container>
    void remove_blanks(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isblank));
    }

    template<class Container>
    void remove_cntrls(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::iscntrl));
    }

    template<class Container>
    void remove_graphicals(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isgraph));
    }

    template<class Container>
    void remove_printables(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isprint));
    }

    template<class Container>
    void remove_hexdigits(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isxdigit));
    }

    template<class Container>
    void remove_upper(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::isupper));
    }

    template<class Container>
    void remove_lower(Container& container)
    {
        remove_characters(container, static_cast<int(*)(int)>(std::islower));
    }

    template<class Container>
    void remove_character(Container& container, char c)
    {
        remove_characters(container, equal_bind(c));
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

    template<class T, class Container, class Function>
    std::vector<T> split_impl(const Container& container, const std::string& s, Function f)
    {
        std::vector<T> tokens;
        std::regex split_regex(s);
        std::sregex_token_iterator it(std::cbegin(container), std::cend(container), split_regex, -1);
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
            rest.erase(std::end(rest) - 1);
            tokens.push_back(std::move(rest));
        }

        return tokens;
    }

    template<class T, class Container, class Function>
    std::vector<T> split(const Container& container, const std::string& s, Function fn)
    {
        return split_impl<T>(container, s, fn);
    }

    template<class T = std::string, class Container>
    std::vector<T> split(const Container& container, const std::string& s=" ", std::size_t n=-1)
    {
        if constexpr(std::is_same_v<T, std::string>)
        {
            return split_impl(container, s, n, [](const auto& in) { return in; });
        }
        else if constexpr(std::is_same_v<T, int>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stoi(in); });
        }
        else if constexpr(std::is_same_v<T, long>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stol(in); });
        }
        else if constexpr(std::is_same_v<T, unsigned long>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stoul(in); });
        }
        else if constexpr(std::is_same_v<T, long int>)
        {
            return split<T>(container, s, [](const auto& in) { return std::strtol(in); });
        }
        else if constexpr(std::is_same_v<T, float>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stof(in); });
        }
        else if constexpr(std::is_same_v<T, double>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stod(in); });
        }
        else if constexpr(std::is_same_v<T, long double>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stold(in); });
        }
        else if constexpr(std::is_same_v<T, long long>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stoll(in); });
        }
        else if constexpr(std::is_same_v<T, unsigned long long>)
        {
            return split<T>(container, s, [](const auto& in) { return std::stoull(in); });
        }
    }

    template<class InputIterator>
    std::string join(InputIterator begin, InputIterator end, const std::string& join_string)
    {
        std::stringstream s;
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
    std::string join(const Container& container, const std::string& join_string)
    {
        return join(std::cbegin(container), std::cend(container), join_string);
    }
}

#endif