module;

#include "cmoon/string/literals.hpp"

export module cmoon.string.platform;

import <string_view>;

import cmoon.platform;
import cmoon.string.literals;

namespace cmoon
{
	export
    template<class CharT = char>
    inline constexpr std::basic_string_view<CharT> line_terminator = cmoon::win32_api ? choose_str_literal<CharT>(STR_LITERALS("\r\n"))
                                                                                      : choose_str_literal<CharT>(STR_LITERALS("\n"));

    export
	inline constexpr bool win_unicode =
	cmoon::win32_api ? (
	#ifdef UNICODE
		true
	#else
		false
	#endif
	) : false
	;
}