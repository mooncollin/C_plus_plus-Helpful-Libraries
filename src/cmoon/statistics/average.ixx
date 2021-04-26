export module cmoon.statistics.average;

import <optional>;
import <ranges>;
import <iterator>;
import <concepts>;
import <algorithm>;
import <numeric>;
import <functional>;
import <initializer_list>;
import <type_traits>;

namespace cmoon
{
	export
	template<std::input_iterator I, class T>
	[[nodiscard]] constexpr auto average(I first, std::make_unsigned_t<std::iter_difference_t<I>> size, T&& init)
	{
		return std::reduce(first, first + size, std::forward<T>(init)) / size;
	}

	export
	template<std::contiguous_iterator I, class T>
	[[nodiscard]] constexpr auto average(I first, I last, T&& init)
	{
		return average(first, last - first, std::forward<T>(init));
	}

	export
	template<std::contiguous_iterator I, class T>
	[[nodiscard]] constexpr auto average(I first, I last)
	{
		return average(first, last, 0.0);
	}

	export
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

	export
	template<std::ranges::range R, class T>
	[[nodiscard]] constexpr auto average(R&& r, T&& init)
	{
		return average(std::ranges::begin(r), std::ranges::end(r), std::forward<T>(init));
	}

	export
	template<std::ranges::range R>
	[[nodiscard]] constexpr auto average(R&& r)
	{
		return average(std::ranges::begin(r), std::ranges::end(r), 0.0);
	}

	export
	template<class T, class T2>
	[[nodiscard]] constexpr auto average(std::initializer_list<T> values, T2&& init)
	{
		return average(std::begin(values), std::end(values), std::forward<T2>(init));
	}

	export
	template<class T>
	[[nodiscard]] constexpr auto average(std::initializer_list<T> values)
	{
		return average(std::begin(values), std::end(values), 0.0);
	}

	export
	template<std::ranges::input_range R, class T, class BinaryOp = std::plus<>, class Proj = std::identity>
	[[nodiscard]] constexpr auto average(R&& r, T&& init, BinaryOp&& op = {}, Proj&& proj = {})
	{
		return average(std::ranges::begin(r), std::ranges::end(r), std::forward<T>(init), std::forward<BinaryOp>(op), std::forward<Proj>(proj));
	}
}