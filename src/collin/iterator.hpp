#pragma once

#include <iterator>
#include <tuple>
#include <utility>

#include "collin/string.hpp"
#include "collin/iostream.hpp"
#include "collin/tuple.hpp"
#include "collin/concepts.hpp"

namespace collin
{
	namespace iterator
	{
		template<class Container>
		class back_emplace_iterator : public std::iterator<std::output_iterator_tag, typename Container::value_type, typename Container::difference_type, typename Container::pointer, typename Container::reference>
		{
			public:
				explicit back_emplace_iterator(Container& x)
					: container(x) {}

				template<class... Args>
				back_emplace_iterator& operator=(Args&&... args)
				{
					container.get().emplace_back(std::forward<Args>(args)...);
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
				std::reference_wrapper<Container> container;
		};

		template<class Container>
		back_emplace_iterator<Container> back_emplacer(Container& c)
		{
			return back_emplace_iterator<Container>(c);
		}

		template<class ForwardIterator>
		class enumerate : public std::iterator<std::forward_iterator_tag, std::tuple<int, typename ForwardIterator::value_type>>
		{
			public:
				constexpr explicit enumerate(ForwardIterator begin, ForwardIterator end, int start=0, int step=1)
					: current_(begin), end_(end), index_(start), step_(step) {}

				constexpr enumerate begin() const
				{
					return *this;
				}

				constexpr enumerate end() const
				{
					return enumerate(end_, end_);
				}

				constexpr enumerate& operator++()
				{
					++current_;
					index_ += step_;

					return *this;
				}

				constexpr enumerate operator++(int)
				{
					enumerate e(*this);
					operator++();
					return e;
				}

				constexpr auto operator*() const
				{
					return std::make_pair(index_, current_);
				}

				constexpr bool operator==(const enumerate& rhs) const
				{
					return current_ == rhs.current_;
				}

				constexpr bool operator!=(const enumerate& rhs) const
				{
					return !operator==(rhs);
				}

				constexpr int index() const noexcept
				{
					return index_;
				}

				constexpr int step() const noexcept
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
				istream_iterator_sep() noexcept = default;
                explicit istream_iterator_sep(std::istream& input, std::string_view sep = " ") noexcept
					: input(&input), sep_(sep) {}

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
                            if constexpr(collin::concepts::same<T, std::string>)
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