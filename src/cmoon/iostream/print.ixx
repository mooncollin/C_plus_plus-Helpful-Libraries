export module cmoon.iostream.print;

import <format>;
import <cstdio>;
import <cwchar>;
import <ostream>;
import <string_view>;
import <concepts>;

namespace cmoon
{
	template<class CharT>
	[[nodiscard]] constexpr auto get_newline() noexcept
	{
		return CharT('\n');
	}

	export
	auto fprint(std::FILE* file, char ch) noexcept
	{
		return static_cast<std::size_t>(std::fputc(ch, file) == ch);
	}

	export
	auto fprint(std::FILE* file, wchar_t ch) noexcept
	{
		return static_cast<std::size_t>(std::fputwc(ch, file) == ch);
	}

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

	export
	auto fprintln(std::FILE* file, char ch) noexcept
	{
		const auto first_write = fprint(file, ch);
		return first_write + fprint(file, get_newline<char>());
	}

	export
	auto fprintln(std::FILE* file, wchar_t ch) noexcept
	{
		const auto first_write = fprint(file, ch);
		return first_write + fprint(file, get_newline<wchar_t>());
	}

	export
	inline std::size_t fprintln(std::FILE* file, std::string_view output_str) noexcept
	{
		const auto first_write = std::fwrite(output_str.data(), sizeof(char), std::size(output_str), file);
		return first_write + fprint(file, get_newline<char>());
	}

	export
	inline std::size_t fprintln(std::FILE* file, std::wstring_view output_str) noexcept
	{
		const auto first_write = std::fwrite(output_str.data(), sizeof(wchar_t), std::size(output_str), file);
		return first_write + fprint(file, get_newline<wchar_t>());
	}

	export
	template<class... Args>
	inline std::size_t fprintln(std::FILE* file, std::string_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		const auto first_write = fprint(file, output_str);
		return first_write + fprint(file, get_newline<char>());
	}

	export
	template<class... Args>
	inline std::size_t fprintln(std::FILE* file, std::wstring_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		const auto first_write = fprint(file, output_str);
		return first_write + fprint(file, get_newline<wchar_t>());
	}

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& fprintln(std::basic_ostream<CharT, Traits>& os)
	{
		return os.put(get_newline<CharT>());
	}

	export
	template<class... Args>
	std::ostream& fprintln(std::ostream& os, std::string_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		os.write(output_str.data(), std::size(output_str));
		return fprintln(os);
	}

	export
	template<class... Args>
	std::wostream& fprintln(std::wostream& os, std::wstring_view fmt, const Args&... args)
	{
		const auto output_str = std::format(fmt, args...);
		os.write(output_str.data(), std::size(output_str));
		return fprintln(os);
	}

	export
	template<class CharT, class Traits>
	std::basic_ostream<CharT, Traits>& fprintln(std::basic_ostream<CharT, Traits>& os, CharT ch)
	{
		os.put(ch);
		return fprintln(os);
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
	auto println(char ch) noexcept
	{
		return fprintln(stdout, ch);
	}

	export
	auto println(wchar_t ch) noexcept
	{
		return fprintln(stdout, ch);
	}

	export
	template<class... Args>
	std::size_t println(std::string_view fmt, const Args&... args)
	{
		return fprintln(stdout, fmt, args...);
	}

	export
	template<class... Args>
	std::size_t println(std::wstring_view fmt, const Args&... args)
	{
		return fprintln(stdout, fmt, args...);
	}

	export
	std::size_t println(std::string_view fmt) noexcept
	{
		return fprintln(stdout, fmt);
	}

	export
	std::size_t println(std::wstring_view fmt) noexcept
	{
		return fprintln(stdout, fmt);
	}
}