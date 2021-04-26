module;

#include "cmoon/string/literals.hpp";

export module cmoon.measures.metric_system;

import <ratio>;
import <string_view>;

import cmoon.string;

namespace cmoon::measures
{
	struct metric_system
    {
        template<class T, class CharT>
        struct suffix
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS(""))};
        };

        template<class CharT>
        struct suffix<std::atto, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("a"))};
        };

        template<class CharT>
        struct suffix<std::femto, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("f"))};
        };

        template<class CharT>
        struct suffix<std::pico, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("p"))};
        };

        template<class CharT>
        struct suffix<std::nano, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("n"))};
        };

        template<class CharT>
        struct suffix<std::micro, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("u"))};
        };

        template<class CharT>
        struct suffix<std::milli, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("m"))};
        };

        template<class CharT>
        struct suffix<std::centi, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("c"))};
        };

        template<class CharT>
        struct suffix<std::deci, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("d"))};
        };

        template<class CharT>
        struct suffix<std::deca, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("da"))};
        };

        template<class CharT>
        struct suffix<std::hecto, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("h"))};
        };

        template<class CharT>
        struct suffix<std::kilo, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("k"))};
        };

        template<class CharT>
        struct suffix<std::mega, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("M"))};
        };

        template<class CharT>
        struct suffix<std::giga, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("G"))};
        };

        template<class CharT>
        struct suffix<std::tera, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("T"))};
        };

        template<class CharT>
        struct suffix<std::peta, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("P"))};
        };

        template<class CharT>
        struct suffix<std::exa, CharT>
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("E"))};
        };

        template<class T, class CharT>
        static constexpr auto suffix_v = suffix<T, CharT>::value;
    };
}