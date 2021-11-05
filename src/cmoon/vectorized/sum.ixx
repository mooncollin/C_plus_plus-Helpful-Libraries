export module cmoon.vectorized.sum;

import <iterator>;
import <ranges>;
import <functional>;
import <utility>;
import <concepts>;

import cmoon.concepts;

import cmoon.vectorized.impl.vectorized_projection;

namespace cmoon::vectorized
{
	export
	template<std::contiguous_iterator I, std::sentinel_for<I> S, cmoon::arithmetic T, vectorized_projection<T> Proj = std::identity>
	constexpr auto sum(I first, S last, T init, Proj proj = {})
	{
		#pragma omp simd
		for (; first != last; ++first)
		{
			init += std::invoke(proj, *first);
		}
		
		return init;
	}

	export
	template<std::ranges::contiguous_range R, cmoon::arithmetic T, vectorized_projection<T> Proj = std::identity>
	constexpr auto sum(R&& r, T init, Proj proj = {})
	{
		return sum(std::ranges::begin(r), std::ranges::end(r), std::move(init), std::move(proj));
	}
}