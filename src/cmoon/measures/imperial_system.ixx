module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.imperial_system;

import <string_view>;

import cmoon.string;

namespace cmoon::measures
{
    export
    struct imperial_system
    {
        template<class T, class CharT>
        struct suffix
        {
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS(""))};
        };
    };
}