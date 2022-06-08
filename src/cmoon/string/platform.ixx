export module cmoon.string.platform;

import <string_view>;

import cmoon.platform;

namespace cmoon
{
	export
    inline constexpr std::string_view line_terminator = cmoon::win32_api ? "\r\n"
                                                                         : "\n";

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