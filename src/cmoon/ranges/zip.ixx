export module cmoon.ranges.zip;

import <ranges>;
import <functional>;
import <utility>;
import <iterator>;
import <type_traits>;
import <tuple>;
import <concepts>;

namespace cmoon::ranges
{
	export
	template<std::ranges::input_range... Rngs>
		requires(sizeof...(Rngs) > 0)
	class zip_view : public std::ranges::view_interface<zip_view<Rngs...>>
	{
		class zip_view_iterator
		{
			template<typename T>
			using copy_or_reference_type = std::conditional_t<std::ranges::forward_range<T>,
															  std::ranges::range_reference_t<T>,
															  std::ranges::range_value_t<T>>;

			template<class... Ts>
			struct value_type_chooser : public std::type_identity<std::tuple<copy_or_reference_type<Ts>...>> {};

			template<class... Ts>
				requires(sizeof...(Ts) == 2)
			struct value_type_chooser<Ts...> : public std::type_identity<std::pair<copy_or_reference_type<Ts>...>> {};

			using internal_iterator_category = std::conditional_t<std::conjunction_v<std::bool_constant<std::ranges::contiguous_range<Rngs>>...>,
																  std::contiguous_iterator_tag,
																  std::conditional_t<std::conjunction_v<std::bool_constant<std::ranges::random_access_range<Rngs>>...>,
																  std::random_access_iterator_tag,
																  std::conditional_t<std::conjunction_v<std::bool_constant<std::ranges::bidirectional_range<Rngs>>...>,
																  std::bidirectional_iterator_tag,
																  std::conditional_t<std::conjunction_v<std::bool_constant<std::ranges::forward_range<Rngs>>...>,
																  std::forward_iterator_tag,
																  std::input_iterator_tag>>>>;
			public:
				using iterator_category = std::conditional_t<std::bool_constant<std::same_as<std::input_iterator_tag>,
															 std::input_iterator_tag,
															 std::conditional_t<std::bool_constant<std::forward_iterator_tag>,
															 std::forward_iterator_tag,
															 std::bidirectional_iterator_tag>>>;
				using difference_type = std::ptrdiff_t;
				using value_type = typename value_type_chooser<Rngs...>::type;
				using pointer = value_type*;
				using reference = value_type&;

				constexpr zip_view_iterator(const zip_view& parent, std::ranges::iterator_t<Rngs>... itrs)
					: parent_{std::addressof(parent)}, itrs_{itrs...}
				{
					store_values();
				}

				constexpr reference operator*()
				{
					return reinterpret_cast<reference>(current);
				}

				constexpr pointer operator->()
				{
					return std::addressof(std::get<value_type>(current));
				}

				constexpr zip_view_iterator& operator++()
				{
					
					std::apply([](auto&... it) { (++it, ...); }, itrs_);
					store_values();
					return *this;
				}

				constexpr zip_view_iterator operator++(int)
				{
					auto tmp = *this;
					++*this;
					return tmp;
				}

				constexpr zip_view_iterator& operator--() requires(std::same_as<internal_iterator_category, std::bidirectional_iterator_tag> ||
																   std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																   std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					std::apply([](auto&... it) { (--it, ...); }, itrs_);
					store_values();
					return *this;
				}

				constexpr zip_view_iterator operator--(int) requires(std::same_as<internal_iterator_category, std::bidirectional_iterator_tag> ||
																     std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																     std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					auto tmp = *this;
					--*this;
					return tmp;
				};

				constexpr zip_view_iterator& operator+=(difference_type n) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																				    std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					plus_equal_helper(std::index_sequence_for<Rngs...>{});
					store_values();
					return *this;
				}

				constexpr zip_view_iterator& operator-=(difference_type n) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
                                                                                    std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					plus_equal_helper(-n, std::index_sequence_for<Rngs...>{});
					store_values();
					return *this;
				}

				[[nodiscard]] friend constexpr zip_view_iterator operator+(const zip_view_iterator& lhs, difference_type rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																	   std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					auto tmp = lhs;
					tmp += rhs;
					return tmp;
				}

