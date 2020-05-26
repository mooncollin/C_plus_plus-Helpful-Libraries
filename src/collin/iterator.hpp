#pragma once

#include <iterator>
#include <tuple>
#include "string.hpp"
#include "iostream.hpp"

namespace collin
{
	namespace iterator
	{
		template<class Container>
		class back_emplace_iterator : public std::iterator<std::output_iterator_tag, typename Container::value_type, typename Container::difference_type, typename Container::pointer, typename Container::reference>
		{
			public:
				explicit back_emplace_iterator(Container& x)
					: container(&x)
				{
				}

				template<class... Args>
				back_emplace_iterator<Container>& operator=(Args&&... args)
				{
					container->emplace_back(std::forward<Args>(args)...);
					return *this;
				}

				back_emplace_iterator& operator*()
				{
					return *this;
				}

				back_emplace_iterator& operator++()
				{
					return *this;
				}

				back_emplace_iterator& operator++(int)
				{
					return *this;
				}
			private:
				Container* container;
		};

		template<class Container>
		back_emplace_iterator<Container> back_emplacer(Container& c)
		{
			return back_emplace_iterator<Container>(c);
		}

		template<class ForwardIterator>
		class Enumerate : public std::iterator<std::forward_iterator_tag, std::tuple<int, typename ForwardIterator::value_type>>
		{
			public:
				explicit Enumerate(ForwardIterator begin, ForwardIterator end, int start=0, int step=1)
					: current_(begin), end_(end), index_(start), step_(step) {}

				Enumerate(const Enumerate& other) = default;
				Enumerate& operator=(const Enumerate& other) = default;

				Enumerate(Enumerate&&) = default;
				Enumerate& operator=(Enumerate&&) = default;

				Enumerate begin() const
				{
					return *this;
				}

				Enumerate end() const
				{
					return Enumerate(end_, end_);
				}

				Enumerate& operator++()
				{
					++current_;
					index_ += step_;

					return *this;
				}

				Enumerate operator++(int)
				{
					Enumerate e(*this);
					operator++();
					return e;
				}

				auto operator*() const
				{
					return std::make_tuple(index_, current_);
				}

				bool operator==(const Enumerate& rhs) const
				{
					return current_ == rhs.current_;
				}

				bool operator!=(const Enumerate& rhs) const
				{
					return !operator==(rhs);
				}

				int index() const noexcept
				{
					return index_;
				}

				int step() const noexcept
				{
					return step_;
				}

			private:
				ForwardIterator current_;
				ForwardIterator end_;
				int index_;
				int step_;
		};

		template<class T = std::string>
        class istream_iterator_sep : public std::istream_iterator<T>
        {
            using parent = std::istream_iterator<T>;
            static constexpr auto end_of_input = nullptr;

            public:
				istream_iterator_sep() = default;
				istream_iterator_sep(const istream_iterator_sep&) = default;
				istream_iterator_sep(istream_iterator_sep&&) = default;
				istream_iterator_sep& operator=(const istream_iterator_sep&) = default;
				istream_iterator_sep& operator=(istream_iterator_sep&&) = default;

                istream_iterator_sep(std::istream& input) : input(&input)
                {
                }

				istream_iterator_sep(std::istream& input, std::string_view sep) : input(&input), sep_(sep)
                {
                }

                istream_iterator_sep end() const
                {
                    return istream_iterator_sep();
                }

                istream_iterator_sep begin() const
                {
                    return *this;
                }

                typename parent::reference operator*()
                {
                    return current;
                }

                typename parent::pointer operator->()
                {
                    return &current;
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

				const std::string& seperator() const
				{
					return sep_;
				}

                bool operator==(const istream_iterator_sep& rhs)
                {
                    return input == rhs.input;
                }

                bool operator!=(const istream_iterator_sep& rhs)
                {
                    return !(*this == rhs);
                }

            private:
                std::istream* input {end_of_input};
				std::string sep_ {" "};
                T current;

                void next_input()
                {
                    if(input != end_of_input)
                    {
                        if(input->fail() || input->eof())
                        {
                            input = end_of_input;
                        }
                        else
                        {
                            if constexpr(std::is_same_v<T, std::string>)
                            {
								current.clear();
                                iostream::getline(*input, current, sep_);
                            }
                            else
                            {
                                std::string str;
                                iostream::getline(*input, str, sep_);

                                current = strings::from_string<T>(str);
                            }
                        }
                    }
                }
        };
	}
}