export module cmoon.string.from_string;

import <charconv>;
import <typeinfo>;
import <string_view>;
import <system_error>;
import <sstream>;
import <concepts>;

namespace cmoon
{
	template<class T, class CharT>
    concept from_chars_valid = std::same_as<CharT, char> &&
        requires(T& t, const char* begin, const char* end)
    {
        std::from_chars(begin, end, t);
    };

    export
    struct bad_string_conversion : std::bad_cast
    {
        const char* what() const override
        {
            return "bad cast from string";
        }
    };

    export
    template<class T, class CharT>
    T from_string(std::basic_string_view<CharT> str)
    {
        if constexpr(from_chars_valid<T, CharT>)
        {
            T value;
            const auto result = std::from_chars(str.data(), str.data() + str.size(), value);
            if (result.ec != std::errc{})
            {
                throw bad_string_conversion{};
            }
            return value;
        }
        else if constexpr (std::same_as<T, std::basic_string<CharT>>)
        {
            return std::string{str};
        }
        else if constexpr (std::same_as<T, std::basic_string_view<CharT>>)
        {
            return str;
        }
        else if constexpr (std::same_as<T, CharT>)
        {
            return str.front();
        }
        else
        {
            T value;
            std::basic_istringstream<CharT> ss{str.data()};
            if (!(ss >> value))
            {
                throw bad_string_conversion{};
            }
            return value;
        }

    }
}