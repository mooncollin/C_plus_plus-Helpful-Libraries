export module cmoon.iterator.istream_iterator_sep;

import <string>;
import <iterator>;
import <string_view>;
import <memory>;
import <concepts>;

import cmoon.string;
import cmoon.io.read;

namespace cmoon
{
    export
	template<class T, class CharT = char, class Traits = std::char_traits<CharT>>
    class istream_iterator_sep : public std::istream_iterator<T, CharT, Traits>
    {
        using parent = std::istream_iterator<T, CharT, Traits>;
        static constexpr auto end_of_input = nullptr;

        public:
			istream_iterator_sep(std::default_sentinel_t = {}) noexcept
                : input{end_of_input}, has_processed{false} {}

            explicit istream_iterator_sep(std::basic_istream<CharT, Traits>& input, std::basic_string_view<CharT, Traits> sep = " ") noexcept
				: input{std::addressof(input)}, sep_{sep}, has_processed{false} {}

            istream_iterator_sep end() const noexcept
            {
                return istream_iterator_sep();
            }

            istream_iterator_sep begin() const noexcept
            {
                return *this;
            }

            typename parent::reference operator*()
            {
				if (!has_processed)
				{
					next_input();
				}

                return current;
            }

            typename parent::pointer operator->()
            {
				if (!has_processed)
				{
					next_input();
				}

                return std::addressof(current);
            }

            istream_iterator_sep& operator++()
            {
                next_input();
                return *this;
            }

            istream_iterator_sep operator++(int)
            {
                auto l = istream_iterator_sep(*this);
                next_input();

                return l;
            }

			void separator(std::basic_string_view<CharT, Traits> sep)
			{
				sep_ = sep;
			}

			const std::string& separator() const noexcept
			{
				return sep_;
			}

            [[nodiscard]] friend bool operator==(const istream_iterator_sep& lhs, const istream_iterator_sep& rhs) noexcept
            {
                return lhs.input == rhs.input;
            }

            [[nodiscard]] friend bool operator!=(const istream_iterator_sep& lhs, const istream_iterator_sep& rhs) noexcept = default;
            [[nodiscard]] friend bool operator==(const istream_iterator_sep& lhs, std::default_sentinel_t)
            {
                return lhs.input == end_of_input;
            }

        private:
            std::basic_istream<CharT, Traits>* input {end_of_input};
			std::string sep_;
			bool has_processed;
            T current;

            void next_input()
            {
				has_processed = true;
                if(input != end_of_input)
                {
                    if(input->fail() || input->eof())
                    {
                        input = end_of_input;
                    }
                    else
                    {
                        if constexpr(std::same_as<T, std::string>)
                        {
                            cmoon::read_until(*input, current, std::basic_string_view<CharT, Traits>{sep_});
                        }
                        else
                        {
                            std::string str;
                            cmoon::read_until(*input, str, std::basic_string_view<CharT, Traits>{sep_});
                            cmoon::from_string<T>(std::basic_string_view<CharT, Traits>{str}, current);
                        }
                    }
                }
            }
    };
}