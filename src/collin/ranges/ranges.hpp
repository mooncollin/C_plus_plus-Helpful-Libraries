#pragma once

#include <initializer_list>
#include <type_traits>

#include "collin/type_traits.hpp"
#include "collin/iterator.hpp"
#include "collin/ranges/iterator.hpp"

namespace collin
{
	namespace ranges
	{
		template<class I>
		constexpr auto iter_move(I& i) noexcept
		{
			return static_cast<decltype(*i)&&>(*i);
		}

		template<class T>
		constexpr auto begin(T&& t)
		{

		}

		template<class T>
		constexpr auto cbegin(T&& t)
		{

		}

		template<class T>
		constexpr auto end(T&& t)
		{

		}

		template<class T>
		constexpr auto cend(T&& t)
		{

		}

		template<class T>
		constexpr auto rbegin(T&& t)
		{

		}

		template<class T>
		constexpr auto crbegin(T&& t)
		{

		}

		template<class T>
		constexpr auto rend(T&& t)
		{

		}

		template<class T>
		constexpr auto crend(T&& t)
		{

		}

		template<class T>
		constexpr auto size(T&& t)
		{

		}

		template<class T>
		constexpr auto ssize(T&& t)
		{

		}

		template<class T>
		constexpr auto empty(T&& t)
		{

		}

		template<class T>
		constexpr auto data(T&& t)
		{

		}

		template<class T>
		constexpr auto cdata(T&& t)
		{

		}

		struct dangling
		{
			constexpr dangling() noexcept = default;
			
			template<class... Args>
			constexpr dangling(Args&&...) {};
		};

		struct unreachable_sentinel_t 
		{
			template<iterators::weakly_incrementable I>
			friend constexpr bool operator==(unreachable_sentinel_t, const I&) noexcept { return false; }
		};

		constexpr unreachable_sentinel_t unreachable_sentinel{};

		template<class T>
		concept range =
			requires(T& t)
		{
			ranges::begin(t);
			ranges::end(t);
		};

		template<class T>
		concept sized_range = range<T> &&
			requires(T& t)
		{
			ranges::size(t);
		};

		template<class>
		constexpr bool disable_sized_range = false;

		template<class T>
		using iterator_t = decltype(ranges::begin(std::declval<T&>()));

		template<range R>
		using sentinel_t = decltype(ranges::end(std::declval<R&>()));

		template<sized_range R>
		using range_size_t = decltype(ranges::size(std::declval<R&>()));

		template<range R>
		using range_difference_t = iterators::iter_difference_t<iterator_t<R>>;

		template<range R>
		using range_value_t = iterators::iter_value_t<iterator_t<R>>;

		template<range R>
		using range_reference_t = iterators::iter_reference_t<iterator_t<R>>;

		template<range R>
		using range_rvalue_reference_t = iterators::iter_rvalue_reference_t<iterator_t<R>>;

		template<class S, class I>
		concept sentinel_for = collin::concepts::semi_regular<S> &&
							   collin::iterators::input_or_output_iterator<I> &&
							   collin::concepts::weakly_equality_comparable_with<S, I>;

		template<class S, class I>
		constexpr bool disable_sized_sentinel_for = false;

		template<class S, class I>
		concept sized_sentinel_for = sentinel_for<S, I> &&
									 !disable_sized_sentinel_for<std::remove_cv_t<S>, std::remove_cv_t<I>> &&
			requires(const I& i, const S& s)
		{
			{ s - i } -> concepts::same<collin::iterators::iter_difference_t<I>>;
			{ i - s } -> concepts::same<collin::iterators::iter_difference_t<I>>;
		};

		struct view_base {};

		template<class T>
		constexpr bool enable_view = concepts::derived_from<T, view_base>;

		template<class T>
		concept view = range<T> && concepts::semi_regular<T> && enable_view<T>;

		template<class R>
		constexpr bool enable_borrowed_range = false;

		template<class R>
		concept borrowed_range = range<R> && (std::is_lvalue_reference_v<R> || enable_borrowed_range<std::remove_cv_t<R>>);

		template<range R>
		using borrowed_iterator_t = std::conditional_t<borrowed_range<R>, iterator_t<R>, dangling>;

		template<class T>
		concept input_range = range<T> && iterators::input_iterator<iterator_t<T>>;

