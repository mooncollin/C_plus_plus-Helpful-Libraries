#pragma once

#include <iterator>
#include <locale>
#include <string_view>
#include <string>

namespace cmoon
{
	template<class... Ts>
	struct overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;

	template<class Out, class CharT>
		requires(std::output_iterator<Out, const CharT&>)
	class basic_format_context;

	template<class T, class CharT = char>
	struct formatter;

	template<class Context>
	class basic_format_args;

	template<class Out, class CharT>
	using format_args_t = basic_format_args<basic_format_context<Out, CharT>>;

	namespace details
	{
		template<typename CharT, class Out>
		Out vformat_to(Out, const std::locale&, std::basic_string_view<CharT>, format_args_t<Out, CharT>&&);
	}

	template<typename T, class Context>
	concept context_has_formatter_type = 
		requires()
	{
		Context::template formatter_type<T>;
	} && requires(typename Context::template formatter_type<T> formatter, const T& t, Context& ctx)
	{
		formatter.format(t, ctx);
	};

	template<class Context, class... Args>
	struct format_arg_store;

	template<std::output_iterator<char> Out, class... Args>
	Out format_to(Out out, std::string_view fmt, const Args&... args);

	template<std::output_iterator<wchar_t> Out, class... Args>
	Out format_to(Out out, std::wstring_view fmt, const Args&... args);

	template<class CharT>
	class basic_format_parse_context;

	template<class Context>
	class basic_format_arg;
}