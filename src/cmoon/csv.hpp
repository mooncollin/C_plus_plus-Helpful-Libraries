#pragma once

#include <string_view>
#include <iterator>
#include <iostream>
#include <utility>
#include <string>
#include <tuple>
#include <type_traits>
#include <array>
#include <concepts>

#include "cmoon/format.hpp"
#include "cmoon/string.hpp"
#include "cmoon/iostream.hpp"

namespace cmoon
{
	namespace csv
	{
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

		template<std::output_iterator<std::string_view> OutputIterator>
		constexpr void parse_line(std::string_view line, const dialect& dialect, OutputIterator out, std::size_t amount=-1)
		{
			std::size_t previous_index {0};
			std::size_t current_index {0};
			auto delimiter_it = std::cbegin(dialect.delimiter);
			auto quote_it = std::cbegin(dialect.quote);
			bool in_quote {false};
			bool quoted {false};

			for (auto ch : line)
			{
				if(!in_quote)
				{
					if (ch == *delimiter_it)
					{
						delimiter_it++;
						if (delimiter_it == std::cend(dialect.delimiter)) // Found an element!
						{
							*out = line.substr(previous_index, current_index - previous_index);
							out++;
							amount--;
							if (amount == 0)
							{
								return;
							}
							current_index += quoted ? std::size(dialect.quote) : 0;
							current_index++;
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
					quote_it++;
					if (quote_it == std::cend(dialect.quote))
					{
						if(!in_quote)
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

				current_index++;
			}

			*out = line.substr(previous_index);
		}

		class base_csv_reader
		{
			public:
				base_csv_reader() = default;

				base_csv_reader(std::istream& is, const dialect& dialect={})
					: is{std::addressof(is)}, dialect_{dialect} {}

				base_csv_reader(std::istream& is, dialect&& dialect)
					: is{std::addressof(is)}, dialect_{std::move(dialect)} {}

				bool operator==(const base_csv_reader& other) const
				{
					return is == other.is;
				}

				bool operator!=(const base_csv_reader& other) const
				{
					return !(*this == other);
				}

				dialect& get_dialect()
				{
					return dialect_;
				}

				const dialect& get_dialect() const
				{
					return dialect_;
				}
			protected:
				static constexpr auto end_of_input {nullptr};

				std::istream* is {end_of_input};
				dialect dialect_;
		};

		struct csv_any {};
		struct csv_ignore {};

		template<class... Args>
			requires((... && !std::is_void_v<Args>))
		class csv_reader : public base_csv_reader
		{
			static constexpr std::size_t num_types{sizeof...(Args)};
			using base = base_csv_reader;

			public:
				using difference_type = std::ptrdiff_t;
				using value_type = std::tuple<Args...>;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::input_iterator_tag;

				csv_reader() = default;

				csv_reader(std::istream& is, const dialect& dialect={})
					: base{is, dialect} {}

				csv_reader(std::istream& is, dialect&& dialect)
					: base{is, std::move(dialect)} {}

				csv_reader begin()
				{
					operator++();
					return *this;
				}

				[[nodiscard]] csv_reader end() const
				{
					return {};
				}

				const value_type& operator*() const
				{
					return current;
				}

				csv_reader& operator++()
				{
					if (cmoon::getline(*is, line, dialect_.line_terminator).eof())
					{
						is = end_of_input;
						return *this;
					}

					std::array<std::string_view, num_types> string_elements;
					parse_line(line, dialect_, std::begin(string_elements), std::size(string_elements));

					current = parse_elements(string_elements);

					return *this;
				}

				csv_reader operator++(int)
				{
					auto v {*this};
					operator++();
					return v;
				}
			private:
				value_type current;
				std::string line;

				template<class T>
				inline static auto before_from_string(std::string_view v)
				{
					if constexpr (std::is_same_v<T, csv_ignore>)
					{
						return csv_ignore{};
					}
					else
					{
						return cmoon::from_string<T>(v);
					}
				}

				template<std::size_t... I>
				static value_type parse_elements_helper(const std::array<std::string_view, num_types>& strings, std::index_sequence<I...>)
				{
					return std::make_tuple((before_from_string<std::tuple_element_t<I, value_type>>(strings[I]))...);
				}

				static value_type parse_elements(const std::array<std::string_view, num_types>& strings)
				{
					return parse_elements_helper(strings, std::make_index_sequence<num_types>{});
				}
		};

		template<>
		class csv_reader<csv_any> : public base_csv_reader
		{
			using base = base_csv_reader;

			public:
				using difference_type = std::ptrdiff_t;
				using value_type = std::vector<std::string_view>;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::input_iterator_tag;

				csv_reader() = default;

				csv_reader(std::istream& is, const dialect& dialect={})
					: base{is, dialect} {}

				csv_reader(std::istream& is, dialect&& dialect)
					: base{is, std::move(dialect)} {}

				csv_reader begin()
				{
					operator++();
					return *this;
				}

				[[nodiscard]] csv_reader end() const
				{
					return {};
				}

				const value_type& operator*() const
				{
					return current;
				}

				csv_reader& operator++()
				{
					if (cmoon::getline(*is, line, dialect_.line_terminator).eof())
					{
						is = end_of_input;
						return *this;
					}

					current.clear();
					parse_line(line, dialect_, std::back_inserter(current));

					return *this;
				}

				csv_reader operator++(int)
				{
					auto v {*this};
					operator++();
					return v;
				}
			private:
				value_type current;
				std::string line;
		};

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
}