		template<class R, class T>
		concept output_range = range<R> && iterators::output_iterator<iterator_t<R>, T>;

		template<class T>
		concept forward_range = input_range<T> && iterators::forward_iterator<iterator_t<T>>;

		template<class T>
		concept bidirectional_range = forward_range<T> && iterators::bidirectional_iterator<iterator_t<T>>;

		template<class T>
		concept random_access_range = bidirectional_range<T> && iterators::random_access_iterator<iterator_t<T>>;

		template<class T>
		concept contiguous_range = random_access_range<T> && iterators::contiguous_iterator<iterator_t<T>> &&
			requires(T& t)
		{
			{ ranges::data(t) } -> concepts::same<std::add_pointer_t<range_reference_t<T>>>;
		};

		template<class T>
		concept common_range = range<T> && concepts::same<iterator_t<T>, sentinel_t<T>>;

		template<class T>
		concept viewable_range = range<T> && (borrowed_range<T> || view<std::remove_cvref_t<T>>);

		template<class D>
			requires(std::is_class_v<D> && concepts::same<D, std::remove_cv_t<D>>)
		class view_interface : public view_base
		{
			constexpr const D& derived() const noexcept
			{
				return static_cast<const D&>(*this);
			}

			public:
				// The below might be commented because MSVC cannot handle it and will refuse to do Intellisense on the entire project

				//constexpr bool empty() const requires(forward_range<const D>);
				//constexpr explicit operator bool() const requires requires { ranges::empty(derived()); };

				//constexpr auto data() requires(contiguous_iterator<iterator_t<D>>);
				//constexpr auto data() const requires(range<const D> && contiguous_iterator<iterator_t<const D>>);

				//constexpr auto size() const requires(forward_range<const D> && sized_sentinel<sentinel_t<const D>, iterator_t<const D>>);

				//constexpr decltype(auto) front() requires(forward_range<D>);
				//constexpr decltype(auto) front() const requires(forward_range<const D>);
				//constexpr decltype(auto) back() requires(bidirectional_range<D> && common_range<D>);
				//constexpr decltype(auto) back() const requires(bidirectional_range<const D> && common_range<const D>);

				//template<random_access_range R = D>
				//constexpr decltype(auto) operator[](int);

				//template<random_access_range R = const D>
				//constexpr decltype(auto) operator[](int) const;
		};

		template<class T>
			requires(std::is_object_v<T>)
		class empty_view : public view_interface<empty_view<T>>
		{

		};

		namespace views
		{
			template<class T>
			constexpr empty_view<T> empty{};
		}

		template<class T>
		constexpr bool enable_borrowed_range<empty_view<T>> = true;

		enum class subrange_kind : bool
		{
			unsized,
			sized
		};

		template<collin::iterators::input_or_output_iterator I, sentinel_for<I> S = I, 
			subrange_kind K = collin::type_traits::conditional_value2_v<sized_sentinel_for<S, I>, subrange_kind, subrange_kind::sized, subrange_kind::unsized>>
			requires(K == subrange_kind::sized || !sized_sentinel_for<S, I>)
		class subrange : public view_interface<subrange<I, S, K>>
		{

		};

		template<collin::iterators::input_or_output_iterator I, sentinel_for<I> S, subrange_kind K>
		constexpr bool enable_borrowed_range<subrange<I, S, K>> = true;

		template<range R>
		using borrowed_subrange_t = std::conditional_t<borrowed_range<R>, subrange<iterator_t<R>>, dangling>;

		template<collin::iterators::weakly_incrementable W, concepts::semi_regular Bound = unreachable_sentinel_t>
			requires(concepts::weakly_equality_comparable_with<W, Bound> && concepts::semi_regular<W>)
		class iota_view : public view_interface<iota_view<W, Bound>>
		{
			public:
				class iterator
				{
					public:
						constexpr explicit iterator(W value)
							: value_{value} {}

						constexpr W operator*() const noexcept(std::is_nothrow_constructible_v<W>)
						{
							return value_;
						}
						
						constexpr iterator& operator++()
						{
							++value_;
							return *this;
						}

						constexpr void operator++(int)
						{
							++value_;
						}

