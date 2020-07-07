#pragma once

#include <map>
#include <string>
#include <array>
#include <cctype>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string_view>

#include "collin/string.hpp"
#include "collin/algorithm.hpp"
#include "collin/utility.hpp"

namespace collin
{
    namespace stemming
    {
        class porter_stemmer
        {
            using stem_transform = std::function<bool(std::string&)>;

            public:
                void stem_word(std::string& word)
                {
                    const auto location = stemmed_cache.find(word);
                    if(location != stemmed_cache.end())
                    {
                        word = location->second;
                        return;
                    }

                    auto copy = word;

                    porter_stemmer::stemming_transforms_1a(word);
                
                    if(porter_stemmer::stemming_transforms_1b(word))
                    {
                        porter_stemmer::stemming_transforms_1b_supplemental(word);
                    }

                    porter_stemmer::stemming_transforms_1c(word);
                    porter_stemmer::stemming_transforms_2(word);
                    porter_stemmer::stemming_transforms_3(word);
                    porter_stemmer::stemming_transforms_4(word);
                    porter_stemmer::stemming_transforms_5a(word);
                    porter_stemmer::stemming_transforms_5b(word);

                    stemmed_cache[copy] = word;
                }

                static constexpr int get_measure(std::string_view word) noexcept
                {
                    // State 0 is at consonants, looking for vowels
                    // to go to the next state.

                    // State 1 is at vowels, looking for consonants
                    // to add to measure and go back to state 0.
                    enum class char_state
                    {
                        consonant,
                        vowel
                    };

                    int measure = 0;

                    auto state = char_state::consonant;

                    for(std::size_t i = 0; i < word.length(); i++)
                    {
                        const auto current_char = word[i];

                        switch(state)
                        {
                            case char_state::consonant:
                                if(strings::is_vowel(current_char)
                                        || (current_char == 'y' && i != 0 && strings::is_consonant(word[i-1])))
                                {
                                    state = char_state::vowel;
                                }
                                break;
                            case char_state::vowel:
                                if(strings::is_consonant(current_char)
                                        && !(current_char == 'y' && i != 0 && strings::is_consonant(word[i-1])))
                                {
                                    measure++;
                                    state = char_state::consonant;
                                }
                                break;
                        }
                    }

                    return measure;
                }

            private:
                std::map<std::string, std::string> stemmed_cache;

                static inline bool stemming_transforms_1a(std::string& word)
                {
                    return porter_stemmer::ends_with_transform(word, "sses", "ss") ||
                           porter_stemmer::ends_with_transform(word, "ies", "i") ||
                           porter_stemmer::ends_with_transform(word, "ss", "ss") ||
                           porter_stemmer::ends_with_transform(word, "s", "");
                }

                static inline bool stemming_transforms_1b(std::string& word)
                {
                    return porter_stemmer::measure_ends(word, "eed", "ee", 0) ||
                           porter_stemmer::vowel_ends(word, "ed", "") ||
                           porter_stemmer::vowel_ends(word, "ing", "");
                }

                static inline bool stemming_transforms_1b_supplemental(std::string& word)
                {
                    if (porter_stemmer::ends_with_transform(word, "at", "ate") ||
                        porter_stemmer::ends_with_transform(word, "bl", "ble") ||
                        porter_stemmer::ends_with_transform(word, "iz", "ize"))
                    {
                        return true;
                    }
                    
                    if (std::size(word) < 2)
                    {
                        return false;
                    }

                    const auto last_char = last_of(word);
                    if (last_char == 'l' || last_char == 's' || last_char == 'z')
                    {
                        return false;
                    }

                    if (ends_with_double_consonant(word))
                    {
                        word.erase(std::size(word) - 1, 1);
                        return true;
                    }

                    return false;
                }

                static inline bool stemming_transforms_1c(std::string& word)
                {
                    return porter_stemmer::vowel_ends(word, "y", "i");
                }

                static inline bool stemming_transforms_2(std::string& word)
                {
                    return 
                    porter_stemmer::measure_ends(word, "ational", "ate", 0) ||
                    porter_stemmer::measure_ends(word, "tional", "tion", 0) ||
                    porter_stemmer::measure_ends(word, "enci", "ence", 0) ||
                    porter_stemmer::measure_ends(word, "anci", "ance", 0) ||
                    porter_stemmer::measure_ends(word, "izer", "ize", 0) ||
                    porter_stemmer::measure_ends(word, "abli", "able", 0) ||
                    porter_stemmer::measure_ends(word, "alli", "al", 0) ||
                    porter_stemmer::measure_ends(word, "entli", "ent", 0) ||
                    porter_stemmer::measure_ends(word, "eli", "e", 0) ||
                    porter_stemmer::measure_ends(word, "ousli", "ous", 0) ||
                    porter_stemmer::measure_ends(word, "ization", "ize", 0) ||
                    porter_stemmer::measure_ends(word, "ation", "ate", 0) ||
                    porter_stemmer::measure_ends(word, "ator", "ate", 0) ||
                    porter_stemmer::measure_ends(word, "alism", "al", 0) ||
                    porter_stemmer::measure_ends(word, "iveness", "ive", 0) ||
                    porter_stemmer::measure_ends(word, "fulness", "ful", 0) ||
                    porter_stemmer::measure_ends(word, "ousness", "ous", 0) ||
                    porter_stemmer::measure_ends(word, "aliti", "al", 0) ||
                    porter_stemmer::measure_ends(word, "iviti", "ive", 0) ||
                    porter_stemmer::measure_ends(word, "biliti", "ble", 0);
                }

