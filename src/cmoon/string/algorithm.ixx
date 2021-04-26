export module cmoon.string.algorithm;

import <iterator>;
import <locale>;
import <ranges>;
import <string>;
import <sstream>;
import <string_view>;

namespace cmoon
{
	export
    template<std::input_iterator InputIterator>
    auto remove_punctuation(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::ispunct(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_punctuation(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::ispunct(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_punctuation(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_punctuation(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_spaces(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isspace(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_spaces(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isspace(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_spaces(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_spaces(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_alphanumerics(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isalnum(ch, loc);});
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_alphanumerics(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isalnum(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_alphanumerics(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_alphanumerics(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_alphas(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch){ return std::isalpha(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_alphas(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch){ return std::isalpha(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_alphas(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_alphas(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_digits(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isdigit(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_digits(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isdigit(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_digits(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_digits(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_blanks(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isblank(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_blanks(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isblank(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_blanks(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_blanks(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_cntrls(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::iscntrl(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_cntrls(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::iscntrl(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_cntrls(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(remove_cntrls(container, loc, proj), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_graphicals(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isgraph(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_graphicals(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isgraph(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_graphicals(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(remove_graphicals(container, loc, proj), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_printables(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isprint(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_printables(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isprint(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_printables(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(remove_printables(container, loc, proj), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_hexdigits(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isxdigit(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_hexdigits(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isxdigit(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_hexdigits(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_hexdigits(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_upper(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::isupper(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_upper(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isupper(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_upper(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_upper(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator>
    auto remove_lower(InputIterator first, InputIterator last, const std::locale& loc = {})
    {
        return std::remove_if(first, last, [&loc](const auto ch) { return std::islower(ch, loc); });
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_lower(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::islower(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void erase_lower(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_lower(container, loc, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator, class CharT>
    constexpr auto remove_character(InputIterator first, InputIterator last, CharT c)
    {
        return std::remove_if(first, last, [c](const auto ch) { return ch == c; });
    }

    export
    template<std::ranges::input_range Range, class CharT, class Proj = std::identity>
    constexpr auto remove_character(Range&& r, CharT c, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [c](const auto ch) { return ch == c; }, proj);
    }

    export
    template<class Container, class CharT, class Proj = std::identity>
    constexpr void erase_character(Container& container, CharT c, Proj proj = {})
    {
        container.erase(std::ranges::begin(remove_character(container, c, proj)), std::end(container));
    }

    export
    template<std::input_iterator InputIterator, std::output_iterator<std::iter_value_t<InputIterator>> OutputIterator>
    auto uppercase(InputIterator first, InputIterator last, OutputIterator first2, const std::locale& loc = {})
    {
        return std::transform(first, last, first2, [&loc](const auto ch) { return std::toupper(ch, loc); });
    }

    export
    template<std::ranges::input_range Range1, class O, class Proj = std::identity>
    auto uppercase(Range1&& r1, O out, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(std::forward<Range1>(r1), out, [&loc](const auto ch) { return std::toupper(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void uppercase(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        uppercase(container, std::begin(container), loc, proj);
    }

    export
    template<std::input_iterator InputIterator, std::output_iterator<std::iter_value_t<InputIterator>> OutputIterator>
    auto lowercase(InputIterator first, InputIterator last, OutputIterator first2, const std::locale& loc = {})
    {
        return std::transform(first, last, first2, [&loc](const auto ch) { return std::tolower(ch, loc); });
    }

    export
    template<std::ranges::input_range Range1, class O, class Proj = std::identity>
    auto lowercase(Range1&& r1, O out, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(std::forward<Range1>(r1), out, [&loc](const auto ch) { return std::tolower(ch, loc); }, proj);
    }

    export
    template<class Container, class Proj = std::identity>
    void lowercase(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        lowercase(container, std::begin(container), loc, proj);
    }

    export
    template<class CharT, class Traits>
    constexpr std::basic_string_view<CharT, Traits> trim_front(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
    {
        const auto start_position = s.find_first_not_of(trim_str);
        if (start_position == std::string_view::npos)
        {
            return "";
        }

        return s.substr(start_position);
    }

    export
    template<class CharT, class Traits>
    void trim_front(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
    {
        const auto start_position = s.find_first_not_of(trim_str);
        if (start_position == std::string::npos)
        {
            s = {};
        }
        s.erase(std::begin(s), std::begin(s) + start_position);
    }

    export
    template<class CharT, class Traits>
    constexpr std::basic_string_view<CharT, Traits> trim_back(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
    {
        const auto end_position = s.find_last_not_of(trim_str);
        if (end_position == std::string_view::npos)
        {
            return "";
        }

        return s.substr(0, end_position);
    }

    export
    template<class CharT, class Traits>
    void trim_back(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
    {
        const auto end_position = s.find_last_not_of(trim_str);
        if (end_position == std::string::npos)
        {
            s = "";
        }
        s.erase(end_position);
    }

    export
    template<class CharT, class Traits>
    constexpr std::string_view trim(std::basic_string_view<CharT, Traits> s, std::basic_string_view<CharT, Traits> trim_str) noexcept
    {
        return trim_back(trim_front(s, trim_str), trim_str);
    }

    export
    template<class CharT, class Traits>
    void trim(std::basic_string<CharT, Traits>& s, std::basic_string_view<CharT, Traits> trim_str)
    {
        trim_front(s, trim_str);
        trim_back(s, trim_str);
    }

    export
    template<class OutputIterator, class CharT, class Traits>
        requires(std::output_iterator<OutputIterator, std::basic_string_view<CharT, Traits>>)
    constexpr void split(std::basic_string_view<CharT, Traits> str, std::basic_string_view<CharT, Traits> delim, OutputIterator it, std::size_t amount=-1) noexcept
    {
        std::size_t last_index {0};
        std::size_t index {str.find(delim, last_index)};

        while (index != std::basic_string_view<CharT, Traits>::npos && amount != 0)
        {
            *it = str.substr(last_index, index - last_index);
            it++;
            last_index = index + 1;
            index = str.find(delim, last_index);
            amount--;
        }

        *it = str.substr(last_index);
    }

    export
    template<class CharT, class Traits>
    void find_and_replace(std::basic_string<CharT, Traits>& input, std::basic_string_view<CharT, Traits> find, std::basic_string_view<CharT, Traits> replace)
    {
        std::size_t current_location {input.find(find)};

        while (current_location != std::basic_string<CharT, Traits>::npos)
        {
            input.replace(current_location, std::size(find), replace);
            current_location = input.find(find, current_location + 1);
        }
    }

    export
    template<class CharT, class Traits>
    std::vector<std::basic_string_view<CharT, Traits>> split(std::basic_string_view<CharT, Traits> str, std::basic_string_view<CharT, Traits> delim, std::size_t amount=-1)
    {
        std::vector<std::basic_string_view<CharT, Traits>> results;
        split(str, delim, std::back_inserter(results), amount);
        return results;
    }

    export
    template<std::input_iterator InputIterator, class CharT, class Traits>
    std::string join(InputIterator begin, InputIterator end, std::basic_string_view<CharT, Traits> join_string)
    {
        std::stringstream ss;
        while(begin != end)
        {
            ss << *begin;
            if(begin + 1 != end)
            {
                ss << join_string;
            }

            ++begin;
        }

        return ss.str();
    }

    export
    template<std::ranges::input_range Range, class CharT, class Traits>
    std::basic_string<CharT, Traits> join(Range&& r, std::basic_string_view<CharT, Traits> join_string)
    {
        return join(std::ranges::begin(r), std::ranges::end(r), join_string);
    }
}