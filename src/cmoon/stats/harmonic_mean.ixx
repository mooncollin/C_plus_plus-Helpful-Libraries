export module cmoon.stats.harmonic_mean;

import <functional>;
import <ranges>;
import <type_traits>;
import <concepts>;
import <algorithm>;
import <iterator>;
import <execution>;
import <cmath>;
import <limits>;

import cmoon.math;

import cmoon.stats.concepts;
import cmoon.stats.stats_result_t;

namespace cmoon::stats
{
	export
	template<std::ranges::input_range R, typename P = std::identity, std::floating_point Result = stats_result_t<R, P>>
	constexpr Result harmonic_mean(R&& r, P proj = P{}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		std::size_t size {0};
		Result sum {0};

		auto values_transform = r |
								std::ranges::views::transform(std::move(proj));

		for (const auto v : values_transform)
		{
			if (is_invalid_input(v) ||
				cmoon::is_negative(v))
			{
				return nan;
			}

			if (v == 0)
			{
				return inf;
			}

			++size;
			sum += Result{1.0} / v;
		}

		if (size == 0)
		{
			return inf;
		}

		return size / sum;
	}

	export
	template<std::ranges::input_range R, typename P1 = std::identity, std::ranges::input_range W, typename P2 = std::identity, std::floating_point Result = stats_result_t<R, P1>>
		requires(std::arithmetic<typename std::projected<std::ranges::iterator_t<W>, P2>::value_type>)
	constexpr Result harmonic_mean(R&& r, W&& w, P1 proj1 = {}, P2 proj2 = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		auto values_transform = r |
								std::ranges::views::transform(std::move(proj1));

		auto weights_transform = w |
								 std::ranges::views::transform(std::move(proj2));

		std::size_t size {0};
		Result sum_of_weights {0};
		Result divisor {0};
		const auto r_end = std::ranges::end(values_transform);
		const auto w_end = std::ranges::end(weights_transform);

		auto r_it = std::ranges::begin(values_transform);
		auto w_it = std::ranges::begin(weights_transform);

		if (r_it == r_end || w_it == w_end)
		{
			return inf;
		}

		for (; r_it != r_end && w_it != w_end; ++r_it, ++w_it)
		{
			const auto value = *r_it;

			if (is_invalid_input(value) ||
				cmoon::is_negative(value))
			{
				return nan;
			}

			if (value == 0)
			{
				return inf;
			}

			const auto weight = *w_it;

			if (is_invalid_input(weight))
			{
				return nan;
			}

			sum_of_weights += weight;
			divisor += weight / value;
		}

		if (((r_it == r_end) != (w_it == w_end)) ||
			sum_of_weights == 0)
		{
			return inf;
		}

		return sum_of_weights / divisor;
	}

	export
	template<typename ExecutionPolicy, std::ranges::input_range R, typename P = std::identity, std::floating_point Result = stats_result_t<R, P>>
		requires(std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>>)
	constexpr Result harmonic_mean(ExecutionPolicy&& policy, R&& r, P proj = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		if constexpr (std::ranges::input_range<R> ||
					  std::ranges::forward_range<R> ||
					  std::is_constant_evaluated())
		{
			return harmonic_mean(std::forward<R>(r), std::move(proj));
		}
		else
		{
			if (std::ranges::empty(r))
			{
				return inf;
			}

			auto values_transform = r |
									std::ranges::views::transform(std::move(proj));

			for (const auto v : values_transform)
			{
				if (is_invalid_input(v) || cmoon::is_negative(v))
				{
					return nan;
				}

				if (v == 0)
				{
					return inf;
				}

			}

			const auto sum = std::transform_reduce(policy,
												   std::ranges::begin(values_transform),
												   std::ranges::end(values_transform),
												   Result{},
												   std::plus{},
				[](const auto v) {
				return Result{1.0} / v;
			});

			return std::ranges::size(r) / sum;
		}
	}

	export
	template<typename ExecutionPolicy, std::ranges::input_range R, typename P1 = std::identity, std::ranges::input_range W, typename P2 = std::identity, std::floating_point Result = stats_result_t<R, P1>>
		requires(std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>> &&
				 std::arithmetic<typename std::projected<std::ranges::iterator_t<W>, P2>::value_type>)
	constexpr Result harmonic_mean(ExecutionPolicy&& policy, R&& r, W&& w, P1 proj1 = {}, P2 proj2 = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		if constexpr (std::ranges::input_range<R>||
					  std::ranges::input_range<W> ||
					  std::ranges::forward_range<R> ||
					  std::ranges::forward_range<W> ||
					  std::is_constant_evaluated())
		{
			return harmonic_mean(std::forward<R>(r), std::move(proj));
		}
		else
		{
			if (std::ranges::empty(r) || std::ranges::size(r) != std::ranges::size(w))
			{
				return inf;
			}

			auto values_transform = r |
									std::ranges::views::transform(std::move(proj1));

			for (const auto v : values_transform)
			{
				if (is_invalid_input(v) || cmoon::is_negative(v))
				{
					return nan;
				}

				if (v == 0)
				{
					return inf;
				}

			}

			auto weights_transform = r |
									 std::ranges::views::transform(std::move(proj2));

			if (std::any_of(policy,
							 std::ranges::begin(weights_transform),
							 std::ranges::end(weights_transform),
				[](const auto w) {
					return is_invalid_input(w);
				})
			{
				return nan;
			}

			const auto sum_of_weights = std::reduce(policy,
													std::ranges::begin(weights_transform),
													std::ranges::end(weights_transform),
													Result{});

			if (sum_of_weights == 0)
			{
				return inf;
			}

			const auto denominator = std::transform_reduce(policy,
														   std::ranges::begin(values_transform),
														   std::ranges::end(values_transform),
														   std::ranges::begin(weights_transform),
														   Result{},
														   std::plus{},
			[](const auto v, const auto w) {
				return w / v;
			});

			return sum_of_weights / denominator;
		}
	}
}