                static inline bool stemming_transforms_3(std::string& word)
                {
                    return
                    porter_stemmer::measure_ends(word, "icate", "ic", 0) ||
                    porter_stemmer::measure_ends(word, "ative", "", 0) ||
                    porter_stemmer::measure_ends(word, "alize", "al", 0) ||
                    porter_stemmer::measure_ends(word, "iciti", "ic", 0) ||
                    porter_stemmer::measure_ends(word, "ical", "ic", 0) ||
                    porter_stemmer::measure_ends(word, "ful", "", 0) ||
                    porter_stemmer::measure_ends(word, "ness", "", 0);
                }

                static inline bool stemming_transforms_4(std::string& word)
                {
                    if (porter_stemmer::measure_ends(word, "al", "", 1) ||
                        porter_stemmer::measure_ends(word, "ance", "", 1) ||
                        porter_stemmer::measure_ends(word, "ence", "", 1) ||
                        porter_stemmer::measure_ends(word, "er", "", 1) ||
                        porter_stemmer::measure_ends(word, "ic", "", 1) ||
                        porter_stemmer::measure_ends(word, "able", "", 1) ||
                        porter_stemmer::measure_ends(word, "ible", "", 1) ||
                        porter_stemmer::measure_ends(word, "ant", "", 1) ||
                        porter_stemmer::measure_ends(word, "ement", "", 1) ||
                        porter_stemmer::measure_ends(word, "ment", "", 1) ||
                        porter_stemmer::measure_ends(word, "ent", "", 1))
                    {
                        return true;
                    }

                    auto copy = word;
                    if (porter_stemmer::measure_ends(copy, "ion", "", 1))
                    {
                        const auto last_char = last_of(copy);
                        if (last_char != 's' && last_char != 't')
                        {
                            word = std::move(copy);
                            return true;
                        }
                    }

                    return 
                        porter_stemmer::measure_ends(word, "ou", "", 1) ||
                        porter_stemmer::measure_ends(word, "ism", "", 1) ||
                        porter_stemmer::measure_ends(word, "ate", "", 1) ||
                        porter_stemmer::measure_ends(word, "iti", "", 1) ||
                        porter_stemmer::measure_ends(word, "ous", "", 1) ||
                        porter_stemmer::measure_ends(word, "ive", "", 1) ||
                        porter_stemmer::measure_ends(word, "ize", "", 1);
                }

                static inline bool stemming_transforms_5a(std::string& word)
                {
                    if (porter_stemmer::measure_ends(word, "e", "", 1))
                    {
                        return true;
                    }

                    auto copy = word;
                    if (!porter_stemmer::cvc_ends(copy, "e", "") && porter_stemmer::get_measure(copy) == 1)
                    {
                        if (porter_stemmer::ends_with_transform(copy, "e", ""))
                        {
                            word = std::move(copy);
                            return true;
                        }
                    }

                    return false;
                }

                static inline bool stemming_transforms_5b(std::string& word)
                {
                    if (porter_stemmer::get_measure(word) > 1 &&
                        porter_stemmer::ends_with_double_consonant(word) &&
                        last_of(word) == 'l')
                    {
                        word.erase(std::size(word) - 1, 1);
                        return true;
                    }

                    return false;
                }

                static constexpr bool ends_with_double_consonant(std::string_view str)
                {
                    if(std::size(str) < 2)
                    {
                        return false;
                    }

                    const auto first_char = last_of(str);
                    const auto second_char = last_of(str, 2);

                    return strings::is_consonant(first_char) && strings::is_consonant(second_char);
                }

                static bool ends_with_transform(std::string& str, std::string_view ends, std::string_view replace)
                {
                    if (str.ends_with(ends))
                    {
                        str.erase(std::end(str) - std::size(ends));
                        return true;
                    }

                    return false;
                }

                static bool measure_ends(std::string& str, std::string_view ends, std::string_view replace, const int measure)
                {
                    auto copy = str;
                    if (porter_stemmer::ends_with_transform(copy, ends, replace) && porter_stemmer::get_measure(copy) > measure)
                    {
                        str = std::move(copy);
                        return true;
                    }

                    return false;
                }

                static bool vowel_ends(std::string& str, std::string_view ends, std::string_view replace)
                {
                    auto copy = str;
                    if (porter_stemmer::ends_with_transform(copy, ends, replace) && strings::contains_vowel(std::begin(copy), std::end(copy)))
                    {
                        str = std::move(copy);
                        return true;
                    }

                    return false;
                }

                static bool cvc_ends(std::string& str, std::string_view ends, std::string_view replace)
                {
                    if (std::size(str) < 3)
                    {
                        return false;
                    }

                    const auto first_char = last_of(str);
                    const auto second_char = last_of(str, 2);
                    const auto third_char = last_of(str, 3);

                    if (porter_stemmer::get_measure(str) == 1 &&
                        strings::is_consonant(first_char) &&
                        strings::is_vowel(second_char) &&
                        strings::is_consonant(third_char) &&
                        third_char != 'w' &&
                        third_char != 'x' &&
                        third_char != 'y')
                    {
                        return porter_stemmer::ends_with_transform(str, ends, replace);
                    }

                    return false;
                }

                static std::vector<stem_transform> get_stemming_transforms_5b() noexcept
                {
                    std::vector<stem_transform> transforms;

                    transforms.push_back([](std::string& str)
                    {
                        if(porter_stemmer::get_measure(str) > 1
                            && ends_with_double_consonant(str)
                            && last_of(str) == 'l')
                        {
                            str.erase(std::size(str) - 1, 1);
                            return true;
                        } 

                        return false;
                    });

                    return transforms;
                }
        };
    }
}
