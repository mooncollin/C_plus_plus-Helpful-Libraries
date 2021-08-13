export module cmoon.iostream.lines;

import <iterator>;
import <iostream>;
import <vector>;

import cmoon.iterator;

namespace cmoon
{
	export
    template<class T, class CharT = char, class Traits = std::char_traits<CharT>>
    class lines : public cmoon::istream_iterator_sep<T, CharT, Traits>
    {
        private:
            using base = cmoon::istream_iterator_sep<T, CharT, Traits>;
        public:
            lines(std::default_sentinel_t = {}) noexcept
                : base{std::default_sentinel} {}

            lines(std::basic_istream<CharT, Traits>& input)
                : base{input, "\n"} {}
    };

    export
    template<class T, class CharT, class Traits>
    lines(std::basic_istream<CharT, Traits>&) -> lines<T, CharT, Traits>;

    export
    template<class T, class CharT, class Traits>
    std::vector<T> read_lines(std::basic_istream<CharT, Traits>& input)
    {
        return std::vector<T>{lines<T>{input}, lines<T>{}};
    }

    export
    template<class T, class CharT, class Traits, class Output>
        requires(std::output_iterator<Output, T>)
    Output read_lines(std::basic_istream<CharT, Traits>& input, Output out)
    {
        return std::copy(lines<T>{input}, lines<T>{}, out);
    }
}