export module cmoon.format.base_formatter;
import cmoon.format.basic_format_parse_context;
import cmoon.format.format_parser;

namespace cmoon
{
	export
	template<class T, class CharT>
	struct base_formatter
	{
		protected:
			format_parser<T, CharT> parser;
		public:
			constexpr typename basic_format_parse_context<CharT>::iterator parse(basic_format_parse_context<CharT>& ctx)
			{
				parser = {};
				parser.parse(ctx);
				return ctx.begin();
			}
	};
}