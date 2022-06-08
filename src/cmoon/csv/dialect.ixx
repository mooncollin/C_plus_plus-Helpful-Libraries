export module cmoon.csv.dialect;

import <string_view>;
import <string>;

import cmoon.string;

namespace cmoon::csv
{
	export
	enum class quoting_option
	{
		QUOTE_ALL,
		QUOTE_MINIMAL,
		QUOTE_NONNUMERIC
	};

	export
	template<class CharT, class Traits = std::char_traits<CharT>>
	struct basic_dialect
	{
		public:
			static constexpr std::basic_string_view<CharT, Traits> default_delimiter {","};
			static constexpr std::basic_string_view<CharT, Traits> default_quote {"\""};
			static constexpr std::basic_string_view<CharT, Traits> default_escape {"\\"};
			static constexpr std::basic_string_view<CharT, Traits> default_line_terminator {"\n"};

			std::basic_string<CharT, Traits> delimiter {default_delimiter};
			std::basic_string<CharT, Traits> quote {default_quote};
			std::basic_string<CharT, Traits> line_terminator {default_line_terminator};
			quoting_option quoting {quoting_option::QUOTE_MINIMAL};
	};

	export
	using dialect = basic_dialect<char>;

	export
	using wdialect = basic_dialect<wchar_t>;
}