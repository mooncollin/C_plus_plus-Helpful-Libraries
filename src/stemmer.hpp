#ifndef COLLIN_STEMMER
#define COLLIN_STEMMER

#include <unordered_map>
#include <string>
#include <array>
#include <cctype>
#include <algorithm>
#include <functional>
#include <iostream>
#include <string_view>
#include "collin_string.hpp"
#include "collin_algorithm.hpp"
#include "collin_utility.hpp"

namespace collin
{
    class Stemmer
    {
        using stem_transform = std::function<bool(std::string&)>;

        public:
            Stemmer()
                : stemming_transforms_1a(get_stemming_transforms_1a()),
                stemming_transforms_1b(get_stemming_transforms_1b()),
                stemming_transforms_1b_supplemental(get_stemming_transforms_1b_supplemental()),
                stemming_transforms_1c(get_stemming_transforms_1c()),
                stemming_transforms_2(get_stemming_transforms_2()),
                stemming_transforms_3(get_stemming_transforms_3()),
                stemming_transforms_4(get_stemming_transforms_4()),
                stemming_transforms_5a(get_stemming_transforms_5a()),
                stemming_transforms_5b(get_stemming_transforms_5b())
            {
            }

            void stem_word(std::string& word) noexcept
            {
                const auto location = stemmed_cache.find(word);
                if(location != stemmed_cache.end())
                {
                    word = location->second;
                    return;
                }

                auto copy = word;

                run_through_transforms(stemming_transforms_1a, word);
                
                if(run_through_transforms(stemming_transforms_1b, word))
                {
                    run_through_transforms(stemming_transforms_1b_supplemental, word);
                }

                run_through_transforms(stemming_transforms_1c, word);
                run_through_transforms(stemming_transforms_2, word);
                run_through_transforms(stemming_transforms_3, word);
                run_through_transforms(stemming_transforms_4, word);
                run_through_transforms(stemming_transforms_5a, word);
                run_through_transforms(stemming_transforms_5b, word);

                stemmed_cache[copy] = word;
            }

            static int get_measure(std::string_view word) noexcept
            {
                // State 0 is at consonants, looking for vowels
                // to go to the next state.

                // State 1 is at vowels, looking for consonants
                // to add to measure and go back to state 0.
                constexpr auto consonant_state = 0;
                constexpr auto vowel_state = 1;

                int measure = 0;

                auto state = consonant_state;

                for(std::size_t i = 0; i < word.length(); i++)
                {
                    const auto current_char = word[i];

                    switch(state)
                    {
                        case consonant_state:
                            if(is_vowel(current_char)
                                    || (current_char == 'y' && i != 0 && is_consonant(word[i-1])))
                            {
                                state = vowel_state;
                            }
                            break;
                        case vowel_state:
                            if(is_consonant(current_char)
                                    && !(current_char == 'y' && i != 0 && is_consonant(word[i-1])))
                            {
                                measure++;
                                state = consonant_state;
                            }
                            break;
                    }
                }

                return measure;
            }

        private:
            std::unordered_map<std::string, std::string> stemmed_cache;
            const std::vector<stem_transform> stemming_transforms_1a;
            const std::vector<stem_transform> stemming_transforms_1b;
            const std::vector<stem_transform> stemming_transforms_1b_supplemental;
            const std::vector<stem_transform> stemming_transforms_1c;
            const std::vector<stem_transform> stemming_transforms_2;
            const std::vector<stem_transform> stemming_transforms_3;
            const std::vector<stem_transform> stemming_transforms_4;
            const std::vector<stem_transform> stemming_transforms_5a;
            const std::vector<stem_transform> stemming_transforms_5b;

            bool run_through_transforms(const std::vector<stem_transform>& transforms, std::string& word) const
            {
                return any_of(transforms, [&](const auto& trans) {
                    return trans(word);
                });
            }

            static bool ends_with_double_consonant(std::string_view str)
            {
                if(str.length() < 2)
                {
                    return false;
                }

                const auto first_char = last_of(str);
                const auto second_char = last_of(str, 2);

                return is_consonant(first_char) && is_consonant(second_char);
            }

