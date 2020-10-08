#pragma once

#include <iterator>
#include <tuple>
#include <utility>
#include <memory>
#include <functional>

#include "collin/string.hpp"
#include "collin/iostream.hpp"
#include "collin/tuple.hpp"
#include "collin/concepts.hpp"

namespace collin
{
	namespace iterators
	{
		/*
		* I have the redefinition of iterator concepts here, because this is where I
		* want them to eventually be. However, MSVC will not let me move the concepts
		* out of the concepts header. If I do, then this namespace is hidden for some
		* reason.
		*/
		template<class T>
		concept iterator = concepts::copyable<T> &&
						   concepts::swappable<T> &&
			requires(T it)
		{
			typename std::iterator_traits<T>::value_type;
			typename std::iterator_traits<T>::difference_type;
			typename std::iterator_traits<T>::reference;
			typename std::iterator_traits<T>::pointer;
			typename std::iterator_traits<T>::iterator_category;

			{ *it }  -> concepts::referenceable;
			{ ++it } -> concepts::convertible_to<T&>;
			{ *it++ } -> concepts::referenceable;
		};

		template<class T>
		concept input_iterator = iterator<T> &&
								 concepts::equality_comparable<T> &&
			requires(T it, const T const_it)
		{
			{ it != const_it }  -> concepts::boolean;
			{ it.operator->() };
		};

		template<class T, class U>
		concept output_iterator = iterator<T> &&
								  (std::is_pointer_v<T> || std::is_class_v<T>) &&
			requires(T it, U&& u)
		{
			{ ++it } -> concepts::convertible_to<T&>;
			{ it++ } -> concepts::convertible_to<const T&>;
			{ *it++ = std::forward<U>(u) };
			{ *it = std::forward<U>(u) };
		};

		template<class T>
		concept forward_iterator = input_iterator<T> &&
								   concepts::default_constructible<T> &&
								   ((output_iterator<T, std::add_lvalue_reference_t<typename std::iterator_traits<T>::value_type>> && concepts::same<typename std::iterator_traits<T>::reference, std::add_lvalue_reference_t<typename std::iterator_traits<T>::value_type>>) ||
									concepts::same<typename std::iterator_traits<T>::reference, const std::add_lvalue_reference_t<typename std::iterator_traits<T>::value_type>>) &&
			requires(T it)
		{
			{ it++ }  -> concepts::same<T>;
			{ *it++ } -> concepts::same<typename std::iterator_traits<T>::reference>;
		};

		template<class T>
		concept bidirectional_iterator = forward_iterator<T> &&
			requires(T a)
		{
			{ --a }  -> concepts::same<T&>;
			{ a-- }  -> concepts::convertible_to<const T&>;
			{ *a-- } -> concepts::same<typename std::iterator_traits<T>::reference>;
		};

		template<class T>
		concept random_access_iterator = bidirectional_iterator<T> &&
			requires(T a, const T b, T& r, typename std::iterator_traits<T>::difference_type n)
		{
			{ r += n } -> concepts::same<T&>;
			{ a + n }  -> concepts::same<T>;
			{ n + a }  -> concepts::same<T>;
			{ r -= n } -> concepts::same<T&>;
			{ a - n }  -> concepts::same<T>;
			{ b - a }  -> concepts::same<typename std::iterator_traits<T>::difference_type>;
			{ a[n] }   -> concepts::convertible_to<typename std::iterator_traits<T>::reference>;
			{ a < b }  -> concepts::boolean;
			{ a > b }  -> concepts::boolean;
			{ a >= b } -> concepts::boolean;
			{ a <= b } -> concepts::boolean;
		};

		template<class T>
		concept contiguous_iterator = random_access_iterator<T> &&
			requires(T a, typename std::iterator_traits<T>::difference_type n)
		{
			{ *(a + n) } -> concepts::same<decltype(*(std::addressof(*a) + n))>;
		};
		
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

		template<class InputIterator>
		class enumerate
		{
			public:
				using difference_type = std::intmax_t;
				using value_type = std::pair<std::intmax_t, typename std::iterator_traits<InputIterator>::value_type>;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::input_iterator_tag;

