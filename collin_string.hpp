#ifndef COLLIN_STRING
#define COLLIN_STRING

#include <unordered_set>
#include <algorithm>
#include "collin_utility.hpp"
#include "collin_algorithm.hpp"

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
}

#endif