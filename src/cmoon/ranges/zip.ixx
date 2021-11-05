export module cmoon.ranges.zip;

import <ranges>;
import <functional>;
import <utility>;
import <iterator>;
import <type_traits>;
import <tuple>;
import <concepts>;
import <algorithm>;

import cmoon.algorithm;

namespace cmoon::ranges
{
	template<std::ranges::input_range R>
	struct ref_or_value : public std::type_identity<std::remove_cvref_t<R>> {};

	template<std::ranges::input_range R>
		requires(std::is_lvalue_reference_v<R> && std::ranges::borrowed_range<R>)
	struct ref_or_value<R> : public std::type_identity<std::ranges::ref_view<std::remove_reference_t<R>>> {};

	export
	template<std::ranges::input_range R>
	using ref_or_value_t = typename ref_or_value<R>::type;

	export
	template<std::ranges::input_range... Rngs>
	constexpr bool can_all_be_views = (std::ranges::view<ref_or_value_t<Rngs>> && ...);

	struct empty {};

	template<typename T>
	using copy_or_reference_type = std::conditional_t<std::ranges::input_range<T>,
														std::ranges::range_value_t<T>,
														std::ranges::range_reference_t<T>>;

	template<class T>
	struct iterator_category_chooser : public std::type_identity<std::input_iterator_tag> {};

	template<class T>
		requires(requires { typename std::iterator_traits<T>::iterator_category; })
	struct iterator_category_chooser<T> : public std::type_identity<typename std::iterator_traits<T>::iterator_category> {};

	template<class T>
	using iterator_category_chooser_t = typename iterator_category_chooser<T>::type;

	export
	template<std::ranges::input_range... Rngs>
	class zip_view : public std::conditional_t<can_all_be_views<Rngs...>, std::ranges::view_interface<zip_view<Rngs...>>, empty>
	{
		private:
			template<class... Its>
			class zip_view_iterator
			{
				template<class... OtherIts>
				friend class zip_view_iterator;

				private:
					static constexpr auto ranges_index_sequence = std::index_sequence_for<Its...>{};
				public:
					using iterator_category = std::common_type_t<iterator_category_chooser_t<Its>...>;

					using difference_type = std::ptrdiff_t;
					using value_type = std::tuple<copy_or_reference_type<ref_or_value_t<Rngs>>...>;
					using pointer = void;
					using reference = value_type;

					constexpr zip_view_iterator() noexcept = default;

					constexpr zip_view_iterator(Its... itrs)
						: itrs_{itrs...} {}

					constexpr reference operator*() const
					{
						return std::apply([](auto&... it) { return value_type{*it...}; }, itrs_);
					}

					constexpr zip_view_iterator& operator++()
					{
					
						std::apply([](auto&... it) { (++it, ...); }, itrs_);
						return *this;
					}

					constexpr zip_view_iterator operator++(int)
					{
						auto tmp = *this;
						++*this;
						return tmp;
					}

					constexpr zip_view_iterator& operator--()
						requires(std::derived_from<iterator_category, std::bidirectional_iterator_tag>)
					{
						std::apply([](auto&... it) { (--it, ...); }, itrs_);
						return *this;
					}

					constexpr zip_view_iterator operator--(int)
						requires(std::derived_from<iterator_category, std::bidirectional_iterator_tag>)
					{
						auto tmp = *this;
						--*this;
						return tmp;
					};

					constexpr zip_view_iterator& operator+=(difference_type n)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						std::apply([n](auto&... it) { ((it += n), ...); }, itrs_);
						return *this;
					}

					constexpr zip_view_iterator& operator-=(difference_type n)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						std::apply([n](auto&... it) { ((it -= n), ...); }, itrs_);
						return *this;
					}

