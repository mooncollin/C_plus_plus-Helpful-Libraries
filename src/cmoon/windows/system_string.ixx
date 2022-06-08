export module cmoon.windows.system_string;

import <string>;

import cmoon.string;

namespace cmoon::windows
{
	export
	using ansi_string = std::string;

	export
	using unicode_string = std::wstring;

	export
	using string_type = std::conditional_t<win_unicode, unicode_string, ansi_string>;

	export
	using string_view_type = std::conditional_t<win_unicode, std::wstring_view, std::string_view>;
}