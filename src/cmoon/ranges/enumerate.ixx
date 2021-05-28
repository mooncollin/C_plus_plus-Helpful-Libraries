export module cmoon.ranges.enumerate;

import <ranges>;
import <utility>;
import <iterator>;
import <cstddef>;

namespace cmoon::ranges
{
	export
	template<std::ranges::input_range Rng>
	class enumerate_view : public std::ranges::view_interface<enumerate_view<Rng>>
	{
		public:
			using count_type = std::size_t;
		private:
			class enumerate_view_iterator
			{
				template<typename T>
				using copy_or_reference_type = std::conditional_t<std::ranges::forward_range<T>,
																  std::ranges::range_reference_t<T>,
																  std::ranges::range_value_t<T>>;

				public:
					using difference_type = std::ranges::range_difference_t<Rng>;
					using value_type = std::pair<copy_or_reference_type<Rng>, count_type>;
					using pointer = void;
					using reference = value_type;
					using iterator_category = std::common_type_t<std::random_access_iterator_tag,
																 typename std::iterator_traits<std::ranges::iterator_t<Rng>>::iterator_category>;

					constexpr enumerate_view_iterator() noexcept = default;

					constexpr enumerate_view_iterator(std::ranges::iterator_t<Rng> it, count_type start)
						: it{it}, count{start} {}

					constexpr reference operator*()
					{
						return {*it, count};
					}

					constexpr enumerate_view_iterator& operator++()
					{
						++it;
						++count;
						return *this;
					}

					constexpr enumerate_view_iterator& operator++(int)
					{
						auto tmp = *this;
						++*this;
						return tmp;
					}

					constexpr enumerate_view_iterator& operator--()
						requires(std::ranges::bidirectional_range<Rng>)
					{
						--it;
						--count;
						return *this;
					}

					constexpr enumerate_view_iterator& operator--(int)
						requires(std::ranges::bidirectional_range<Rng>)
					{
						auto tmp = *this;
						--*this;
						return tmp;
					}

					constexpr enumerate_view_iterator& operator+=(difference_type n)
						requires(std::ranges::random_access_range<Rng>)
					{
						it += n;
						count += static_cast<count_type>(n);
						return *this;
					}

					constexpr enumerate_view_iterator& operator-=(difference_type n)
						requires(std::ranges::random_access_range<Rng>)
					{
						it -= n;
						count -= static_cast<count_type>(n);
						return *this;
					}

					[[nodiscard]] friend constexpr enumerate_view_iterator operator+(const enumerate_view_iterator& lhs, difference_type rhs)
						requires(std::ranges::random_access_range<Rng>)
					{
						return enumerate_view_iterator{lhs.it + rhs, lhs.count + static_cast<count_type>(rhs)};
					}

					[[nodiscard]] friend constexpr enumerate_view_iterator operator+(difference_type lhs, const enumerate_view_iterator& rhs)
						requires(std::ranges::random_access_range<Rng>)
					{
						return rhs + lhs;
					}

					[[nodiscard]] friend constexpr enumerate_view_iterator operator-(const enumerate_view_iterator& lhs, difference_type rhs)
						requires(std::ranges::random_access_range<Rng>)
					{
						return enumerate_view_iterator{lhs.it - rhs, lhs.count - static_cast<count_type>(rhs)};
					}

					[[nodiscard]] friend constexpr difference_type operator-(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs)
						requires(std::ranges::random_access_range<Rng>)
					{
						return lhs.it - rhs.it;
					}

					constexpr reference operator[](difference_type idx)
						requires(std::ranges::random_access_range<Rng>)
					{
						return {it[idx], count + idx};
					}

					[[nodiscard]] friend constexpr bool operator==(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs)
					{
						return lhs.it == rhs.it;
					}

					[[nodiscard]] friend constexpr bool operator!=(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs) = default;
					[[nodiscard]] friend constexpr bool operator<(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs)
						requires(std::ranges::random_access_range<Rng>)
					{
						return lhs.it < rhs.it;
					}

					[[nodiscard]] friend constexpr bool operator>(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs) requires(std::ranges::random_access_range<Rng>) = default;
					[[nodiscard]] friend constexpr bool operator<=(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs) requires(std::ranges::random_access_range<Rng>) = default;
					[[nodiscard]] friend constexpr bool operator>=(const enumerate_view_iterator& lhs, const enumerate_view_iterator& rhs) requires(std::ranges::random_access_range<Rng>) = default;
				private:
					std::ranges::iterator_t<Rng> it {};
					count_type count {};
			};
		public:
			constexpr enumerate_view(Rng&& rng, count_type start=0)
				: rng{rng}, start{start} {}

			[[nodiscard]] constexpr auto begin() const
			{
				return enumerate_view_iterator{std::ranges::begin(rng), start};
			}

			[[nodiscard]] constexpr auto end() const
			{
				return enumerate_view_iterator{std::ranges::end(rng), start};
			}

			[[nodiscard]] constexpr auto empty() const
				requires(requires(Rng rng) { std::ranges::empty(rng); })
			{
				return std::ranges::empty(rng);
			}

			[[nodiscard]] constexpr operator bool() const
				requires(requires(Rng rng) { std::ranges::empty(rng); })
			{
				return !empty();
			}

			[[nodiscard]] constexpr auto size() const
				requires(std::ranges::sized_range<Rng>)
			{
				return std::ranges::size(rng);
			}

			constexpr auto operator[](std::size_t idx) const
				requires(std::ranges::random_access_range<Rng>)
			{
				return typename enumerate_view_iterator::value_type{*(std::ranges::begin(rng) + idx), start + idx};
			}

			constexpr auto front() const
			{
				return typename enumerate_view_iterator::value_type{*std::ranges::begin(rng), start};
			}

			constexpr auto back() const
				requires(std::ranges::bidirectional_range<Rng> && std::ranges::common_range<Rng>)
			{
				if constexpr (std::ranges::sized_range<Rng>)
				{
					return typename enumerate_view_iterator::value_type{*(--std::ranges::end(rng)), start + size() - 1};
				}
				else
				{
					return typename enumerate_view_iterator::value_type{*(--std::ranges::end(rng)), start};
				}
			}
		private:
			std::ranges::ref_view<std::decay_t<Rng>> rng;
			count_type start;
	};

	export
	template<std::ranges::input_range Rng>
	enumerate_view(Rng&&) -> enumerate_view<Rng>;

	namespace views
	{
		export
		template<std::ranges::input_range Rng>
		[[nodiscard]] constexpr enumerate_view<Rng> enumerate(Rng&& rng, typename enumerate_view<Rng>::count_type count = 0)
		{
			return {std::forward<Rng>(rng), count};
		}
	}
}