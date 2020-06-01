#pragma once

#include <string_view>
#include <iterator>
#include <iostream>
#include <utility>
#include <string>
#include <tuple>
#include <type_traits>
#include <array>

#include "string.hpp"
#include "iostream.hpp"

namespace collin
{
	namespace csv
	{

		struct dialect
		{
			public:
				static constexpr std::string_view default_delimiter {","};
				static constexpr std::string_view default_quote {"\""};
				static constexpr std::string_view default_escape {"\\"};
				static constexpr std::string_view default_line_terminator {"\r\n"};

				enum class quoting_option
				{
					QUOTE_ALL,
					QUOTE_MINIMAL,
					QUOTE_NONNUMERIC
				};

				dialect() = default;
				dialect(const dialect&) = default;
				dialect(dialect&&) noexcept = default;

				dialect& operator=(const dialect&) = default;
				dialect& operator=(dialect&&) noexcept = default;

				~dialect() noexcept = default;

				std::string delimiter {default_delimiter};
				std::string quote {default_quote};
				std::string line_terminator {default_line_terminator};
				quoting_option quoting {quoting_option::QUOTE_MINIMAL};
		};

		template<class OutputIterator>
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
					: is{&is}, dialect_{dialect} {}

				base_csv_reader(std::istream& is, dialect&& dialect)
					: is{&is}, dialect_{std::move(dialect)} {}

				base_csv_reader(const base_csv_reader&) = default;
				base_csv_reader(base_csv_reader&&) noexcept = default;

				base_csv_reader& operator=(const base_csv_reader&) = default;
				base_csv_reader& operator=(base_csv_reader&&) noexcept = default;

				~base_csv_reader() noexcept = default;

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

		template<class... Args>
		class csv_reader : public base_csv_reader, public std::iterator<std::input_iterator_tag, std::tuple<Args...>>
		{
			// Probably needs to be more constrained to only those in which collin::strings::from_string
			// works on, but i'm not sure how to get that to work.
			using enable_check = std::enable_if_t<(... && (!std::is_void_v<Args> && !std::is_same_v<Args, csv_any>))>;

			static constexpr std::size_t num_types{sizeof...(Args)};

			using base_it = std::iterator<std::input_iterator_tag, std::tuple<Args...>>;
			using base = base_csv_reader;

			public:
				csv_reader() = default;

				csv_reader(std::istream& is, const dialect& dialect={})
					: base{is, dialect} {}

				csv_reader(std::istream& is, dialect&& dialect)
					: base{is, std::move(dialect)} {}

				csv_reader(const csv_reader&) = default;
				csv_reader(csv_reader&&) noexcept = default;

				csv_reader& operator=(const csv_reader&) = default;
				csv_reader& operator=(csv_reader&&) noexcept = default;

				~csv_reader() noexcept = default;

				[[nodiscard]] csv_reader begin()
				{
					operator++();
					return *this;
				}

				[[nodiscard]] csv_reader end()
				{
					return {};
				}

				const typename base_it::value_type& operator*() const
				{
					return current;
				}

				csv_reader& operator++()
				{
					if (!iostream::getline(*is, line, dialect_.line_terminator))
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
				typename base_it::value_type current;
				std::string line;

				template<std::size_t... I>
				static typename base_it::value_type parse_elements_helper(const std::array<std::string_view, num_types>& strings, std::index_sequence<I...>)
				{
					return std::make_tuple((collin::strings::from_string<std::tuple_element_t<I, typename base_it::value_type>>(strings[I]))...);
				}

				static typename base_it::value_type parse_elements(const std::array<std::string_view, num_types>& strings)
				{
					return parse_elements_helper(strings, std::make_index_sequence<num_types>{});
				}
		};

		template<>
		class csv_reader<csv_any> : public base_csv_reader, std::iterator<std::input_iterator_tag, std::vector<std::string_view>>
		{
			using base_it = std::iterator<std::input_iterator_tag, std::vector<std::string_view>>;
			using base = base_csv_reader;

			public:
				csv_reader() = default;

				csv_reader(std::istream& is, const dialect& dialect={})
					: base{is, dialect} {}

				csv_reader(std::istream& is, dialect&& dialect)
					: base{is, std::move(dialect)} {}

				csv_reader(const csv_reader&) = default;
				csv_reader(csv_reader&&) noexcept = default;

				csv_reader& operator=(const csv_reader&) = default;
				csv_reader& operator=(csv_reader&&) noexcept = default;

				~csv_reader() noexcept = default;

				[[nodiscard]] csv_reader begin()
				{
					operator++();
					return *this;
				}

				[[nodiscard]] csv_reader end()
				{
					return {};
				}

				const typename base_it::value_type& operator*() const
				{
					return current;
				}

				csv_reader& operator++()
				{
					if (!iostream::getline(*is, line, dialect_.line_terminator))
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
				base_it::value_type current;
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

				template<class InputIterator, typename = std::enable_if_t<
		/* requires */	type_traits::is_iterator_v<InputIterator>
				>>
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
					write_variadic_element(args...);
					os.get() << dialect_.line_terminator;
				}
			private:
				std::reference_wrapper<std::ostream> os;
				dialect dialect_;

				template<class Arg>
				void write_variadic_element(Arg&& arg)
				{
					write_element(arg);
				}

				template<class Arg, class... Args>
				void write_variadic_element(Arg&& arg, Args&&... args)
				{
					write_element(arg);
					os.get() << dialect_.delimiter;
					write_variadic_element(args...);
				}

				template<class T>
				void write_element(const T& element)
				{
					switch (dialect_.quoting)
					{
						case dialect::quoting_option::QUOTE_MINIMAL:
							{
								const auto str = strings::to_string(element);
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