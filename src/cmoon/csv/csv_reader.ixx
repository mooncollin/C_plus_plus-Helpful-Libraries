export module cmoon.csv.csv_reader;

import <utility>;
import <memory>;
import <iostream>;
import <type_traits>;
import <iterator>;
import <string>;
import <string_view>;
import <vector>;
import <array>;

import cmoon.iostream;
import cmoon.string;

import cmoon.csv.dialect;
import cmoon.csv.csv_any;
import cmoon.csv.csv_ignore;
import cmoon.csv.parse_line;

namespace cmoon::csv
{
	export
	template<class CharT, class Traits = std::char_traits<CharT>>
	class base_csv_reader
	{
		public:
			base_csv_reader() = default;

			base_csv_reader(std::basic_istream<CharT, Traits>& is, const basic_dialect<CharT, Traits>& dialect={})
				: is{std::addressof(is)}, dialect_{dialect} {}

			base_csv_reader(std::basic_istream<CharT, Traits>& is, basic_dialect<CharT, Traits>&& dialect)
				: is{std::addressof(is)}, dialect_{std::move(dialect)} {}

			bool operator==(const base_csv_reader& other) const
			{
				return is == other.is;
			}

			bool operator!=(const base_csv_reader& other) const
			{
				return !(*this == other);
			}

			basic_dialect<CharT, Traits>& get_dialect()
			{
				return dialect_;
			}

			const basic_dialect<CharT, Traits>& get_dialect() const
			{
				return dialect_;
			}
		protected:
			static constexpr auto end_of_input {nullptr};

			std::basic_istream<CharT, Traits>* is {end_of_input};
			basic_dialect<CharT, Traits> dialect_;
	};

	export
	template<class CharT, class Traits, class... Args>
		requires((... && !std::is_void_v<Args>))
	class basic_csv_reader : public base_csv_reader<CharT, Traits>
	{
		static constexpr std::size_t num_types{sizeof...(Args)};
		using base = base_csv_reader<CharT, Traits>;

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::tuple<Args...>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;

			basic_csv_reader() = default;

			basic_csv_reader(std::basic_istream<CharT, Traits>& is, const basic_dialect<CharT, Traits>& dialect = {})
				: base{is, dialect} {}

			basic_csv_reader(std::basic_istream<CharT, Traits>& is, basic_dialect<CharT, Traits>&& dialect)
				: base{is, std::move(dialect)} {}

			basic_csv_reader begin()
			{
				operator++();
				return *this;
			}

			[[nodiscard]] basic_csv_reader end() const
			{
				return {};
			}

			const value_type& operator*() const
			{
				return current;
			}

			basic_csv_reader& operator++()
			{
				if (cmoon::getline(*this->is, line, std::basic_string_view<CharT, Traits>{this->dialect_.line_terminator.data(), this->dialect_.line_terminator.size()}).eof())
				{
					this->is = this->end_of_input;
					return *this;
				}

				std::array<std::basic_string_view<CharT, Traits>, num_types> string_elements;
				parse_line(std::basic_string_view<CharT, Traits>{line.data(), line.size()}, this->dialect_, std::begin(string_elements), std::size(string_elements));

				current = parse_elements(string_elements);

				return *this;
			}

			basic_csv_reader operator++(int)
			{
				auto v {*this};
				operator++();
				return v;
			}
		private:
			value_type current;
			std::basic_string<CharT, Traits> line;

			template<class T>
			inline static auto before_from_string(std::basic_string_view<CharT, Traits> v)
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
			static value_type parse_elements_helper(const std::array<std::basic_string_view<CharT, Traits>, num_types>& strings, std::index_sequence<I...>)
			{
				return std::make_tuple((before_from_string<std::tuple_element_t<I, value_type>>(strings[I]))...);
			}

			static value_type parse_elements(const std::array<std::basic_string_view<CharT, Traits>, num_types>& strings)
			{
				return parse_elements_helper(strings, std::make_index_sequence<num_types>{});
			}
	};

	export
	template<class CharT, class Traits>
	class basic_csv_reader<CharT, Traits, csv_any> : public base_csv_reader<CharT, Traits>
	{
		using base = base_csv_reader<CharT, Traits>;

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::vector<std::basic_string_view<CharT, Traits>>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;

			basic_csv_reader() = default;

			basic_csv_reader(std::basic_istream<CharT, Traits>& is, const basic_dialect<CharT, Traits>& dialect={})
				: base{is, dialect} {}

			basic_csv_reader(std::basic_istream<CharT, Traits>& is, basic_dialect<CharT, Traits>&& dialect)
				: base{is, std::move(dialect)} {}

			basic_csv_reader begin()
			{
				operator++();
				return *this;
			}

			[[nodiscard]] basic_csv_reader end() const
			{
				return {};
			}

			const value_type& operator*() const
			{
				return current;
			}

			basic_csv_reader& operator++()
			{
				if (cmoon::getline(*this->is, line, std::basic_string_view<CharT, Traits>{this->dialect_.line_terminator.data(), this->dialect_.line_terminator.size()}).eof())
				{
					this->is = this->end_of_input;
					return *this;
				}

				current.clear();
				parse_line(std::basic_string_view<CharT, Traits>{line.data(), line.size()}, this->dialect_, std::back_inserter(current));

				return *this;
			}

			basic_csv_reader operator++(int)
			{
				auto v {*this};
				operator++();
				return v;
			}
		private:
			value_type current;
			std::basic_string<CharT, Traits> line;
	};

	export
	template<class... Args>
	using csv_reader = basic_csv_reader<char, std::char_traits<char>, Args...>;

	export
	template<class... Args>
	using wcsv_reader = basic_csv_reader<wchar_t, std::char_traits<wchar_t>, Args...>;
}