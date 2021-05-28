export module cmoon.iostream.print;

import <format>;
import <cstdio>;
import <cwchar>;
import <ostream>;
import <string_view>;

namespace cmoon
{
	export
	inline std::size_t fprint(std::FILE* file, std::string_view output_str) noexcept
	{
		return std::fwrite(output_str.data(), sizeof(char), std::size(output_str), file);
	}

	export
	inline std::size_t fprint(std::FILE* file, std::wstring_view output_str) noexcept
	{
		return std::fwrite(output_str.data(), sizeof(wchar_t), std::size(output_str), file);
	}

	export
	template<class... Args>
	inline std::size_t fprint(std::FILE* file, std::string_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		return fprint(file, output_str);
	}

	export
	template<class... Args>
	inline std::size_t fprint(std::FILE* file, std::wstring_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		return fprint(file, output_str);
	}

	export
	template<class... Args>
	std::size_t print(std::string_view fmt, const Args&... args)
	{
		return fprint(stdout, fmt, args...);
	}

	export
	template<class... Args>
	std::size_t print(std::wstring_view fmt, const Args&... args)
	{
		return fprint(stdout, fmt, args...);
	}

	export
	std::size_t print(std::string_view fmt) noexcept
	{
		return fprint(stdout, fmt);
	}

	export
	std::size_t print(std::wstring_view fmt) noexcept
	{
		return fprint(stdout, fmt);
	}

	export
	auto fprint(std::FILE* file, char ch) noexcept
	{
		return std::fputc(ch, file);
	}

	export
	auto fprint(std::FILE* file, wchar_t ch) noexcept
	{
		return std::fputwc(ch, file);
	}

	export
	auto print(char ch) noexcept
	{
		return fprint(stdout, ch);
	}

	export
	auto print(wchar_t ch) noexcept
	{
		return fprint(stdout, ch);
	}

	export
	template<class... Args>
	std::ostream& fprint(std::ostream& os, std::string_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		return os.write(output_str.data(), std::size(output_str));
	}

	export
	template<class... Args>
	std::wostream& fprint(std::wostream& os, std::wstring_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		return os.write(output_str.data(), std::size(output_str));
	}

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& fprint(std::basic_ostream<CharT, Traits>& os, CharT ch)
	{
		return os.put(ch);
	}
}