				[[nodiscard]] friend constexpr zip_view_iterator operator-(const zip_view_iterator& lhs, difference_type rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																	   std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					auto tmp = lhs;
					tmp -= rhs;
					return tmp;
				}

				[[nodiscard]] friend constexpr difference_type operator-(const zip_view_iterator& lhs, const zip_view_iterator& rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																		      std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					return std::get<0>(lhs.itrs_) - std::get<0>(rhs.iters_);
				}

				[[nodiscard]] friend constexpr bool operator!=(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
				{
					return not_equal_helper(lhs, rhs, std::index_sequence_for<Rngs...>{});
				}

				[[nodiscard]] friend constexpr bool operator<(const zip_view_iterator& lhs, const zip_view_iterator& rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																   std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					return std::get<0>(lhs.iters_) < std::get<0>(rhs.iters_);
				}

				[[nodiscard]] friend constexpr bool operator>(const zip_view_iterator& lhs, const zip_view_iterator& rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																   std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					return rhs < lhs;
				}

				[[nodiscard]] friend constexpr bool operator<=(const zip_view_iterator& lhs, const zip_view_iterator& rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																    std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					return !(rhs < lhs);
				}

				[[nodiscard]] friend constexpr bool operator>=(const zip_view_iterator& lhs, const zip_view_iterator& rhs) requires(std::same_as<internal_iterator_category, std::random_access_iterator_tag> ||
																																    std::same_as<internal_iterator_category, std::contiguous_iterator_tag>)
				{
					return !(lhs < rhs);
				}
			private:
				const zip_view* parent_;
				std::tuple<std::ranges::iterator_t<Rngs>...> itrs_;
				char current[sizeof(value_type)] {0};

				constexpr void store_values()
				{
					store_values_helper(std::index_sequence_for<Rngs...>{});
				}

				template<std::size_t... I>
				constexpr void store_values_helper(std::index_sequence<I...>)
				{
					if ((... && (std::get<I>(itrs_) != std::ranges::end(std::get<I>(parent_->ranges_)))))
					{
						new (current) value_type{*std::get<I>(itrs_)...};
					}
				}

				template<std::size_t... I>
				[[nodiscard]] friend constexpr bool not_equal_helper(const zip_view_iterator& lhs, const zip_view_iterator& rhs, std::index_sequence<I...>)
				{
					return ((std::get<I>(lhs.itrs_) != std::get<I>(rhs.itrs_)) || ...);
				}

				template<std::size_t... I>
				void plus_equal_helper(difference_type n, std::index_sequence<I...>)
				{
					((std::get<I>(itrs_) = std::clamp(std::get<I>(itrs_) + n, std::ranges::begin(std::get<I>(parent_->ranges_)), std::ranges::end(std::get<I>(parent_->ranges_)))), ...);
				}
		};

		public:
			constexpr zip_view(Rngs&&... rngs)
				: ranges_{rngs...} {}

			constexpr auto begin() const
			{
				return begin_helper(std::make_index_sequence<sizeof...(Rngs)>{});
			}

			constexpr auto end() const
			{
				return end_helper(std::make_index_sequence<sizeof...(Rngs)>{});
			}
		private:
			std::tuple<std::ranges::ref_view<std::decay_t<Rngs>>...> ranges_;

			template<std::size_t... I>
			constexpr auto begin_helper(std::index_sequence<I...>) const
			{
				return zip_view_iterator{*this, std::ranges::begin(std::get<I>(ranges_))...};
			}

			template<std::size_t... I>
			constexpr auto end_helper(std::index_sequence<I...>) const
			{
				return zip_view_iterator{*this, std::ranges::end(std::get<I>(ranges_))...};
			}
	};

	export
	template<std::ranges::input_range Rng>
	class zip_view<Rng> : public std::ranges::views::all_t<Rng> {};

	export
	template<std::ranges::input_range... Rngs>
	zip_view(Rngs&&...)->zip_view<Rngs...>;

	namespace views
	{
		export
		template<std::ranges::input_range... Rngs>
		[[nodiscard]] constexpr zip_view<Rngs...> zip(Rngs&&... rngs)
		{
			return zip_view{std::forward<Rngs>(rngs)...};
		}
	}
}