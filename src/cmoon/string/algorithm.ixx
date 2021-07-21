export module cmoon.string.algorithm;

import <iterator>;
import <locale>;
import <ranges>;
import <string>;
import <string_view>;
import <functional>;
import <algorithm>;

import cmoon.string.to_string;

namespace cmoon
{
	export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_punctuation(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::ispunct(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_punctuation(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::ispunct(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    auto erase_punctuation(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_punctuation(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_spaces(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isspace(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_spaces(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isspace(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    auto erase_spaces(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_spaces(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_alphanumerics(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isalnum(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_alphanumerics(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isalnum(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    auto erase_alphanumerics(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_alphanumerics(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_alphas(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch){ return std::isalpha(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_alphas(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch){ return std::isalpha(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    auto erase_alphas(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_alphas(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_digits(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isdigit(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_digits(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isdigit(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_digits(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_digits(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_blanks(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isblank(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_blanks(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isblank(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_blanks(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_blanks(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_cntrls(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::iscntrl(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_cntrls(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::iscntrl(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_cntrls(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_cntrls(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_graphicals(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isgraph(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_graphicals(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isgraph(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_graphicals(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_graphicals(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_printables(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isprint(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_printables(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isprint(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_printables(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_printables(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_hexdigits(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isxdigit(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_hexdigits(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isxdigit(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_hexdigits(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_hexdigits(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_upper(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::isupper(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_upper(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::isupper(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_upper(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_upper(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity>
    auto remove_lower(I first, S last, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [&loc](const auto ch) { return std::islower(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class Proj = std::identity>
    auto remove_lower(Range&& r, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [&loc](const auto ch) { return std::islower(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class Proj = std::identity>
    void erase_lower(Container& container, const std::locale& loc = {}, Proj proj = {})
    {
        auto it = remove_lower(container, loc, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class CharT, class Proj = std::identity>
    constexpr auto remove_character(I first, S last, CharT c, Proj proj = {})
    {
        return std::ranges::remove_if(first, last, [c](const auto ch) { return ch == c; }, std::ref(proj));
    }

    export
    template<std::ranges::input_range Range, class CharT, class Proj = std::identity>
    constexpr auto remove_character(Range&& r, CharT c, Proj proj = {})
    {
        return std::ranges::remove_if(std::forward<Range>(r), [c](const auto ch) { return ch == c; }, std::ref(proj));
    }

    export
    template<std::ranges::range Container, class CharT, class Proj = std::identity>
    constexpr void erase_character(Container& container, CharT c, Proj proj = {})
    {
        auto it = remove_character(container, c, std::ref(proj));
        auto r = std::ranges::distance(it, std::ranges::end(container));
        container.erase(it, std::ranges::end(container));
        return r;
    }

    export
    template<std::input_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O, class Proj = std::identity>
    auto uppercase(I first, S last, O first2, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(first, last, first2, [&loc](const auto ch) { return std::toupper(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range R, std::ranges::output_range<std::ranges::range_value_t<R>> O, class Proj = std::identity>
    auto uppercase(R&& r, O out, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(std::forward<R>(r), out, [&loc](const auto ch) { return std::toupper(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range R, class Proj = std::identity>
        requires(std::ranges::output_range<R, std::ranges::range_value_t<R>>)
    void uppercase(R& r, const std::locale& loc = {}, Proj proj = {})
    {
        uppercase(r, std::ranges::begin(r), loc, std::ref(proj));
    }

    export
        template<std::input_iterator I, std::sentinel_for<I> S, std::output_iterator<std::iter_value_t<I>> O, class Proj = std::identity>
    auto lowercase(I first, S last, O first2, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(first, last, first2, [&loc](const auto ch) { return std::tolower(ch, loc); }, std::ref(proj));
    }

    export
    template<std::ranges::input_range R, class O, class Proj = std::identity>
    auto lowercase(R&& r, O out, const std::locale& loc = {}, Proj proj = {})
    {
        return std::ranges::transform(std::forward<R>(r), out, [&loc](const auto ch) { return std::tolower(ch, loc); }, std::ref(proj));
    }

    export
        template<std::ranges::input_range R, class Proj = std::identity>
    requires(std::ranges::output_range<R, std::ranges::range_value_t<R>>)
    void lowercase(R& r, const std::locale& loc = {}, Proj proj = {})
    {
        lowercase(r, std::ranges::begin(r), loc, std::ref(proj));
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
    template<class CharT, class Traits, std::output_iterator<std::basic_string_view<CharT, Traits>> O>
    constexpr void split(std::basic_string_view<CharT, Traits> str, std::basic_string_view<CharT, Traits> delim, O it, std::size_t amount=-1) noexcept
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

    template<class CharT>
    struct to_string_proj;

    template<>
    struct to_string_proj<char>
    {
        template<class T>
        auto operator()(const T& t)
        {
            return cmoon::to_string(t);
        }
    };

    template<>
    struct to_string_proj<wchar_t>
    {
        template<class T>
        auto operator()(const T& t)
        {
            return cmoon::to_wstring(t);
        }
    };

    export
    template<std::input_iterator I, std::sentinel_for<I> S, class CharT, class Traits, class Proj = to_string_proj<CharT>>
    std::basic_string<CharT, Traits> join(I begin, S end, std::basic_string_view<CharT, Traits> join_string, Proj proj)
    {
        std::basic_string<CharT, Traits> result;
        while(begin != end)
        {
            result += proj(*begin);
            if(begin + 1 != end)
            {
                result += join_string;
            }

            ++begin;
        }

        return result;
    }

    export
    template<std::ranges::input_range Range, class CharT, class Traits, class Proj = to_string_proj<CharT>>
    std::basic_string<CharT, Traits> join(Range&& r, std::basic_string_view<CharT, Traits> join_string, Proj proj)
    {
        return join(std::ranges::begin(r), std::ranges::end(r), join_string, std::ref(proj));
    }
}