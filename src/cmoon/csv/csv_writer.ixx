export module cmoon.csv.csv_writer;

import <iostream>;
import <ranges>;
import <iterator>;
import <functional>;
import <type_traits>;
import <format>;

import cmoon.string;

import cmoon.csv.dialect;

namespace cmoon::csv
{
	export
	template<class CharT, class Traits = std::char_traits<CharT>>
	class basic_csv_writer
	{
		public:
			basic_csv_writer(std::basic_ostream<CharT, Traits>& os, const basic_dialect<CharT, Traits>& dialect={})
				: os{os}, dialect_{dialect} {}

			basic_csv_writer(std::basic_ostream<CharT, Traits>& os, basic_dialect<CharT, Traits>&& dialect)
				: os{os}, dialect_{std::move(dialect)} {}

			basic_dialect<CharT, Traits>& get_dialect()
			{
				return dialect_;
			}

			const basic_dialect<CharT, Traits>& get_dialect() const
			{
				return dialect_;
			}

			template<std::ranges::input_range Range>
			void write_row(Range&& r)
			{
				write_row(std::begin(r), std::end(r));
			}

			template<std::input_iterator InputIterator>
			void write_row(InputIterator begin, InputIterator end)
			{
				while (begin != end)
				{
					write_element(*begin);
					begin++;
					if (begin != end)
					{
						os.get() << dialect_.delimiter;
					}
				}
				os.get() << dialect_.line_terminator;
			}

			template<class... Args>
			void write_row(Args&&... args)
			{
				write_variadic_element(std::forward<Args>(args)...);
				os.get() << dialect_.line_terminator;
			}
		private:
			std::reference_wrapper<std::basic_ostream<CharT, Traits>> os;
			basic_dialect<CharT, Traits> dialect_;

			template<class Arg>
			void write_variadic_element(Arg&& arg)
			{
				write_element(std::forward<Arg>(arg));
			}

			template<class Arg, class... Args>
			void write_variadic_element(Arg&& arg, Args&&... args)
			{
				write_element(std::forward<Arg>(arg));
				os.get() << dialect_.delimiter;
				write_variadic_element(std::forward<Args>(args)...);
			}

			template<class T>
			void write_element(const T& element)
			{
				switch (dialect_.quoting)
				{
					case dialect::quoting_option::QUOTE_MINIMAL:
						{
							const auto str = cmoon::to_string(element);
							const auto needs_quotes = str.find(dialect_.delimiter) != std::string::npos;
							if (needs_quotes)
							{
								os.get() << dialect_.quote;
							}
							os.get() << str;
							if (needs_quotes)
							{
								os.get() << dialect_.quote;
							}
							break;
						}
					case dialect::quoting_option::QUOTE_NONNUMERIC:
						if constexpr (std::is_arithmetic_v<T>)
						{
							os.get() << element;
							break;
						}
					case dialect::quoting_option::QUOTE_ALL:
						os.get() << dialect_.quote;
						os.get() << element;
						os.get() << dialect_.quote;
				}
			}
	};

	export
	using csv_writer = basic_csv_writer<char>;

	export
	using wcsv_writer = basic_csv_writer<wchar_t>;
}