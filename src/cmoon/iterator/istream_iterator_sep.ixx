export module cmoon.iterator.istream_iterator_sep;

import <string>;
import <iterator>;
import <string_view>;
import <memory>;
import <concepts>;

import cmoon.string;

namespace cmoon
{
    export
	template<class T = std::string>
    class istream_iterator_sep : public std::istream_iterator<T>
    {
        using parent = std::istream_iterator<T>;
        static constexpr auto end_of_input = nullptr;

        public:
			istream_iterator_sep() noexcept = default;
            explicit istream_iterator_sep(std::istream& input, std::string_view sep = " ") noexcept
				: input{std::addressof(input)}, sep_{sep} {}

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
				if (!have_processed)
				{
					next_input();
				}

                return current;
            }

            typename parent::pointer operator->()
            {
				if (!have_processed)
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

			void seperator(std::string_view sep)
			{
				sep_ = sep;
			}

			const std::string& seperator() const noexcept
			{
				return sep_;
			}

            bool operator==(const istream_iterator_sep& rhs) const noexcept
            {
                return input == rhs.input;
            }

            bool operator!=(const istream_iterator_sep& rhs) const noexcept
            {
                return !(*this == rhs);
            }

        private:
            std::istream* input {end_of_input};
			std::string sep_;
			bool have_processed {false};
            T current;

            void next_input()
            {
				have_processed = true;
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
							current.clear();
                            cmoon::getline(*input, current, sep_);
                        }
                        else
                        {
                            std::string str;
                            cmoon::getline(*input, str, sep_);

                            current = cmoon::from_string<T>(str);
                        }
                    }
                }
            }
    };
}