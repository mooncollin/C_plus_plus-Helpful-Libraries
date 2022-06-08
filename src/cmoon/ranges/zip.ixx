export module cmoon.ranges.zip;

import <ranges>;
import <functional>;
import <utility>;
import <iterator>;
import <type_traits>;
import <tuple>;
import <concepts>;
import <algorithm>;
import <compare>;
import <memory>;

import cmoon.tuple;
import cmoon.math;
import cmoon.utility;
import cmoon.concepts;

namespace cmoon::ranges
{
	template<class R>
	concept simple_view = std::ranges::view<R> &&
						  std::ranges::range<const R> &&
						  std::same_as<std::ranges::iterator_t<R>, std::ranges::iterator_t<const R>> &&
						  std::same_as<std::ranges::sentinel_t<R>, std::ranges::sentinel_t<const R>>;

	template<class... Rs>
	concept zip_is_common = (sizeof...(Rs) == 1 && (std::ranges::common_range<Rs>&& ...)) ||
							(!(std::ranges::bidirectional_range<Rs> && ...) && (std::ranges::common_range<Rs> && ...)) ||
							((std::ranges::random_access_range<Rs> && ...) && (std::ranges::sized_range<Rs> && ...));

	template<bool Const, class View>
	struct maybe_const_t : std::type_identity<View> {};

	template<class View>
	struct maybe_const_t<true, View> : std::type_identity<const View> {};

	template<bool Const, class View>
	using maybe_const = typename maybe_const_t<Const, View>::type;

	template<bool C, class... Views>
	concept all_forward =
		(std::ranges::forward_range<maybe_const<C, Views>> && ...);

	template<bool C, class... Views>
	concept all_bidirectional =
		(std::ranges::bidirectional_range<maybe_const<C, Views>> && ...);

	template<bool C, class... Views>
	concept all_random_access =
		(std::ranges::random_access_range<maybe_const<C, Views>> && ...);


	template<bool Const, class...>
	struct iterator_category_chooser {};

	template<bool Const, class... Views>
		requires(all_forward<Const, Views...>)
	struct iterator_category_chooser<Const, Views...>
	{
		using iterator_category = std::input_iterator_tag;
	};

	template<std::copy_constructible F, std::ranges::input_range... Views>
		requires((std::ranges::view<Views> && ...) &&
				 sizeof...(Views) > 0 &&
				 std::is_object_v<F> &&
				 std::regular_invocable<F&, std::ranges::range_reference_t<Views>...> &&
				 can_reference<std::invoke_result_t<F&, std::ranges::range_reference_t<Views>...>>)
	class zip_transform_view;

	export
	template<std::ranges::input_range... Views>
		requires((std::ranges::view<Views> && ...) && (sizeof...(Views) > 0))
	class zip_view : public std::ranges::view_interface<zip_view<Views...>>
	{
		private:
			template<bool Const>
			class iterator : public iterator_category_chooser<Const, Views...>
			{
				friend class zip_view;

				template<std::copy_constructible F, std::ranges::input_range... Views2>
					requires((std::ranges::view<Views2> && ...) &&
							 sizeof...(Views2) > 0 &&
							 std::is_object_v<F> &&
							 std::regular_invocable<F&, std::ranges::range_reference_t<Views2>...> &&
							 can_reference<std::invoke_result_t<F&, std::ranges::range_reference_t<Views2>...>>)
				friend class zip_transform_view;

				template<bool Const2>
				friend class iterator;

				cmoon::tuple_like<std::ranges::iterator_t<maybe_const<Const, Views>>...> current_;

				constexpr iterator(cmoon::tuple_like<std::ranges::iterator_t<maybe_const<Const, Views>>...> current)
					: current_{std::move(current)} {}

				public:
					using iterator_concept = std::conditional_t<
												all_random_access<Const, Views...>,
												std::random_access_iterator_tag,
												std::conditional_t<
													all_bidirectional<Const, Views...>,
													std::bidirectional_iterator_tag,
													std::conditional_t<
														all_forward<Const, Views...>,
														std::forward_iterator_tag,
														std::input_iterator_tag
													>
												>
											 >;
					using value_type = cmoon::tuple_like<std::ranges::range_value_t<maybe_const<Const, Views>>...>;
					using difference_type = std::common_type_t<std::ranges::range_difference_t<maybe_const<Const, Views>>...>;

