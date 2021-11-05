export module cmoon.vectorized.product;

import <iterator>;
import <ranges>;
import <functional>;
import <utility>;

import cmoon.concepts;

import cmoon.vectorized.impl.vectorized_projection;

namespace cmoon::vectorized
{
	export
	template<std::contiguous_iterator I, std::sentinel_for<I> S, cmoon::arithmetic T, vectorized_projection<T> Proj = std::identity>
	constexpr auto product(I first, S last, T init, Proj proj = {})
	{
		#pragma omp simd
		for (; first != last; ++first)
		{
			init *= std::invoke(proj, *first);
		}
		
		return init;
	}

	export
	template<std::ranges::contiguous_range R, cmoon::arithmetic T, vectorized_projection<T> Proj = std::identity>
	constexpr auto product(R&& r, T init, Proj proj = {})
	{
		return product(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(proj));
	}
}