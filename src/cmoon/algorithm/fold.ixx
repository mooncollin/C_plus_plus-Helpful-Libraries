export module cmoon.algorithm.fold;

import <concepts>;
import <iterator>;
import <ranges>;
import <functional>;
import <utility>;
import <optional>;

//import cmoon.vectorized;

namespace cmoon
{
	template<class F>
	struct flipped
	{
		F f;

		template<class T, class U>
			requires(std::invocable<F&, U, T>)
		std::invoke_result_t<F&, U, T> operator()(T&& t, U&& u);
	};

	export
	template<class I, class T>
	struct in_value_result
	{
		[[no_unique_address]] I in;
		[[no_unique_address]] T value;

		template<class I2, class T2>
			requires(std::convertible_to<const I&, I2> && std::convertible_to<const T&, T2>)
		constexpr operator in_value_result<I2, T2>() const &
		{
			return {in, value};
		}

		template<class I2, class T2>
			requires(std::convertible_to<I, I2> && std::convertible_to<T, T2>)
		constexpr operator in_value_result<I2, T2>() &&
		{
			return {std::move(in), std::move(value)};
		}
	};

	template<class F, class T, class I, class U>
	concept indirectly_binary_left_foldable_impl =
		std::movable<T> &&
		std::movable<U> &&
		std::convertible_to<T, U> &&
		std::invocable<F&, U, std::iter_reference_t<I>> &&
		std::assignable_from<U&, std::invoke_result_t<F&, U, std::iter_reference_t<I>>>;

	template<class F, class T, class I>
	concept indirectly_binary_left_foldable =
		std::copy_constructible<F> &&
		std::indirectly_readable<I> &&
		std::invocable<F&, T, std::iter_reference_t<I>> &&
		std::convertible_to<std::invoke_result_t<F&, T, std::iter_reference_t<I>>,
							std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>> &&
		indirectly_binary_left_foldable_impl<F, T, I, std::decay_t<std::invoke_result_t<F&, T, std::iter_reference_t<I>>>>;

	template<class F, class T, class I>
	concept indirectly_binary_right_foldable =
		indirectly_binary_left_foldable<flipped<F>, T, I>;

	template<class F, class T, class I>
	concept indirectly_short_circuit_left_foldable =
		std::copy_constructible<F> &&
		std::movable<T> &&
		std::indirectly_readable<I> &&
		std::invocable<F&, T&, std::iter_reference_t<I>> &&
		std::convertible_to<std::invoke_result_t<F&, T&, std::iter_reference_t<I>>, bool>;

	template<class I, class T>
	using fold_while_result = in_value_result<I, T>;

	export
	template<std::input_iterator I, std::sentinel_for<I> S, class T, class Proj = std::identity,
			 indirectly_binary_left_foldable<T, std::projected<I, Proj>> F>
	constexpr auto foldl(I first, S last, T init, F f, Proj proj = {})
	{
		/*if constexpr (cmoon::vectorizable<I, T, F, Proj>)
		{
			return cmoon::vectorize(std::move(first), std::move(last), std::move(init), std::move(f), std::move(proj));
		}
		else
		{*/
			using U = std::invoke_result_t<F&, T, std::indirect_result_t<Proj&, I>>;
			if (first == last)
			{
				return U{std::move(init)};
			}

			U accum = std::invoke(f, std::move(init), std::invoke(proj, *first));
			for (++first; first != last; ++first)
			{
				accum = std::invoke(f, std::move(accum), std::invoke(proj, *first));
			}

			return accum;
		//}
	}

	export
	template<std::ranges::input_range R, class T, class Proj = std::identity,
			 indirectly_binary_left_foldable<T, std::projected<std::ranges::iterator_t<R>, Proj>> F>
	constexpr auto foldl(R&& r, T init, F f, Proj proj = {})
	{
		return foldl(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(f), std::move(proj));
	}

	export
	template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity,
			 indirectly_binary_left_foldable<std::iter_value_t<std::projected<I, Proj>>, std::projected<I, Proj>> F>
	constexpr auto foldl1(I first, S last, F f, Proj proj = {})
	{
		using U = decltype(foldl(std::move(first), std::move(last), std::invoke(proj, *first), std::move(f), std::move(proj)));
		if (first == last)
		{
			return std::optional<U>{};
		}

		auto init{std::invoke(proj, *first)};
		++first;
		return std::optional<U>{std::in_place,
			foldl(std::move(first), std::move(last), std::move(init), std::move(f), std::move(proj))};
	}

	export
	template<std::ranges::input_range R, class Proj = std::identity,
			 indirectly_binary_left_foldable<std::iter_value_t<std::projected<std::ranges::iterator_t<R>, Proj>>, std::projected<std::ranges::iterator_t<R>, Proj>> F>
	constexpr auto foldl1(R&& r, F f, Proj proj = {})
	{
		return foldl1(std::ranges::begin(r), std::ranges::end(r), std::move(f), std::move(proj));
	}