					constexpr iterator() = default;

					constexpr iterator(iterator<!Const> i)
						requires(Const && (std::convertible_to<std::ranges::iterator_t<Views>, std::ranges::iterator_t<maybe_const<Const, Views>>> && ...))
						: current_{std::move(i.current_)} {}

					constexpr auto operator*() const
					{
						return cmoon::tuple_transform([](auto& i) -> decltype(auto) { return *i; }, current_);
					}

					constexpr iterator& operator++()
					{
						cmoon::tuple_for_each([](auto& i) { ++i; }, current_);
						return *this;
					}

					constexpr void operator++(int)
					{
						++*this;
					}

					constexpr iterator operator++(int)
						requires(all_forward<Const, Views...>)
					{
						auto tmp {*this};
						++*this;
						return tmp;
					}

					constexpr iterator& operator--()
						requires(all_bidirectional<Const, Views...>)
					{
						cmoon::tuple_for_each([](auto& i) { --i; }, current_);
						return *this;
					}

					constexpr iterator operator--(int)
						requires(all_bidirectional<Const, Views...>)
					{
						auto tmp {*this};
						--*this;
						return tmp;
					};

					constexpr iterator& operator+=(difference_type n)
						requires(all_random_access<Const, Views...>)
					{
						cmoon::tuple_for_each([&n]<class I>(I& i) { i += std::iter_difference_t<I>(n); }, current_);
						return *this;
					}

					constexpr iterator& operator-=(difference_type n)
						requires(all_random_access<Const, Views...>)
					{
						cmoon::tuple_for_each([&n]<class I>(I& i) { i -= std::iter_difference_t<I>(n); }, current_);
						return *this;
					}

					constexpr auto operator[](difference_type n) const
						requires(all_random_access<Const, Views...>)
					{
						return cmoon::tuple_transform([&n]<class I>(I& i) -> decltype(auto) {
							return i[std::iter_difference_t<I>(n)];
						}, current_);
					}

					[[nodiscard]] friend constexpr iterator operator+(const iterator& lhs, difference_type rhs)
						requires(all_random_access<Const, Views...>)
					{
						auto tmp{lhs};
						tmp += rhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr iterator operator+(difference_type lhs, const iterator& rhs)
						requires(all_random_access<Const, Views...>)
					{
						auto tmp{rhs};
						tmp += lhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr iterator operator-(const iterator& lhs, difference_type rhs)
						requires(all_random_access<Const, Views...>)
					{
						auto tmp{lhs};
						tmp -= rhs;
						return tmp;
					}

					[[nodiscard]] friend constexpr difference_type operator-(const iterator& lhs, const iterator& rhs)
						requires((std::sized_sentinel_for<std::ranges::iterator_t<maybe_const<Const, Views>>,
														  std::ranges::iterator_t<maybe_const<Const, Views>>> && ...))
					{
						return [&]<std::size_t... I>(std::index_sequence<I...>) {
								constexpr auto abs {[](const auto v) {
									return v < 0 ? -v : v;
								}};

								return std::ranges::min({abs(difference_type(std::get<I>(lhs.current_) - std::get<I>(rhs.current_)))...});
							}(std::index_sequence_for<Views...>{});
					}

					friend constexpr bool operator==(const iterator& lhs, const iterator& rhs)
						requires((std::equality_comparable<std::ranges::iterator_t<maybe_const<Const, Views>>> && ...))
					{
						if constexpr (all_bidirectional<Const, Views...>)
						{
							return lhs.current_ == rhs.current_;
						}
						else
						{
							return [&]<std::size_t... I>(std::index_sequence<I...>) {
								return (bool(std::get<I>(lhs.current_) == std::get<I>(rhs.current_)) || ...);
							}(std::index_sequence_for<Views...>{});
						}
					}

					friend constexpr bool operator!=(const iterator&, const iterator&) = default;

					friend constexpr bool operator<(const iterator& lhs, const iterator& rhs)
						requires(all_random_access<Const, Views...>)
					{
						return lhs.current_ < rhs.current_;
					}

					friend constexpr bool operator>(const iterator& lhs, const iterator& rhs)
						requires(all_random_access<Const, Views...>)
					{
						return rhs < lhs;
					}

					friend constexpr bool operator<=(const iterator& lhs, const iterator& rhs)
						requires(all_random_access<Const, Views...>)
					{
						return !(rhs < lhs);
					}

					friend constexpr bool operator>=(const iterator& lhs, const iterator& rhs)
						requires(all_random_access<Const, Views...>)
					{
						return !(lhs < rhs);
					}

					friend constexpr auto operator<=>(const iterator& lhs, const iterator& rhs)
						requires(all_random_access<Const, Views...> &&
								 (std::three_way_comparable<std::ranges::iterator_t<maybe_const<Const, Views>>> && ...))
					{
						return lhs.current_ <=> rhs.current_;
					}

					friend constexpr auto iter_move(const iterator& i) noexcept((noexcept(std::ranges::iter_move(std::declval<std::ranges::iterator_t<maybe_const<Const, Views>> const&>())) && ...) &&
																				(std::is_nothrow_move_constructible_v<std::ranges::range_rvalue_reference_t<maybe_const<Const, Views>>> && ...))
					{
						return cmoon::tuple_transform(std::ranges::iter_move, i.current_);
					}

					friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs)
						requires((std::indirectly_swappable<std::ranges::iterator_t<maybe_const<Const, Views>>> && ...))
					{
						[&]<std::size_t... I>(std::index_sequence<I...>) {
							(std::ranges::iter_swap(std::get<I>(lhs.current_), std::get<I>(rhs.current_)), ...);
						}(std::index_sequence_for<Views...>{});
					}
			};