            static stem_transform ends_with_transform(std::string ends, std::string replace) noexcept
            {
                return [=](std::string& str)
                {
                    if(std::size(ends) > std::size(str))
                    {
                        return false;
                    }

                    const auto location = str.rfind(ends);
                    if(location + std::size(ends) == std::size(str))
                    {
                        str.erase(location, std::size(ends));
                        str += replace;
                        return true;
                    }

                    return false;
                };
            }

            static stem_transform measure_ends(std::string ends, std::string replace, int measure) noexcept
            {
                return [=](std::string& str)
                {
                    const auto t = Stemmer::ends_with_transform(ends, replace);
                    auto copy = str;
                    const bool result = t(copy);
                    if(result && Stemmer::get_measure(copy) > measure)
                    {
                        str = std::move(copy);
                        return true;
                    } 

                    return false;
                };
            } 

            static stem_transform vowel_ends(std::string ends, std::string replace) noexcept
            {
                return [=](std::string& str)
                {
                    const auto t = Stemmer::ends_with_transform(ends, replace);
                    auto copy = str;
                    const bool result = t(copy);
                    if(result && contains_vowel(copy))
                    {
                        str = std::move(copy);
                        return true;
                    }

                    return false;
                };
            }

            static stem_transform cvc_ends(std::string ends, std::string replace) noexcept
            {
                return [=](std::string& str)
                {
                    if(std::size(str) < 3)
                    {
                        return false;
                    }

                    const auto first_char = last_of(str);
                    const auto second_char = last_of(str, 2);
                    const auto third_char = last_of(str, 3);

                    if(Stemmer::get_measure(str) == 1 &&
                            is_consonant(first_char) &&
                            is_vowel(second_char) &&
                            is_consonant(third_char) &&
                            third_char != 'w' &&
                            third_char != 'x' && 
                            third_char != 'y')
                    {
                        const auto t = Stemmer::ends_with_transform(ends, replace);
                        t(str);
                        return true;
                    }

                    return false;

                };
            }

