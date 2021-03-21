#pragma once

#include <optional>
#include <ranges>
#include <iterator>
#include <concepts>
#include <algorithm>
#include <numeric>
#include <functional>
#include <initializer_list>
#include <type_traits>

namespace cmoon
{
	template<std::contiguous_iterator I, class T>
	[[nodiscard]] constexpr auto average(I first, I last, T&& init)
	{
		const auto size = last - first;
		return std::reduce(first, last, init) / size;
	}

	template<std::contiguous_iterator I, class T>
	[[nodiscard]] constexpr auto average(I first, I last)
	{
		return average(first, last, 0.0);
	}

	template<std::input_iterator I, std::sentinel_for<I> S, class T, class BinaryOp = std::plus<>, class Proj = std::identity>
	[[nodiscard]] constexpr auto average(I first, S last, T&& init, BinaryOp&& op = {}, Proj&& proj = {})
	{
		std::size_t size {0};
		for (; first != last; ++first)
		{
			init = op(std::move(init), proj(*first));
			++size;
		}

		return init / size;
	}

	template<std::ranges::contiguous_range R, class T>
	[[nodiscard]] constexpr auto average(R&& r, T&& init)
	{
		return average(std::ranges::begin(r), std::ranges::end(r), std::forward<T>(init));
	}

	template<std::ranges::contiguous_range R>
	[[nodiscard]] constexpr auto average(R&& r)
	{
		return average(std::ranges::begin(r), std::ranges::end(r), 0.0);
	}

	template<class T, class T2>
	[[nodiscard]] constexpr auto average(std::initializer_list<T> values, T2&& init)
	{
		return average(std::begin(values), std::end(values), std::forward<T2>(init));
	}

	template<class T>
	[[nodiscard]] constexpr auto average(std::initializer_list<T> values)
	{
		return average(std::begin(values), std::end(values), 0.0);
	}

	template<std::ranges::input_range R>
	[[nodiscard]] constexpr auto average(R&& r)
	{
		return average(std::ranges::begin(r), std::ranges::end(r), 0.0);
	}

	template<std::ranges::input_range R, class T, class BinaryOp = std::plus<>, class Proj = std::identity>
	[[nodiscard]] constexpr auto average(R&& r, T&& init, BinaryOp&& op = {}, Proj&& proj = {})
	{
		return average(std::ranges::begin(r), std::ranges::end(r), std::forward<T>(init), std::forward<BinaryOp>(op), std::forward<Proj>(proj));
	}
}