			template<bool Const>
			class sentinel
			{
				friend class zip_view;

				cmoon::tuple_like<std::ranges::sentinel_t<maybe_const<Const, Views>>...> end_;

				constexpr explicit sentinel(cmoon::tuple_like<std::ranges::sentinel_t<maybe_const<Const, Views>>...> end)
					: end_{end} {}

				public:
					sentinel() = default;

					sentinel(sentinel<!Const> i)
						requires(Const && (std::convertible_to<std::ranges::sentinel_t<Views>, std::ranges::sentinel_t<maybe_const<Const, Views>>> && ...))
						: end_{std::move(i.end_)} {}

					template<bool OtherConst>
						requires((std::sentinel_for<std::ranges::sentinel_t<maybe_const<Const, Views>>, std::ranges::iterator_t<maybe_const<OtherConst, Views>>> && ...))
					friend constexpr bool operator==(const iterator<OtherConst>& lhs, const sentinel& rhs)
					{
						return[&]<std::size_t... I>(std::index_sequence<I...>) {
							return (bool(std::get<I>(lhs.current_) == std::get<I>(rhs.end_)) || ...);
						}(std::index_sequence_for<Views...>{});
					}

					template<bool OtherConst>
					friend constexpr bool operator!=(const iterator<OtherConst>& lhs, const sentinel& rhs)
					{
						return !(lhs == rhs);
					}

					template<bool OtherConst>
						requires((std::sized_sentinel_for<std::ranges::sentinel_t<maybe_const<Const, Views>>,
														 std::ranges::iterator_t<maybe_const<OtherConst, Views>>> && ...))
					friend constexpr std::common_type_t<std::ranges::range_difference_t<maybe_const<OtherConst, Views>>...>
						operator-(const iterator<OtherConst>& lhs, const sentinel& rhs)
					{
						using d = std::common_type_t<std::ranges::range_difference_t<maybe_const<OtherConst, Views>>...>;

						return [&]<std::size_t... I>(std::index_sequence<I...>) {
								constexpr auto abs {[](const auto v) {
									return v < 0 ? -v : v;
								}};

								return std::ranges::min({abs(d(std::get<I>(lhs.current_) - std::get<I>(rhs.current_)))...});
							}(std::index_sequence_for<Views...>{});
					}

					template<bool OtherConst>
						requires((std::sized_sentinel_for<std::ranges::sentinel_t<maybe_const<Const, Views>>,
														 std::ranges::iterator_t<maybe_const<OtherConst, Views>>> && ...))
					friend constexpr std::common_type_t<std::ranges::range_difference_t<maybe_const<OtherConst, Views>>...>
						operator-(const sentinel& lhs, const iterator<OtherConst>& rhs)
					{
						return -(rhs - lhs);
					}
			};
		public:
			constexpr zip_view() requires(std::conjunction_v<std::default_initializable<Views>...>) = default;

			constexpr explicit zip_view(Views... views)
				: views_{std::move(views)...} {}