            static std::vector<stem_transform> get_stemming_transforms_1a() noexcept
            {
                std::vector<stem_transform> transforms;
                transforms.push_back(ends_with_transform("sses", "ss"));
                transforms.push_back(ends_with_transform("ies", "i"));
                transforms.push_back(ends_with_transform("ss", "ss"));
                transforms.push_back(ends_with_transform("s", ""));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_1b() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back(Stemmer::measure_ends("eed", "ee", 0));
                transforms.push_back(Stemmer::vowel_ends("ed", ""));
                transforms.push_back(Stemmer::vowel_ends("ing", ""));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_1b_supplemental() noexcept
            {
                std::vector<stem_transform> transforms;
                transforms.push_back(Stemmer::ends_with_transform("at", "ate"));
                transforms.push_back(Stemmer::ends_with_transform("bl", "ble"));
                transforms.push_back(Stemmer::ends_with_transform("iz", "ize"));
                transforms.push_back([](std::string& str)
                    {
                        if(std::size(str) < 2)
                        {
                            return false;
                        }

                        const auto last_char = last_of(str);

                        if(last_char == 'l' || last_char == 's' || last_char == 'z')
                        {
                            return false;
                        }

                        if(ends_with_double_consonant(str))
                        {
                            str.erase(std::size(str) - 1, 1);
                            return true;
                        }

                        return false;
                    });
                transforms.push_back(Stemmer::cvc_ends("", "e"));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_1c() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back(Stemmer::vowel_ends("y", "i"));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_2() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back(Stemmer::measure_ends("ational", "ate", 0));
                transforms.push_back(Stemmer::measure_ends("tional", "tion", 0));
                transforms.push_back(Stemmer::measure_ends("enci", "ence", 0));
                transforms.push_back(Stemmer::measure_ends("anci", "ance", 0));
                transforms.push_back(Stemmer::measure_ends("izer", "ize", 0));
                transforms.push_back(Stemmer::measure_ends("abli", "able", 0));
                transforms.push_back(Stemmer::measure_ends("alli", "al", 0));
                transforms.push_back(Stemmer::measure_ends("entli", "ent", 0));
                transforms.push_back(Stemmer::measure_ends("eli", "e", 0));
                transforms.push_back(Stemmer::measure_ends("ousli", "ous", 0));
                transforms.push_back(Stemmer::measure_ends("ization", "ize", 0));
                transforms.push_back(Stemmer::measure_ends("ation", "ate", 0));
                transforms.push_back(Stemmer::measure_ends("ator", "ate", 0));
                transforms.push_back(Stemmer::measure_ends("alism", "al", 0));
                transforms.push_back(Stemmer::measure_ends("iveness", "ive", 0));
                transforms.push_back(Stemmer::measure_ends("fulness", "ful", 0));
                transforms.push_back(Stemmer::measure_ends("ousness", "ous", 0));
                transforms.push_back(Stemmer::measure_ends("aliti", "al", 0));
                transforms.push_back(Stemmer::measure_ends("iviti", "ive", 0));
                transforms.push_back(Stemmer::measure_ends("biliti", "ble", 0));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_3() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back(Stemmer::measure_ends("icate", "ic", 0));
                transforms.push_back(Stemmer::measure_ends("ative", "", 0));
                transforms.push_back(Stemmer::measure_ends("alize", "al", 0));
                transforms.push_back(Stemmer::measure_ends("iciti", "ic", 0));
                transforms.push_back(Stemmer::measure_ends("ical", "ic", 0));
                transforms.push_back(Stemmer::measure_ends("ful", "", 0));
                transforms.push_back(Stemmer::measure_ends("ness", "", 0));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_4() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back(Stemmer::measure_ends("al", "", 1));
                transforms.push_back(Stemmer::measure_ends("ance", "", 1));
                transforms.push_back(Stemmer::measure_ends("ence", "", 1));
                transforms.push_back(Stemmer::measure_ends("er", "", 1));
                transforms.push_back(Stemmer::measure_ends("ic", "", 1));
                transforms.push_back(Stemmer::measure_ends("able", "", 1));
                transforms.push_back(Stemmer::measure_ends("ible", "", 1));
                transforms.push_back(Stemmer::measure_ends("ant", "", 1));
                transforms.push_back(Stemmer::measure_ends("ement", "", 1));
                transforms.push_back(Stemmer::measure_ends("ment", "", 1));
                transforms.push_back(Stemmer::measure_ends("ent", "", 1));
                transforms.push_back([](std::string& str)
                    {
                        const auto t = Stemmer::measure_ends("ion", "", 1);
                        auto copy = str;
                        bool results = t(copy);

                        const auto last_char = last_of(copy);

                        if(results && last_char != 's' && last_char != 't')
                        {
                            str = std::move(copy);
                            return true;
                        }

                        return false;
                    });
                transforms.push_back(Stemmer::measure_ends("ou", "", 1));
                transforms.push_back(Stemmer::measure_ends("ism", "", 1));
                transforms.push_back(Stemmer::measure_ends("ate", "", 1));
                transforms.push_back(Stemmer::measure_ends("iti", "", 1));
                transforms.push_back(Stemmer::measure_ends("ous", "", 1));
                transforms.push_back(Stemmer::measure_ends("ive", "", 1));
                transforms.push_back(Stemmer::measure_ends("ize", "", 1));

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_5a() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back(Stemmer::measure_ends("e", "", 1));
                transforms.push_back([](std::string& str)
                    {
                        const auto t = Stemmer::cvc_ends("e", "");
                        auto copy = str;

                        if(!t(copy) && Stemmer::get_measure(copy) == 1)
                        {
                            const auto t2 = Stemmer::ends_with_transform("e", "");
                            if(t2(copy))
                            {
                                str = std::move(copy);
                                return true;
                            }
                        }

                        return false;
                    });

                return transforms;
            }

            static std::vector<stem_transform> get_stemming_transforms_5b() noexcept
            {
                std::vector<stem_transform> transforms;

                transforms.push_back([](std::string& str)
                {
                    if(Stemmer::get_measure(str) > 1
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

#endif