				constexpr explicit enumerate(InputIterator begin, InputIterator end, difference_type start=0, difference_type step=1)
					: current_(begin), end_(end), index_(start), step_(step) {}

				constexpr enumerate begin()
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
					auto e = *this;
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

				constexpr difference_type index() const noexcept
				{
					return index_;
				}

				constexpr difference_type step() const noexcept
				{
					return step_;
				}

			private:
				InputIterator current_;
				InputIterator end_;
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

		template<class... Ranges>
			requires(sizeof...(Ranges) > 0)
		class zip_ranges
		{
			using iterators_type = std::tuple<decltype(std::begin(std::declval<Ranges>()))...>;
			using ranges_type = std::tuple<std::reference_wrapper<Ranges>...>;
			static constexpr auto range_size = sizeof...(Ranges);

			public:
				using difference_type = std::ptrdiff_t;
				using value_type = std::tuple<typename std::iterator_traits<decltype(std::begin(std::declval<Ranges>()))>::value_type...>;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::input_iterator_tag;

				constexpr zip_ranges(Ranges&... ranges)
					: ranges_{std::make_tuple(std::reference_wrapper<Ranges>{ranges}...)},
					  current_{std::make_tuple(std::begin(ranges)...)} {}

				constexpr zip_ranges& operator++()
				{
					collin::tuples::for_each(current_, [](auto& v) { ++v; });
					return *this;
				}

				constexpr zip_ranges operator++(int)
				{
					auto r = *this;
					operator++();
					return r;
				}

				constexpr value_type operator*() const
				{
					return dereference_helper(std::make_index_sequence<range_size>{});
				}

				constexpr value_type operator*()
				{
					return dereference_helper(std::make_index_sequence<range_size>{});
				}

				constexpr zip_ranges begin()
				{
					return *this;
				}

				constexpr zip_ranges end() const
				{
					return end_helper(std::make_index_sequence<range_size>{});
				}

				[[nodiscard]] constexpr bool operator==(const zip_ranges& rhs) const noexcept
				{
					return equal_helper(rhs, std::make_index_sequence<range_size>{});
				}

				[[nodiscard]] constexpr bool operator!=(const zip_ranges& rhs) const noexcept
				{
					return !(*this == rhs);
				}
			private:
				ranges_type ranges_;
				iterators_type current_;

				struct do_end {};

				constexpr zip_ranges(do_end, Ranges&... ranges)
					: ranges_{std::make_tuple(std::reference_wrapper<Ranges>{ranges}...)},
					  current_{std::make_tuple(std::end(ranges)...)} {}

				template<std::size_t... I>
				[[nodiscard]] constexpr zip_ranges end_helper(std::index_sequence<I...>) const noexcept
				{
					return {do_end{}, std::get<I>(ranges_).get()...};
				}

				template<std::size_t... I>
				constexpr value_type dereference_helper(std::index_sequence<I...>)
				{
					return std::make_tuple((*std::get<I>(current_))...);
				}

				template<std::size_t... I>
				constexpr value_type dereference_helper(std::index_sequence<I...>) const
				{
					return std::make_tuple((*std::get<I>(current_))...);
				}

				template<std::size_t... I>
				[[nodiscard]] constexpr bool equal_helper(const zip_ranges& rhs, std::index_sequence<I...>) const noexcept
				{
					return ((std::get<I>(current_) == std::get<I>(rhs.current_)) || ...);
				}
		};

		template<class T>
		class reverse
		{
			public:
				constexpr explicit reverse(T& iterable)
					: iterable_{iterable} {}

				[[nodiscard]] constexpr auto begin() const
				{
					return std::rbegin(iterable_);
				}

				[[nodiscard]] constexpr auto end() const
				{
					return std::rend(iterable_);
				}

				[[nodiscard]] constexpr auto rbegin() const
				{
					return std::begin(iterable_);
				}

				[[nodiscard]] constexpr auto rend() const
				{
					return std::end(iterable_);
				}
			private:
				T& iterable_;
		};
	}
}