	export
	template<std::bidirectional_iterator I, std::sentinel_for<I> S, class T, class Proj = std::identity,
			 indirectly_binary_right_foldable<T, std::projected<I, Proj>> F>
	constexpr auto foldr(I first, S last, T init, F f, Proj proj = {})
	{
		/*if constexpr (cmoon::vectorizable<std::reverse_iterator<I>, T, F, Proj>)
		{
			return cmoon::vectorize(std::reverse_iterator{std::move(last)},
									std::reverse_iterator{std::move(first)},
									std::move(init),
									std::move(f),
									std::move(proj));
		}
		else
		{*/
			using U = std::invoke_result_t<F&, std::indirect_result_t<Proj&, I>, T>;
			if (first == last)
			{
				return U{std::move(init)};
			}

			I tail = std::ranges::next(first, last);
			U accum = std::invoke(f, std::invoke(proj, *--tail), std::move(init));
			while (first != tail)
			{
				accum = std::invoke(f, std::invoke(proj, *--tail), std::move(accum));
			}

			return accum;
		//}
	}

	export
	template<std::ranges::bidirectional_range R, class T, class Proj = std::identity,
			 indirectly_binary_right_foldable<T, std::projected<std::ranges::iterator_t<R>, Proj>> F>
	constexpr auto foldr(R&& r, T init, F f, Proj proj = {})
	{
		return foldr(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(f), std::move(proj));
	}

	export
	template<std::bidirectional_iterator I, std::sentinel_for<I> S, class Proj = std::identity,
			 indirectly_binary_right_foldable<std::iter_value_t<std::projected<I, Proj>>, std::projected<I, Proj>> F>
	constexpr auto foldr1(I first, S last, F f, Proj proj = {})
	{
		using U = decltype(foldr(first, last, std::invoke(proj, *first), f, proj));
		if (first == last)
		{
			return std::optional<U>{};
		}

		I tail = std::ranges::prev(std::ranges::next(first, std::move(last)));
		auto tail_v = std::invoke(proj, *tail);
		return std::optional<U>{std::in_place,
			foldr(std::move(first), tail, std::move(tail_v), std::move(f), std::move(proj))};
	}

	export
	template<std::ranges::bidirectional_range R, class Proj = std::identity,
			 indirectly_binary_right_foldable<std::iter_value_t<std::projected<std::ranges::iterator_t<R>, Proj>>, std::projected<std::ranges::iterator_t<R>, Proj>> F>
	constexpr auto foldr1(R&& r, F f, Proj proj = {})
	{
		return foldr1(std::ranges::begin(r), std::ranges::end(r), std::move(f), std::move(proj));
	}

	export
	template<std::input_iterator I, std::sentinel_for<I> S, class T, class Proj = std::identity,
			 indirectly_short_circuit_left_foldable<T, std::projected<I, Proj>> F>
	constexpr fold_while_result<I, T> foldl_while(I first, S last, T init, F f, Proj proj = {})
	{
		for (; first != last; ++first)
		{
			if (!std::invoke(f, init, std::invoke(proj, *first)))
			{
				break;
			}
		}

		return {std::move(first), std::move(init)};
	}

	export
	template<std::ranges::input_range R, class T, class Proj = std::identity,
			 indirectly_short_circuit_left_foldable<T, std::projected<std::ranges::iterator_t<R>, Proj>> F>
	constexpr fold_while_result<std::ranges::borrowed_iterator_t<R>, T> foldl_while(R&& r, T init, F f, Proj proj = {})
	{
		return foldl_while(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(f), std::move(proj));
	}

	export
	template<std::input_iterator I, std::sentinel_for<I> S, class Proj = std::identity,
			 indirectly_short_circuit_left_foldable<std::iter_value_t<std::projected<I, Proj>>, std::projected<I, Proj>> F>
	constexpr fold_while_result<I, std::optional<std::iter_value_t<std::projected<I, Proj>>>> foldl1_while(I first, S last, F f, Proj proj = {})
	{
		if (first == last)
		{
			return {std::move(first), std::nullopt};
		}

		auto init{std::invoke(proj, *first)};
		++first;
		return foldl_while(std::move(first), std::move(last), std::move(init), std::move(f), std::move(proj));
	}

	export
	template<std::ranges::input_range R, class Proj = std::identity,
			 indirectly_short_circuit_left_foldable<std::iter_value_t<std::projected<std::ranges::iterator_t<R>, Proj>>, std::projected<std::ranges::iterator_t<R>, Proj>> F>
	constexpr fold_while_result<std::ranges::borrowed_iterator_t<R>, std::optional<std::iter_value_t<std::projected<std::ranges::iterator_t<R>, Proj>>>> foldl1_while(R&& r, F f, Proj proj = {})
	{
		return foldl1_while(std::ranges::begin(r), std::ranges::end(r), std::move(f), std::move(proj));
	}
}