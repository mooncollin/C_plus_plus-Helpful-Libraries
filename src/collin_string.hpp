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
#include <charconv>
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

    void trim(std::string& s, const std::string& trim_str)
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
        return T(str.data());
    }

    // The user needs to free the memory
    template<>
    char* from_string<char*>(std::string_view str)
    {
        auto new_str = new char[str.size() + 1];
        std::copy(str.data(), str.data() + str.size(), new_str);
        new_str[str.size()] = '\0';

        return new_str;
    }

    template<>
    int from_string<int>(std::string_view str)
    {
        return std::stoi(str.data());
    }

    template<>
    long from_string<long>(std::string_view str)
    {
        return std::stol(str.data());
    }

    template<>
    unsigned long from_string<unsigned long>(std::string_view str)
    {
        return std::stoul(str.data());
    }

    template<>
    float from_string<float>(std::string_view str)
    {
        return std::stof(str.data());
    }

    template<>
    double from_string<double>(std::string_view str)
    {
        return std::stod(str.data());
    }

    template<>
    long double from_string<long double>(std::string_view str)
    {
        return std::stold(str.data());
    }

    template<>
    long long from_string<long long>(std::string_view str)
    {
        return std::stoll(str.data());
    }

    template<>
    unsigned long long from_string<unsigned long long>(std::string_view str)
    {
        return std::stoull(str.data());
    }

    template<class T>
    T from_string(const std::string& str)
    {
        return from_string<T>(std::string_view(str.data(), str.size()));
    }

    template<class T>
    std::pair<T, std::errc> from_string_fast(std::string_view str)
    {
        T result;

        auto [_, err] = std::from_chars(str.data(), str.data() + str.size(), result);

        return std::make_pair(result, err);
    }

    template<class T>
    std::string to_string(const T& value)
    {
        return std::to_string(value);
    }

    template<>
    std::string to_string<std::string>(const std::string& value)
    {
        return std::string(value);
    }

    template<class T>
    std::pair<T, std::errc> from_string_fast(const std::string& str)
    {
        return from_string_fast<T>(std::string_view(str.data(), str.size()));
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
            const auto current_str = it->str();
            tokens.push_back(f(current_str));
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
            const auto current_str = it->str();
            tokens.push_back(f(current_str));
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
        const auto from_string_f = static_cast<T(*)(const std::string& str)>(from_string);

        if constexpr(std::is_convertible_v<T, std::string>)
        {
            return split_impl(container, s, n, from_string_f);
        }
        else
        {
            return split<T>(container, s, from_string_f);
        }
    }

    template<class T = std::string, class Container>
    std::vector<T> split(const Container& container, const std::string& s, std::size_t n=-1)
    {
        return split(container, std::string_view(s.data(), s.size()), n);
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