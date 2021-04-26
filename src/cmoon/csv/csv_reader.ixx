export module cmoon.csv.csv_reader;
import <utility>;
import <memory>;
import <iostream>;
import <type_traits>;
import <iterator>;

import cmoon.iostream;
import cmoon.csv.dialect;
import cmoon.csv.csv_any;
import cmoon.csv.csv_ignore;

namespace cmoon::csv
{
	export
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

	export
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

	export
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
}