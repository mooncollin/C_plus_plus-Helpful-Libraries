export module cmoon.csv.csv_reader;

import <utility>;
import <memory>;
import <iostream>;
import <type_traits>;
import <iterator>;
import <string>;
import <string_view>;
import <vector>;
import <tuple>;
import <ranges>;
import <functional>;
import <optional>;
import <concepts>;
import <array>;

import cmoon.io;
import cmoon.string;
import cmoon.tuple;

import cmoon.csv.dialect;
import cmoon.csv.csv_any;
import cmoon.csv.ignore;
import cmoon.csv.parse_line;

namespace cmoon::csv
{
	template<class CharT, class Traits>
	class base_csv_reader;

	template<class CharT, class Traits, class... Args>
		requires(sizeof...(Args) > 0 &&
				 std::conjunction_v<std::is_object<Args>...>)
	class basic_csv_reader;

	template<class CharT, class Traits, class... Args>
	class csv_row
	{
		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::tuple<Args...>;
			using pointer = const value_type*;
			using reference = const value_type&;
			using iterator_category = std::input_iterator_tag;

			[[nodiscard]] reference operator*() const noexcept
			{
				return *current;
			}

			pointer operator->() const noexcept
			{
				return current.operator->();
			}

			csv_row& operator++()
			{
				std::string line;

				if (cmoon::read_until(reader.get().is.get(), line, std::basic_string_view{reader.get().dialect().line_terminator}))
				{
					//std::array<std::basic_string_view<CharT, Traits>, sizeof...(Args)> tokens;
					//parse_line(std::basic_string_view{line}, reader.get().dialect(), std::ranges::begin(tokens), std::ranges::size(tokens));
					//current = parse_elements(tokens);
				}

				return *this;
			}

			void operator++(int)
			{
				++*this;
			}

			[[nodiscard]] bool operator==(std::default_sentinel_t) const noexcept
			{
				return !static_cast<bool>(reader.get().is.get());
			}

			[[nodiscard]] bool operator!=(std::default_sentinel_t) const noexcept
			{
				return static_cast<bool>(reader.get().is.get());
			}
		private:
			csv_row(basic_csv_reader<CharT, Traits, Args...>& reader)
				: reader{reader}
			{
				++*this;
			}

			std::reference_wrapper<basic_csv_reader<CharT, Traits, Args...>> reader;
			std::optional<value_type> current;

			template<class T>
			[[nodiscard]] inline static constexpr auto before_from_string(std::basic_string_view<CharT, Traits> v) noexcept(std::same_as<T, csv_ignore_t>)
			{
				if constexpr (std::same_as<T, csv_ignore_t>)
				{
					return csv_ignore;
				}
				else
				{
					return cmoon::from_string<T>(v);
				}
			}

			template<class TupleLike, std::size_t... I>
			[[nodiscard]] static value_type parse_elements_helper(const TupleLike& strings, std::index_sequence<I...>)
			{
				return std::make_tuple((before_from_string<std::tuple_element_t<I, value_type>>(std::get<I>(strings)))...);
			}

			template<class TupleLike>
			[[nodiscard]] static value_type parse_elements(const TupleLike& strings)
			{
				return parse_elements_helper(strings, std::make_index_sequence<sizeof...(Args)>{});
			}

			template<class CharT2, class Traits2, class... Args2>
				requires(sizeof...(Args2) > 0 &&
						 std::conjunction_v<std::is_object<Args2>...>)
			friend class basic_csv_reader;
	};

	template<class CharT, class Traits>
	class csv_row<CharT, Traits, csv_any>
	{
		using container_type = std::vector<std::basic_string<CharT, Traits>>;

		// std::back_inserter will not convert std::string_view
		// to std::string because it is a explicit constructor
		class string_converting_back_insert_iterator
		{
			public:
				using iterator_category = std::output_iterator_tag;
				using value_type = void;
				using difference_type = std::ptrdiff_t;
				using pointer = void;
				using reference = void;

				string_converting_back_insert_iterator(container_type& c) noexcept
					: container{c} {}

				string_converting_back_insert_iterator& operator=(std::basic_string_view<CharT, Traits> view)
				{
					container.get().emplace_back(view);
					return *this;
				}

				string_converting_back_insert_iterator& operator*() noexcept
				{
					return *this;
				}

				string_converting_back_insert_iterator& operator++() noexcept
				{
					return *this;
				}

				string_converting_back_insert_iterator operator++(int) noexcept
				{
					auto temp {*this};
					++*this;
					return temp;
				}
			private:
				std::reference_wrapper<container_type> container;
		};

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::views::all_t<const container_type&>;
			using pointer = void;
			using reference = value_type;
			using iterator_category = std::input_iterator_tag;

			value_type operator*() const noexcept
			{
				return std::views::all(std::as_const(current));
			}

			csv_row& operator++()
			{
				std::basic_string<CharT, Traits> line;
				if (cmoon::read_until(reader.get().is.get(), line, std::basic_string_view{reader.get().dialect().line_terminator}))
				{
					container_type temp;
					parse_line(std::basic_string_view{line}, reader.get().dialect(), string_converting_back_insert_iterator{temp});
					current = std::move(temp);
				}

				return *this;
			}

			void operator++(int)
			{
				++*this;
			}

			[[nodiscard]] bool operator==(std::default_sentinel_t) const noexcept
			{
				return !static_cast<bool>(reader.get().is.get());
			}

			[[nodiscard]] bool operator!=(std::default_sentinel_t) const noexcept
			{
				return static_cast<bool>(reader.get().is.get());
			}
		private:
			csv_row(basic_csv_reader<CharT, Traits, csv_any>& reader)
				: reader{reader}
			{
				++*this;
			}

			std::reference_wrapper<basic_csv_reader<CharT, Traits, csv_any>> reader;
			container_type current;

			template<class CharT2, class Traits2, class... Args2>
				requires(sizeof...(Args2) > 0 &&
						 std::conjunction_v<std::is_object<Args2>...>)
			friend class basic_csv_reader;
	};

	template<class CharT, class Traits, class... Args>
		requires(sizeof...(Args) > 0 &&
				 std::conjunction_v<std::is_object<Args>...>)
	class basic_csv_reader
	{
		public:
			basic_csv_reader(std::basic_istream<CharT, Traits>& is, basic_dialect<CharT, Traits> dialect={}) noexcept
				: is{is}, dialect_{std::move(dialect)} {}

			[[nodiscard]] auto begin() noexcept
			{
				return csv_row{*this};
			}

			[[nodiscard]] std::default_sentinel_t end() const noexcept
			{
				return std::default_sentinel;
			}

			const basic_dialect<CharT, Traits>& dialect() const noexcept
			{
				return dialect_;
			}
		protected:
			std::reference_wrapper<std::basic_istream<CharT, Traits>> is;
			basic_dialect<CharT, Traits> dialect_;

			template<class CharT, class Traits, class... Args>
			friend class csv_row;
	};

	export
	template<class... Args>
	using csv_reader = basic_csv_reader<char, std::char_traits<char>, Args...>;

	export
	template<class... Args>
	using wcsv_reader = basic_csv_reader<wchar_t, std::char_traits<wchar_t>, Args...>;
}