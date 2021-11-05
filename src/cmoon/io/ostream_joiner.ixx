export module cmoon.io.ostream_joiner;

import <string>;
import <iterator>;
import <iostream>;
import <type_traits>;
import <cstddef>;
import <memory>;

namespace cmoon
{
	export
	template<class DelimT, class CharT = char, class Traits = std::char_traits<CharT>>
	class ostream_joiner
	{
		public:
			using char_type = CharT;
			using traits_type = Traits;
			using ostream_type = std::basic_ostream<CharT, Traits>;
			using value_type = void;
			using difference_type = std::ptrdiff_t;
			using pointer = void;
			using reference = void;
			using iterator_category = std::output_iterator_tag;

			ostream_joiner() noexcept
				: out_{nullptr}, first_element{true} {}

			ostream_joiner(ostream_type& stream, const DelimT& delimiter)
				: out_{std::addressof(stream)}, delim_{delimiter}, first_element{true} {}

			ostream_joiner(ostream_type& stream, DelimT&& delimiter)
				: out_{std::addressof(stream)}, delim_{std::move(delimiter)}, first_element{true} {}

			template<class T>
			ostream_joiner& operator=(const T& value)
			{
				if (!first_element)
				{
					*out_ << delim_;
				}

				first_element = false;
				*out_ << value;
				return *this;
			}

			ostream_joiner& operator*() noexcept
			{
				return *this;
			}

			ostream_joiner& operator++() noexcept
			{
				return *this;
			}

			ostream_joiner& operator++(int) noexcept
			{
				return *this;
			}
		private:
			ostream_type* out_;
			DelimT delim_;
			bool first_element;
	};

	export
	template<class CharT, class Traits, class DelimT>
	ostream_joiner<std::decay_t<DelimT>, CharT, Traits> make_ostream_joiner(std::basic_ostream<CharT, Traits>& os, DelimT&& delimiter)
	{
		return {os, std::forward<DelimT>(delimiter)};
	}
}