export module cmoon.csv.parse_line;

import <cstddef>;
import <iterator>;
import <string_view>;
import <string>;

import cmoon.csv.dialect;

namespace cmoon::csv
{
	export
	template<class CharT, class Traits, std::output_iterator<std::basic_string_view<CharT, Traits>> OutputIterator>
	void parse_line(std::basic_string_view<CharT, Traits> line, const basic_dialect<CharT, Traits>& dialect, OutputIterator out, std::size_t amount)
	{
		std::size_t previous_index {0};
		std::size_t current_index {0};
		auto delimiter_it {std::cbegin(dialect.delimiter)};
		auto quote_it {std::cbegin(dialect.quote)};
		bool in_quote {false};
		bool quoted {false};

		for (const auto ch : line)
		{
			if(!in_quote)
			{
				if (ch == *delimiter_it)
				{
					++delimiter_it;
					if (delimiter_it == std::cend(dialect.delimiter)) // Found an element!
					{
						*out = line.substr(previous_index, current_index - previous_index);
						++out;
						--amount;
						if (amount == 0)
						{
							return;
						}

						current_index += quoted ? std::size(dialect.quote) : 0;
						++current_index;
						previous_index = current_index;
						delimiter_it = std::cbegin(dialect.delimiter);
						quote_it = std::cbegin(dialect.quote);
						quoted = false;
						continue;
					}
				}
				else
				{
					delimiter_it = std::cbegin(dialect.delimiter);
				}
			}

			if (ch == *quote_it)
			{
				++quote_it;
				if (quote_it == std::cend(dialect.quote))
				{
					if (!in_quote)
					{
						previous_index += std::size(dialect.quote);
					}
					else
					{
						current_index -= std::size(dialect.quote);
						quoted = true;
					}
					in_quote = !in_quote;
					quote_it = std::cbegin(dialect.quote);
				}
			}
			else
			{
				quote_it = std::cbegin(dialect.quote);
			}

			++current_index;
		}

		*out = line.substr(previous_index);
	}

	export
	template<class CharT, class Traits, std::output_iterator<std::basic_string_view<CharT, Traits>> OutputIterator>
	void parse_line(std::basic_string_view<CharT, Traits> line, const basic_dialect<CharT, Traits>& dialect, OutputIterator out)
	{
		std::size_t previous_index {0};
		std::size_t current_index {0};
		auto delimiter_it {std::cbegin(dialect.delimiter)};
		auto quote_it {std::cbegin(dialect.quote)};
		bool in_quote {false};
		bool quoted {false};

		for (const auto ch : line)
		{
			if(!in_quote)
			{
				if (ch == *delimiter_it)
				{
					++delimiter_it;
					if (delimiter_it == std::cend(dialect.delimiter)) // Found an element!
					{
						*out = line.substr(previous_index, current_index - previous_index);
						++out;

						current_index += quoted ? std::size(dialect.quote) : 0;
						++current_index;
						previous_index = current_index;
						delimiter_it = std::cbegin(dialect.delimiter);
						quote_it = std::cbegin(dialect.quote);
						quoted = false;
						continue;
					}
				}
				else
				{
					delimiter_it = std::cbegin(dialect.delimiter);
				}
			}

			if (ch == *quote_it)
			{
				++quote_it;
				if (quote_it == std::cend(dialect.quote))
				{
					if (!in_quote)
					{
						previous_index += std::size(dialect.quote);
					}
					else
					{
						current_index -= std::size(dialect.quote);
						quoted = true;
					}
					in_quote = !in_quote;
					quote_it = std::cbegin(dialect.quote);
				}
			}
			else
			{
				quote_it = std::cbegin(dialect.quote);
			}

			++current_index;
		}

		*out = line.substr(previous_index);
	}
}