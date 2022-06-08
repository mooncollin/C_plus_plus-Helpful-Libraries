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
	template<class CharT, class Traits>
	class basic_csv_writer
	{
		public:
			basic_csv_writer(std::basic_ostream<CharT, Traits>& os, basic_dialect<CharT, Traits> dialect={}) noexcept
				: os{os}, dialect_{std::move(dialect)} {}

			[[nodiscard]] const basic_dialect<CharT, Traits>& dialect() const noexcept
			{
				return dialect_;
			}

			template<std::ranges::input_range R>
			void write_row(R&& r)
			{
				write_row(std::ranges::begin(r), std::ranges::end(r));
			}

			template<std::input_iterator I, std::sentinel_for<I> S>
			void write_row(I begin, S end)
			{
				const auto write_line_terminator {begin != end};
				while (begin != end)
				{
					write_element(*begin);
					++begin;
					if (begin != end)
					{
						os.get() << dialect_.delimiter;
					}
				}

				if (write_line_terminator)
				{
					os.get() << dialect_.line_terminator;
				}
			}

			template<class... Args>
			void write_row(const Args&... args)
			{
				write_variadic_elements(args...);
				os.get() << dialect_.line_terminator;
			}
		private:
			std::reference_wrapper<std::basic_ostream<CharT, Traits>> os;
			basic_dialect<CharT, Traits> dialect_;

			template<class Arg>
			void write_variadic_elements(const Arg& arg)
			{
				write_element(arg);
			}

			template<class Arg, class... Args>
			void write_variadic_elements(const Arg& arg, const Args&... args)
			{
				write_element(arg);
				os.get() << dialect_.delimiter;
				write_variadic_elements(args...);
			}

			template<class T>
			void write_element(const T& element)
			{
				switch (dialect_.quoting)
				{
					case quoting_option::QUOTE_MINIMAL:
						{
							const auto str {cmoon::to_string(element)};
							const auto needs_quotes {str.contains(dialect_.delimiter)};
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
					case quoting_option::QUOTE_NONNUMERIC:
						if constexpr (std::is_arithmetic_v<T>)
						{
							os.get() << element;
							break;
						}
					case quoting_option::QUOTE_ALL:
						os.get() << dialect_.quote;
						os.get() << element;
						os.get() << dialect_.quote;
				}
			}
	};

	export
	using csv_writer = basic_csv_writer<char, std::char_traits<char>>;

	export
	using wcsv_writer = basic_csv_writer<wchar_t, std::char_traits<wchar_t>>;
}