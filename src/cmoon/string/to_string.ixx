export module cmoon.string.to_string;

import <format>;
import <sstream>;

import cmoon.concepts;

namespace cmoon
{
	export
	template<class T>
		requires(cmoon::formattable<T> ||
				 cmoon::stream_writable<T, std::stringstream>)
	std::string to_string(const T& t)
	{
		if constexpr (cmoon::formattable<T>)
		{
			return std::format("{}", t);
		}
		else if constexpr (cmoon::stream_writable<T, std::stringstream>)
		{
			std::stringstream ss;
			ss << t;
			return ss.str();
		}
	}

	export
	template<class T>
		requires(cmoon::formattable<T, wchar_t> ||
				 cmoon::stream_writable<T, std::wstringstream>)
	std::wstring to_wstring(const T& t)
	{
		if constexpr (cmoon::formattable<T, wchar_t>)
		{
			return std::format(L"{}", t);
		}
		else if constexpr (cmoon::stream_writable<T, std::wstringstream>)
		{
			std::wstringstream ss;
			ss << t;
			return ss.str();
		}
	}
}