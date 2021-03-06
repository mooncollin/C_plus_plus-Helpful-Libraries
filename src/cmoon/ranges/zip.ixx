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
	template<std::ranges::range R>
	struct ref_or_value : public std::type_identity<std::remove_cvref_t<R>> {};

	template<std::ranges::range R>
		requires(std::is_lvalue_reference_v<R> && std::ranges::borrowed_range<R>)
	struct ref_or_value<R> : public std::type_identity<std::ranges::ref_view<std::remove_reference_t<R>>> {};

	export
	template<std::ranges::range R>
	using ref_or_value_t = typename ref_or_value<R>::type;

	export
	template<std::ranges::range... Rngs>
	constexpr bool can_all_be_views = (std::ranges::view<ref_or_value_t<Rngs>> && ...);

	struct empty {};

	template<typename T>
	using copy_or_reference_type = std::conditional_t<std::ranges::input_range<T> || !std::ranges::borrowed_range<T>,
														std::ranges::range_value_t<T>,
														std::ranges::range_reference_t<T>>;

	template<class... Ts>
	struct value_type_chooser : public std::type_identity<std::tuple<copy_or_reference_type<Ts>...>> {};

	template<class T1, class T2>
	struct value_type_chooser<T1, T2> : public std::type_identity<std::pair<copy_or_reference_type<T1>, copy_or_reference_type<T2>>> {};

	template<class T1>
	struct value_type_chooser<T1> : public std::type_identity<copy_or_reference_type<T1>> {};

	template<class T>
	struct iterator_category_chooser : public std::type_identity<std::input_iterator_tag> {};

	template<class T>
		requires(requires { typename std::iterator_traits<T>::iterator_category; })
	struct iterator_category_chooser<T> : public std::type_identity<typename std::iterator_traits<T>::iterator_category> {};

	template<class T>
	using iterator_category_chooser_t = typename iterator_category_chooser<T>::type;

	export
	template<std::ranges::range... Rngs>
	class zip_view : public std::conditional_t<can_all_be_views<Rngs...>, std::ranges::view_interface<zip_view<Rngs...>>, empty>
	{
		private:
			template<class... Its>
			class zip_view_iterator
			{
				private:
					static constexpr auto ranges_index_sequence = std::index_sequence_for<Its...>{};
					using internal_iterator_category = std::common_type_t<iterator_category_chooser_t<Its>...>;
				public:
					using iterator_category = std::common_type_t<std::bidirectional_iterator_tag, internal_iterator_category>;

					using difference_type = std::ptrdiff_t;
					using value_type = typename value_type_chooser<ref_or_value_t<Rngs>...>::type;
					using pointer = void;
					using reference = value_type;

					constexpr zip_view_iterator() noexcept = default;

					constexpr zip_view_iterator(Its... itrs)
						: itrs_{itrs...} {}

					constexpr reference operator*()
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
						requires(std::derived_from<internal_iterator_category, std::bidirectional_iterator_tag>)
					{
						std::apply([](auto&... it) { (--it, ...); }, itrs_);
						return *this;
					}

					constexpr zip_view_iterator operator--(int)
						requires(std::derived_from<internal_iterator_category, std::bidirectional_iterator_tag>)
					{
						auto tmp = *this;
						--*this;
						return tmp;
					};

					constexpr zip_view_iterator& operator+=(difference_type n)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						std::apply([n](auto&... it) { ((it += n), ...); }, itrs_);
						return *this;
					}

					constexpr zip_view_iterator& operator-=(difference_type n)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						std::apply([n](auto&... it) { ((it -= n), ...); }, itrs_);
						return *this;
					}

					[[nodiscard]] friend constexpr zip_view_iterator operator+(const zip_view_iterator& lhs, difference_type rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						auto tmp = lhs;
						tmp += rhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr zip_view_iterator operator-(const zip_view_iterator& lhs, difference_type rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						auto tmp = lhs;
						tmp -= rhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr difference_type operator-(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						return std::get<0>(lhs.itrs_) - std::get<0>(rhs.iters_);
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

					[[nodiscard]] friend constexpr bool operator<(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						return std::get<0>(lhs.iters_) < std::get<0>(rhs.iters_);
					}

					[[nodiscard]] friend constexpr bool operator>(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						return rhs < lhs;
					}

					[[nodiscard]] friend constexpr bool operator<=(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						return !(rhs < lhs);
					}

					[[nodiscard]] friend constexpr bool operator>=(const zip_view_iterator& lhs, const zip_view_iterator& rhs)
						requires(std::derived_from<internal_iterator_category, std::random_access_iterator_tag>)
					{
						return !(lhs < rhs);
					}
				private:
					std::tuple<Its...> itrs_;

					template<class... OtherIts, std::size_t... I>
					[[nodiscard]] friend constexpr bool equal_helper(const zip_view_iterator& lhs, const zip_view_iterator<OtherIts...>& rhs, std::index_sequence<I...>)
					{
						return ((std::get<I>(lhs.itrs_) == std::get<I>(rhs.itrs_)) || ...);
					}

					template<class... OtherIts>
					friend class zip_view_iterator;
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
				requires((std::ranges::sized_range<Rngs> && ...))
			{
				return std::apply([](const auto&... rngs) { return cmoon::min(std::ranges::size(rngs)...); }, ranges_);
			}
		private:
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

	// TODO: MSVC gives C1001 here
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
	template<std::ranges::range... Rngs>
	zip_view(Rngs&&...)->zip_view<Rngs...>;

	namespace views
	{
		export
		template<std::ranges::range... Rngs>
		[[nodiscard]] constexpr zip_view<Rngs...> zip(Rngs&&... rngs)
		{
			return zip_view{std::forward<Rngs>(rngs)...};
		}
	}
}