						constexpr iterator operator++(int)
							requires(iterators::incrementable<W>)
						{
							auto tmp = *this;
							++value_;
							return tmp;
						}

						constexpr iterator& operator--()
							requires(interators::decrementable<W>)
						{
							--value_;
							return *this;
						}

						constexpr iterator operator--(int)
							requires(iterators::decrementable<W>)
						{
							auto tmp = *this;
							--value_;
							return tmp;
						}

						constexpr W operator[](iterators::iter_difference_t<W> n) const
							requires(iterators::advanceable<W>)
						{
							return W{value_ + n};
						}

						friend constexpr bool operator==(const iterator& x, const iterator& y)
						{
							return x.value_ == y.value_;
						}

						friend constexpr bool operator!=(const iterator& x, const iterator& y)
						{
							return !(x == y);
						}
					private:
						W value_;
				};

				class sentinel
				{
					public:
						sentinel() = default;
						constexpr explicit sentinel(Bound bound)
							: bound_{bound} {};

						friend constexpr bool operator==(const iterator& x, const sentinel& y)
						{
							return x.value_ == y.bound_;
						}

						friend constexpr bool operator==(const sentinel& x, const iterator& y)
						{
							return y == x;
						}

						friend constexpr bool operator!=(const iterator& x, const sentinel& y)
						{
							return !(x == y);
						}

						friend constexpr bool operator!=(const sentinel& x, const iterator& y)
						{
							return !(x == y);
						}

						friend constexpr iterators::iter_difference_t<W>
						operator-(const sentinel& x, const iterator& y)
							requires(sized_sentinel_for<Bound, W>)
						{
							return -(y.value_ - x.bound_);
						}

						friend constexpr iterators::iter_difference_t<W>
						operator-(const iterator& x, const sentinel& y)
							requires(sized_sentinel_for<Bound, W>)
						{
							return x.value_ - y.bound_;
						}
					private:
						Bound bound_{};
				};

				iota_view() = default;

				constexpr explicit iota_view(W value)
					: value_{value} {}

				constexpr iota_view(std::type_identity_t<W> value, std::type_identity_t<Bound> bound)
					: value_{value}, bound_{bound} {}

				constexpr auto begin() const
				{
					return iterator{value_};
				}

				constexpr auto end() const
				{
					if constexpr(concepts::same<Bound, unreachable_sentinel_t>)
					{
						return unreachable_sentinel;
					}
					else
					{
						return sentinel{bound_};
					}
				}

				constexpr auto end() const
					requires(concepts::same<W, Bound>)
				{
					return iterator{bound_};
				}

				constexpr auto size() const
					requires((concepts::same<W, Bound> && concepts::advanceable<W, typename iter_difference_t<W>>) ||
							 (concepts::integral<W> && concepts::integral<Bound>) ||
							 sized_sentinel_for<Bound, W>)
				{
					if constexpr(concepts::integer_like<W> && concepts::integer_like<Bound>)
					{
						return (value < 0)
							?
							((bound < 0)
								? collin::type_traits::make_unsigned(-value_) - collin::type_traits::make_unsigned(-bound_)
								: collin::type_traits::make_unsigned(bound_) + collin::type_traits::make_unsigned(-value_))
							: collin::type_traits::make_unsigned(bound_) - collin::type_traits::make_unsigned(value_);
					}
					else
					{
						return collin::type_traits::make_unsigned(bound_);
					}
				}
			private:
				W value_{};
				Bound bound_{};
		};

		template<class W, class Bound>
			requires((!concepts::integer_like<W> || !concepts::integer_like<Bound>) ||
					 concepts::signed_integer_like<W> == concepts::signed_integer_like<Bound>)
			iota_view(W, Bound) -> iota_view<W, Bound>;

		namespace views
		{
			template<collin::iterators::weakly_incrementable W>
			constexpr auto iota(W w)
			{
				return collin::ranges::iota_view{w};
			}

			template<collin::iterators::weakly_incrementable W, concepts::semi_regular Bound>
			constexpr auto iota(W w, Bound b)
			{
				return collin::ranges::iota_view{w, b};
			}
		}

		template<collin::iterators::weakly_incrementable W, concepts::semi_regular Bound>
		constexpr bool enable_borrowed_range<iota_view<W, Bound>> = true;
	}
}