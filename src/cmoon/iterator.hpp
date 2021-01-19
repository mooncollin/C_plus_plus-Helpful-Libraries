#pragma once

#include <iterator>
#include <tuple>
#include <utility>
#include <memory>
#include <functional>
#include <concepts>

#include "cmoon/string.hpp"
#include "cmoon/iostream.hpp"
#include "cmoon/tuple.hpp"

namespace cmoon
{
	template<class Container>
	class back_emplace_iterator
	{
		public:
			using difference_type = typename Container::difference_type;
			using value_type = typename Container::value_type;
			using pointer = typename Container::pointer;
			using reference = typename Container::reference;
			using iterator_category = std::output_iterator_tag;

			constexpr explicit back_emplace_iterator(Container& x)
				: container(x) {}

			template<class... Args>
			constexpr back_emplace_iterator& operator=(Args&&... args)
			{
				container.get().emplace_back(std::forward<Args>(args)...);
				return *this;
			}

			constexpr back_emplace_iterator& operator*()
			{
				return *this;
			}

			constexpr back_emplace_iterator& operator++()
			{
				return *this;
			}

			constexpr back_emplace_iterator& operator++(int)
			{
				return *this;
			}
		private:
			std::reference_wrapper<Container> container;
	};

	template<class Container>
	[[nodiscard]] constexpr back_emplace_iterator<Container> back_emplacer(Container& c)
	{
		return back_emplace_iterator<Container>(c);
	}

	template<std::input_iterator InputIterator>
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

	template<class... Ranges>
		requires(sizeof...(Ranges) > 0)
	class zip_ranges
	{
		using iterators_type = std::tuple<std::ranges::iterator_t<Ranges>...>;
		using ranges_type = std::tuple<std::reference_wrapper<Ranges>...>;
		static constexpr auto range_size = sizeof...(Ranges);

		public:
			using difference_type = std::ptrdiff_t;
			using value_type = std::tuple<std::ranges::range_value_t<Ranges>...>;
			using pointer = value_type*;
			using reference = value_type&;
			using iterator_category = std::input_iterator_tag;

			constexpr zip_ranges(Ranges&... ranges)
				: ranges_{std::make_tuple(std::reference_wrapper<Ranges>{ranges}...)},
					current_{std::make_tuple(std::begin(ranges)...)} {}

			constexpr zip_ranges& operator++()
			{
				cmoon::tuples::for_each(current_, [](auto& v) { ++v; });
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

	template<std::ranges::range T>
		requires(requires(T t) { std::rbegin(t); std::rend(t); })
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

	namespace details
	{
		template<std::input_iterator InputIt, std::invocable<std::iter_value_t<InputIt>> Function>
		class filter_impl
		{
			public:
				using difference_type = std::iter_difference_t<InputIt>;
				using value_type = std::iter_value_t<InputIt>;
				using pointer = typename std::iterator_traits<InputIt>::pointer;
				using reference = std::iter_reference_t<InputIt>;
				using iterator_category = std::input_iterator_tag;

				filter_impl(InputIt begin, InputIt end, Function& f)
					: current_{begin}, end_{end}, function_{f}
				{
					operator++();
				}

				reference operator*()
				{
					return *current_;
				}

				pointer operator->()
				{
					return std::addressof(*current_);
				}

				filter_impl& operator++()
				{
					while (current_ != end_ && !function_.get()(*current_))
					{
						++current_;
					}

					return *this;
				}

				filter_impl operator++(int)
				{
					auto r = *this;
					++*this;
					return r;
				}
					
				[[nodiscard]] friend constexpr bool operator==(const filter_impl& lhs, const filter_impl& rhs) noexcept
				{
					return lhs.current_ == rhs.current_;
				}

				[[nodiscard]] friend constexpr bool operator!=(const filter_impl& lhs, const filter_impl& rhs) noexcept
				{
					return !(lhs == rhs);
				}
			private:
				InputIt current_;
				InputIt end_;
				std::reference_wrapper<Function> function_;
		};
	}

	template<std::input_iterator InputIt, std::invocable<std::iter_value_t<InputIt>> Function>
	class filter
	{
		public:
			using difference_type = std::iter_difference_t<InputIt>;
			using value_type = std::iter_value_t<InputIt>;
			using pointer = typename std::iterator_traits<InputIt>::pointer;
			using reference = std::iter_reference_t<InputIt>;
			using iterator = details::filter_impl<InputIt, Function>;

			constexpr filter(InputIt begin, InputIt end, Function&& f)
				: begin_{begin}, end_{end}, function_{std::forward<Function>(f)} {}

			constexpr iterator begin() const noexcept
			{
				return iterator{begin_, end_, function_};
			}

			constexpr iterator end() const noexcept
			{
				return iterator{end_, end_, function_};
			}
		private:
			InputIt begin_;
			InputIt end_;
			Function function_;
	};
}