export module cmoon.csv.csv_writer;

import <iostream>;
import <ranges>;
import <iterator>;
import <functional>;
import <type_traits>;

import cmoon.format;
import cmoon.csv.dialect;

namespace cmoon::csv
{
	export
	class csv_writer
	{
		public:
			csv_writer(std::ostream& os, const dialect& dialect={})
				: os{os}, dialect_{dialect} {}

			csv_writer(std::ostream& os, dialect&& dialect)
				: os{os}, dialect_{std::move(dialect)} {}

			dialect& get_dialect()
			{
				return dialect_;
			}

			const dialect& get_dialect() const
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
			std::reference_wrapper<std::ostream> os;
			dialect dialect_;

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
							const auto str = cmoon::format("{}", element);
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
}