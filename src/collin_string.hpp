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

    template<class T>
    T from_string(std::string_view str)
    {
        using plain_type = std::remove_cv_t<T>;

        if constexpr(std::is_same_v<plain_type, char*>)
        {
            return str.data();
        }
        else if constexpr(std::is_same_v<plain_type, int>)
        {
            return std::stoi(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, long>)
        {
            return std::stol(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, unsigned long>)
        {
            return std::stoul(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, long int>)
        {
            return std::atol(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, float>)
        {
            return std::stof(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, double>)
        {
            return std::stod(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, long double>)
        {
            return std::stold(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, long long>)
        {
            return std::stoll(str.data());
        }
        else if constexpr(std::is_same_v<plain_type, unsigned long long>)
        {
            return std::stoull(str.data());
        }
        else if constexpr(std::is_convertible_v<plain_type, std::string>)
        {
            return std::string(str.data());
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
            const auto current_str = it->str().c_str();
            const auto current_size = static_cast<std::size_t>(it->length());
            tokens.push_back(
                std::move(f(std::string_view(
                        current_str,
                        current_size
                    )
                )
            ));
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
            const auto current_str = it->str().c_str();
            const auto current_size = static_cast<std::size_t>(it->length());
            tokens.push_back(
                std::move(f(std::string_view(
                        current_str,
                        current_size
                    )
                )
            ));
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
            return split_impl(container, s, n, from_string<T>);
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