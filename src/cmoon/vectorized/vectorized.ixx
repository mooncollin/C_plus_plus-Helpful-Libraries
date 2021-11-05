export module cmoon.vectorized;
export import cmoon.vectorized.sum;
export import cmoon.vectorized.product;

import <iterator>;
import <ranges>;
import <functional>;
import <utility>;
import <concepts>;

import cmoon.concepts;

import cmoon.vectorized.impl.vectorized_projection;

namespace cmoon
{
	template<class F, class T, template<class> class F2>
	concept qualifying_func = std::same_as<F2<T>, F> ||
							  std::same_as<F2<void>, F>;

	struct vectorizor
	{
		template<std::contiguous_iterator class I,
				 std::sentinel_for<I> class S,
				 cmoon::arithmetic class T,
				 qualifying_func<T, std::plus> F,
				 vectorized_projected<T> proj = std::identity>
		constexpr auto operator()(I first, S last, T init, F f, Proj proj = {}) const
		{
			return vectorized::sum(std::move(first), std::move(last), std::move(init), std::move(proj));
		}

		template<std::ranges::contiguous_range R,
				 cmoon::arithmetic T,
				 qualifying_func<T, std::plus> F,
				 vectorized_projected<T> proj = std::identity>
		constexpr auto operator()(R&& r, T init, F f, Proj proj = {}) const
		{
			return vectorized::sum(std::forward<R>(r), std::move(init), std::move(proj));
		}

		template<std::contiguous_iterator I,
				 std::sentinel_for<I> S,
				 cmoon::arithmetic T,
				 qualifying_func<T, std::multiplies> F,
				 vectorized_projected<T> proj = std::identity>
		constexpr auto operator()(I first, S last, T init, F f, Proj proj = {}) const
		{
			return vectorized::product(std::move(first), std::move(last), std::move(init), std::move(proj));
		}

		template<std::ranges::contiguous_range R,
				 cmoon::arithmetic T,
				 qualifying_func<T, std::multiplies> F,
				 vectorized_projected<T> proj = std::identity>
		constexpr auto operator()(R&& r, T init, F f, Proj proj = {}) const
		{
			return vectorized::product(std::forward<R>(r), std::move(init), std::move(proj));
		}
	};

	export
	inline constexpr vectorizor vectorize {};

	export
	template<class I, class T, class F, class Proj = std::identity>
	concept vectorizable = (std::contiguous_iterator<I> &&
		requires(I iterator, T init, F&& f, Proj proj)
	{
		vectorize(std::move(iterator), std::move(iterator), std::move(init), f, std::move(proj));
	}) ||
	 (std::ranges::contiguous_range<I> &&
		requires(I&& range, T init, F&& f, Proj proj)
	{
		vectorize(std::forward<I>(range), std::move(init), f, std::move(proj));
	});
}