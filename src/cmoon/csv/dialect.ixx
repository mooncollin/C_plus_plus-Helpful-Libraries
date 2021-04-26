export module cmoon.csv.dialect;
import <string_view>;
import <string>;

namespace cmoon::csv
{
	export
	struct dialect
	{
		public:
			static constexpr std::string_view default_delimiter {","};
			static constexpr std::string_view default_quote {"\""};
			static constexpr std::string_view default_escape {"\\"};
			static constexpr std::string_view default_line_terminator {"\n"};

			enum class quoting_option
			{
				QUOTE_ALL,
				QUOTE_MINIMAL,
				QUOTE_NONNUMERIC
			};

			std::string delimiter {default_delimiter};
			std::string quote {default_quote};
			std::string line_terminator {default_line_terminator};
			quoting_option quoting {quoting_option::QUOTE_MINIMAL};
	};
}