			[[nodiscard]] constexpr auto begin()
				requires(!(simple_view<Views> && ...))
			{
				return iterator<false>{cmoon::tuple_transform(std::ranges::begin, views_)};
			}

			[[nodiscard]] constexpr auto begin() const
				requires((std::ranges::range<const Views> && ...))
			{
				return iterator<true>{cmoon::tuple_transform(std::ranges::begin, views_)};
			}

			[[nodiscard]] constexpr auto end()	
				requires(!(simple_view<Views> && ...))
			{
				if constexpr (!zip_is_common<Views...>)
				{
					return sentinel<false>{cmoon::tuple_transform(std::ranges::end, views_)};
				}
				else if constexpr ((std::ranges::random_access_range<Views> && ...))
				{
					return begin() + std::iter_difference_t<iterator<false>>(size());
				}
				else
				{
					return iterator<false>{cmoon::tuple_transform(std::ranges::end, views_)};
				}
			}

			[[nodiscard]] constexpr auto end() const
				requires((std::ranges::range<const Views> && ...))
			{
				if constexpr (!zip_is_common<Views...>)
				{
					return sentinel<true>{cmoon::tuple_transform(std::ranges::end, views_)};
				}
				else if constexpr ((std::ranges::random_access_range<Views> && ...))
				{
					return begin() + std::iter_difference_t<iterator<true>>(size());
				}
				else
				{
					return iterator<true>{cmoon::tuple_transform(std::ranges::end, views_)};
				}
			}

			[[nodiscard]] constexpr auto size()
				requires((std::ranges::sized_range<Views> && ...))
			{
				return std::apply(
							[](auto... sizes) {
								using CT = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
								return std::ranges::min({CT(sizes)...});
							}, cmoon::tuple_transform(std::ranges::size, views_));
			}

			[[nodiscard]] constexpr auto size() const
				requires((std::ranges::sized_range<const Views> && ...))
			{
				return std::apply(
							[](auto... sizes) {
								using CT = std::make_unsigned_t<std::common_type_t<decltype(sizes)...>>;
								return std::ranges::min({CT(sizes)...});
							}, cmoon::tuple_transform(std::ranges::size, views_));
			}
		private:
			std::tuple<Views...> views_;
	};

	template<class... Rs>
	zip_view(Rs&&...) -> zip_view<std::views::all_t<Rs>...>;

	namespace views
	{
		export
		constexpr struct zip_fn
		{
			template<std::ranges::input_range... Views>
			[[nodiscard]] constexpr auto operator()(Views&&... views) const
			{
				return zip_view<std::views::all_t<Views>...>{std::forward<Views>(views)...};
			}

			[[nodiscard]] constexpr std::ranges::empty_view<std::tuple<>> operator()() const noexcept
			{
				return {};
			}
		} zip {};
	}