					[[nodiscard]] friend constexpr zip_view_iterator operator+(const zip_view_iterator& lhs, difference_type rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						auto tmp = lhs;
						tmp += rhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr zip_view_iterator operator+(difference_type lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						auto tmp = rhs;
						tmp += lhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr zip_view_iterator operator-(const zip_view_iterator& lhs, difference_type rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						auto tmp = lhs;
						tmp -= rhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr difference_type operator-(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						return std::get<0>(lhs.itrs_) - std::get<0>(rhs.iters_);
					}

					constexpr reference operator[](difference_type idx) const
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						return std::apply([idx](auto&... it) { return value_type{ *(it + idx)... }; }, itrs_);
					}

					template<class... OtherIts>
					[[nodiscard]] friend constexpr bool operator==(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs)
					{
						return equal_helper(lhs, rhs, ranges_index_sequence);
					}

					template<class... OtherIts>
					[[nodiscard]] friend constexpr bool operator!=(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs)
					{
						return !equal_helper(lhs, rhs, ranges_index_sequence);
					}

					template<class... OtherIts>
					[[nodiscard]] friend constexpr bool operator<(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						return std::get<0>(lhs.itrs_) < std::get<0>(rhs.itrs_);
					}

					template<class... OtherIts>
					[[nodiscard]] friend constexpr bool operator>(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						return rhs < lhs;
					}

					template<class... OtherIts>
					[[nodiscard]] friend constexpr bool operator<=(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						return !(rhs < lhs);
					}

					template<class... OtherIts>
					[[nodiscard]] friend constexpr bool operator>=(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs)
						requires(std::derived_from<iterator_category, std::random_access_iterator_tag>)
					{
						return !(lhs < rhs);
					}
				private:
					std::tuple<Its...> itrs_;

					template<class... OtherIts, std::size_t... I>
					[[nodiscard]] static constexpr bool equal_helper(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs, std::index_sequence<I...>)
					{
						return ((std::get<I>(lhs.itrs_) == std::get<I>(rhs.itrs_)) || ...);
					}
			};
		public:
			constexpr zip_view() noexcept
				requires(can_all_be_views<Rngs...>) = default;

			constexpr zip_view(Rngs&&... rngs)
				: ranges_{std::forward<Rngs>(rngs)...} {}

			[[nodiscard]] constexpr auto begin()
			{
				return std::apply([](auto&... rngs) { return zip_view_iterator{std::ranges::begin(rngs)...}; }, ranges_);
			}

			[[nodiscard]] constexpr auto begin() const
			{
				return std::apply([](const auto&... rngs) { return zip_view_iterator{std::ranges::begin(rngs)...}; }, ranges_);
			}

			[[nodiscard]] constexpr auto end()
			{
				return std::apply([](auto&... rngs) { return zip_view_iterator{std::ranges::end(rngs)...}; }, ranges_);
			}

			[[nodiscard]] constexpr auto end() const
			{
				return std::apply([](const auto&... rngs) { return zip_view_iterator{std::ranges::end(rngs)...}; }, ranges_);
			}

			[[nodiscard]] constexpr auto empty() const
				requires(requires(const Rngs&... rngs){ (... || std::ranges::empty(rngs)); })
			{
				return std::apply([](const auto&... rngs) { return (... || std::ranges::empty(rngs)); }, ranges_);
			}

			[[nodiscard]] constexpr operator bool() const
				requires(requires(const Rngs&... rngs) { (... || std::ranges::empty(rngs)); })
			{
				return !empty();
			}

			[[nodiscard]] constexpr auto size() const
				requires((std::ranges::sized_range<Rngs> || ...))
			{
				return std::apply([this](const auto&... rngs) { return size_helper(rngs...); }, ranges_);
			}

			constexpr auto front()
			{
				return *begin();
			}

			constexpr auto front() const
			{
				return *begin();
			}

			constexpr auto back()
				requires(std::bidirectional_iterator<decltype(begin())>)
			{
				return *(--end());
			}

			constexpr auto back() const
				requires(std::bidirectional_iterator<decltype(begin())>)
			{
				return *(--end());
			}

			constexpr auto operator[](std::size_t idx)
				// MSVC will disallow this function with the below
				// constraint, even though the static_assert confirms
				// it.
				//requires(std::random_access_iterator<decltype(begin())>)
			{
				static_assert(std::random_access_iterator<decltype(begin())>);
				return begin()[idx];
			}

			constexpr auto operator[](std::size_t idx) const
				// MSVC will disallow this function with the below
				// constraint, even though the static_assert confirms
				// it.
				//requires(std::random_access_iterator<decltype(begin())>)
			{
				static_assert(std::random_access_iterator<decltype(begin())>);
				return begin()[idx];
			}
		private:
			template<class R1, class... RN>
			[[nodiscard]] constexpr auto size_helper(const R1& r1, const RN&... rn) const
			{
				if constexpr (std::ranges::sized_range<R1>)
				{
					using can_get_more_sizes = decltype(size_helper(rn...));
					if constexpr (std::is_void_v<can_get_more_sizes>)
					{
						return std::ranges::size(r1);
					}
					else
					{
						return std::min(std::ranges::size(r1), size_helper(rn...));
					}
				}
				else
				{
					return size_helper(rn...);
				}
			}

			[[nodiscard]] constexpr void size_helper() const {}

			std::tuple<ref_or_value_t<Rngs>...> ranges_;
	};

	export
	template<>
	class zip_view<> : public std::ranges::empty_view<empty> {};

	template<class T>
	consteval void is_empty_view_impl(const std::ranges::empty_view<T>&) noexcept
	{
	}

	template<class T>
	struct is_empty_view : public std::false_type {};

	template<std::ranges::view T>
		requires(requires(T t) { is_empty_view_impl(t); })
	struct is_empty_view<T> : public std::true_type {};

	 //TODO: MSVC gives C1001 here
	//export
	//template<std::ranges::range... Rngs>
	//	requires((is_empty_view<Rngs>::value || ...))
	//class zip_view<Rngs...> : public std::ranges::empty_view<empty>
	//{
	//	public:
	//		constexpr zip_view() noexcept = default;
	//		constexpr zip_view(Rngs&&...) noexcept {}
	//};

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

		export
		template<class F, std::ranges::input_range... Rngs>
			requires(std::invocable<F, std::ranges::range_reference_t<Rngs>...>)
		[[nodiscard]] constexpr auto zip_transform(F&& f, Rngs&&... rngs)
		{
			return zip(std::forward<Rngs>(rngs)...) |
				std::ranges::views::transform([f = std::forward<F>(f)](auto&& t) {
					return std::apply(f, std::forward<decltype(t)>(t));
			});
		}
	}
}