module;

#include <iterator>
#include <locale>
#include <string_view>
#include <string>

#include "cmoon/format/forward_declarations.hpp"

export module cmoon.format.basic_format_context;
import cmoon.format.basic_format_arg;
import cmoon.format.basic_format_args;

namespace cmoon
{
	export
	template<class Out, class CharT>
		requires(std::output_iterator<Out, const CharT&>)
	class basic_format_context
	{
		public:
			using iterator = Out;
			using char_type = CharT;

			template<class T>
			using formatter_type = formatter<T, CharT>;

			basic_format_arg<basic_format_context> arg(std::size_t id) const
			{
				return args_.get(id);
			}

			iterator out()
			{
				return out_;
			}

			void advance_to(iterator it)
			{
				out_ = it;
			}

			std::locale locale()
			{
				return locale_;
			}
		private:
			basic_format_context(basic_format_args<basic_format_context>&& args, Out o, const std::locale& l = {})
				: args_{std::move(args)}, out_{o}, locale_{l} {}

			basic_format_args<basic_format_context> args_;
			Out out_;
			std::locale locale_;

			template<typename CharT, class Out>
			friend Out details::vformat_to(Out, const std::locale&, std::basic_string_view<CharT>, format_args_t<Out, CharT>&&);
	};

	export
	using format_context = basic_format_context<std::back_insert_iterator<std::string>, char>;

	export
	using wformat_context = basic_format_context<std::back_insert_iterator<std::wstring>, wchar_t>;
}