	/*template<class F, bool Const, class...>
	struct iterator_category_chooser2 {};

	template<class F, bool Const, class... Views>
		requires(std::ranges::forward_range<maybe_const<Const, zip_view<Views...>>>)
	struct iterator_category_chooser2<F, Const, Views...>
	{
		using iterator_category = std::conditional_t<
			!std::is_lvalue_reference_v<std::invoke_result_t<maybe_const<Const, F>&, std::ranges::range_reference_t<maybe_const<Const, Views>>...>>,
			std::input_iterator_tag,
			std::common_type_t<typename std::iterator_traits<std::ranges::iterator_t<maybe_const<Const, Views>>>::iterator_category...>
		>;
	};

	template<class T>
	concept can_reference = cmoon::complete_type<T&>;

	template<std::copy_constructible F, std::ranges::input_range... Views>
		requires((std::ranges::view<Views> && ...) &&
				 sizeof...(Views) > 0 &&
				 std::is_object_v<F> &&
				 std::regular_invocable<F&, std::ranges::range_reference_t<Views>...> &&
				 can_reference<std::invoke_result_t<F&, std::ranges::range_reference_t<Views>...>>)
	class zip_transform_view : public std::ranges::view_interface<zip_transform_view<F, Views...>>
	{
		cmoon::copyable_box<F> fun_;
		zip_view<Views...> zip_;

		using InnerView = zip_view<Views...>;

		template<bool Const>
		using ziperator = std::ranges::iterator_t<maybe_const<Const, InnerView>>;

		template<bool Const>
		using zentinel = std::ranges::sentinel_t<maybe_const<Const, InnerView>>;

		template<bool Const>
		class iterator : public iterator_category_chooser2<F, Const, Views...>
		{
			friend class zip_transform_view;

			using Parent = maybe_const<Const, zip_transform_view>;
			using Base = maybe_const<Const, InnerView>;

			Parent* parent_ {nullptr};
			ziperator<Const> inner_;

			constexpr iterator(Parent& parent, ziperator<Const> inner)
				: parent_{std::addressof(parent)}, inner_{std::move(inner)} {}

			public:
				using iterator_concept = typename ziperator<Const>::iterator_concept;
				using value_type = std::remove_cvref_t<std::invoke_result_t<maybe_const<Const, F>&, std::ranges::range_reference_t<maybe_const<Const, Views>>...>>;
				using difference_type = std::ranges::range_difference_t<Base>;

				constexpr iterator() = default;

				constexpr iterator(iterator<!Const> i) requires(Const && std::convertible_to<ziperator<false>, ziperator<Const>>)
					: parent_{i.parent_}, inner_{std::move(i.inner_)} {}

				constexpr decltype(auto) operator*() const
				{
					return std::apply([&](const auto&... iters) -> decltype(auto) {
						return std::invoke(*parent_->fun_, *iters...);
					}, inner_.current_);
				}

				constexpr iterator& operator++()
				{
					++inner_;
					return *this;
				}

				constexpr void operator++(int)
				{
					++*this;
				}

				constexpr iterator operator++(int) requires(std::ranges::forward_range<Base>)
				{
					auto tmp {*this};
					++*this;
					return tmp;
				}

				constexpr iterator& operator--() requires(std::ranges::bidirectional_range<Base>)
				{
					--inner_;
					return *this;
				}

				constexpr iterator operator--(int) requires(std::ranges::bidirectional_range<Base>)
				{
					auto tmp {*this};
					--*this;
					return tmp;
				}

				constexpr iterator& operator+=(difference_type x) requires(std::ranges::random_access_range<Base>)
				{
					inner_ += x;
					return *this;
				}

				constexpr iterator& operator-=(difference_type x) requires(std::ranges::random_access_range<Base>)
				{
					inner_ -= x;
					return *this;
				}

				constexpr decltype(auto) operator[](difference_type n) const requires(std::ranges::random_access_range<Base>)
				{
					return std::apply([&]<class... Is>(const Is&... iters) -> decltype(auto) {
						return std::invoke(*parent_->fun_, iters[std::iter_difference_t<Is>(n)]...);
					}, inner_.current_);
				}

				friend constexpr bool operator==(const iterator& x, const iterator& y) requires(std::equality_comparable<ziperator<Const>>)
				{
					return x.inner_ == y.inner_;
				}

				friend constexpr bool operator<(const iterator& x, const iterator& y) requires(std::ranges::random_access_range<Base>)
				{
					return x.inner_ < y.inner_;
				}

				friend constexpr bool operator>(const iterator& x, const iterator& y) requires(std::ranges::random_access_range<Base>)
				{
					return x.inner_ > y.inner_;
				}

				friend constexpr bool operator<=(const iterator& x, const iterator& y) requires(std::ranges::random_access_range<Base>)
				{
					return x.inner_ <= y.inner_;
				}

				friend constexpr bool operator>=(const iterator& x, const iterator& y) requires(std::ranges::random_access_range<Base>)
				{
					return x.inner_ >= y.inner_;
				}

				friend constexpr auto operator<=>(const iterator& x, const iterator& y) requires(std::ranges::random_access_range<Base> && std::three_way_comparable<ziperator<Const>>)
				{
					return x.inner_ <=> y.inner_;
				}

				friend constexpr iterator operator+(const iterator& i, difference_type n) requires(std::ranges::random_access_range<Base>)
				{
					return iterator{*i.parent_, i.inner_ + n};
				}

				friend constexpr iterator operator+(difference_type n, const iterator& i) requires(std::ranges::random_access_range<Base>)
				{
					return iterator{*i.parent_, i.inner_ + n}; 
				}

				friend constexpr iterator operator-(const iterator& i, difference_type n) requires(std::ranges::random_access_range<Base>)
				{
					return iterator{*i.parent_, i.inner_ - n};
				}

				friend constexpr difference_type operator-(const iterator& x, const iterator& y) requires(std::sized_sentinel_for<ziperator<Const>, ziperator<Const>>)
				{
					return x.inner_ - y.inner_;
				}

				friend constexpr void iter_swap(const iterator& lhs, const iterator& rhs)
				{
					std::ranges::iter_swap(lhs.inner_, rhs.inner_);
				}
		};

		template<bool Const>
		class sentinel
		{
			zentinel<Const> inner_;

			constexpr explicit sentinel(zentinel<Const> inner)
				: inner_{inner} {}

			public:
				constexpr sentinel() = default;

				constexpr sentinel(sentinel<!Const> i) requires(Const && std::convertible_to<zentinel<false>, zentinel<Const>>)
					: inner_{std::move(i.inner_)} {}

				template<bool OtherConst>
					requires(std::sentinel_for<zentinel<Const>, ziperator<OtherConst>>)
				friend constexpr bool operator==(const iterator<OtherConst>& x, const sentinel& y)
				{
					return x.inner_ == y.inner_;
				}

				template<bool OtherConst>
					requires(std::sized_sentinel_for<zentinel<Const>, ziperator<OtherConst>>)
				friend constexpr std::ranges::range_difference_t<maybe_const<OtherConst, InnerView>>
				operator-(const iterator<OtherConst>& x, const sentinel& y)
				{
					return x.inner_ - y.inner_;
				}

				template<bool OtherConst>
					requires(std::sized_sentinel_for<zentinel<Const>, ziperator<OtherConst>>)
				friend constexpr std::ranges::range_difference_t<maybe_const<OtherConst, InnerView>>
				operator-(const sentinel& x, const iterator<OtherConst>& y)
				{
					return x.inner_ - y.inner_;
				}
		};

		public:
			constexpr zip_transform_view() = default;

			constexpr zip_transform_view(const zip_transform_view& other)
				: fun_{other.fun_}, zip_{other.zip_} {}

			constexpr zip_transform_view(zip_transform_view&& other)
				: fun_{std::move(other.fun_)}, zip_{std::move(other.zip_)} {}

			constexpr explicit zip_transform_view(F fun, Views... views)
				: fun_{std::move(fun)}, zip_{std::move(views)...} {}

			constexpr auto begin()
			{
				return iterator<false>{*this, zip_.begin()};
			}

			constexpr auto begin() const
				requires(std::ranges::range<const InnerView> &&
						 std::regular_invocable<const F&, std::ranges::range_reference_t<const Views>...>)
			{
				return iterator<true>{*this, zip_.begin()};
			}

			constexpr auto end()
			{
				if constexpr (std::ranges::common_range<InnerView>)
				{
					return iterator<false>{*this, zip_.end()};
				}
				else
				{
					return sentinel<false>{zip_.end()};
				}
			}

			constexpr auto end() const
				requires(std::ranges::range<const InnerView> &&
						 std::regular_invocable<const F&, std::ranges::range_reference_t<const Views>...>)
			{
				if constexpr (std::ranges::common_range<const InnerView>)
				{
					return iterator<true>{*this, zip_.end()};
				}
				else
				{
					return sentinel<true>{zip_.end()};
				}
			}

			constexpr auto size() requires(std::ranges::sized_range<InnerView>)
			{
				return zip_.size();
			}

			constexpr auto size() const requires(std::ranges::sized_range<const InnerView>)
			{
				return zip_.size();
			}
	};

	template<class F, class... Rs>
	zip_transform_view(F, Rs&&...) -> zip_transform_view<F, std::views::all_t<Rs>...>;

	namespace views
	{
		export
		constexpr struct zip_transform_fn
		{
			template<class F, std::ranges::input_range... Views>
				requires(std::invocable<F, std::ranges::range_reference_t<Views>...>)
			constexpr auto operator()(F&& f, Views&&... views) const
			{
				return zip_transform_view<F, std::views::all_t<Views>...>{std::forward<F>(f), std::forward<Views>(views)...};
			}

			template<class F, class FD = std::decay_t<decltype((F))>>
				requires((std::copy_constructible<FD> && std::regular_invocable<FD&>) &&
						 std::is_object_v<std::decay_t<std::invoke_result_t<FD&>>>)
			constexpr auto operator()(F&&) const
			{
				return cmoon::decay_copy(std::views::empty<std::decay_t<std::invoke_result_t<FD&>>>);
			}
		} zip_transform {};
	}*/
}