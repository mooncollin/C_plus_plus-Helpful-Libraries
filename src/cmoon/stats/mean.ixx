export module cmoon.stats.mean;

import <functional>;
import <ranges>;
import <type_traits>;
import <concepts>;
import <algorithm>;
import <iterator>;
import <execution>;
import <limits>;
import <cmath>;

import cmoon.stats.stats_result_t;
import cmoon.stats.impl.invalid_input

namespace cmoon::stats
{
	export
	template<std::ranges::input_range R, typename P = std::identity, std::floating_point Result = stats_result_t<R, P>>
	constexpr Result mean(R&& r, P proj = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		std::size_t size {0};
		Result sum {0};

		for (const auto v : r | std::ranges::views::transform(std::move(proj)))
		{
			if (is_invalid_input(v))
			{
				return nan;
			}

			++size;
			sum += v;
		}

		if (size == 0)
		{
			return inf;
		}

		return sum / size;
	}

	export
	template<std::ranges::input_range R, typename P1 = std::identity, std::ranges::input_range W, typename P2 = std::identity, std::floating_point Result = stats_result_t<R, P1>>
		requires(std::arithmetic<typename std::projected<std::ranges::iterator_t<W>, P2>::value_type>)
	constexpr Result mean(R&& r, W&& w, P1 proj1 = P1{}, P2 proj2 = P2{}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		auto values_transform = r |
								std::ranges::views::transform(std::move(proj1));
								
		auto weights_transform = w |
								 std::ranges::views::transform(std::move(proj2));

		Result sum_weights {0};
		Result sum_products {0};
		const auto r_end = std::ranges::end(values_transform);
		const auto w_end = std::ranges::end(weights_transform);

		auto r_it = std::ranges::begin(values_transform);
		auto w_it = std::ranges::begin(weights_transform);

		for (; r_it != r_end && w_it != w_end; ++r_it, ++w_it)
		{
			const auto value = *r_it;

			if (is_invalid_input(value))
			{
				return nan;
			}

			const auto weight = *w_it;

			if (is_invalid_input(weight))
			{
				return nan;
			}

			sum_weights += weight;
			sum_products += value * weight;
		}

		if (((r_it == r_end) != (w_it == w_end)) ||
			sum_weights == 0)
		{
			return inf;
		}

		return sum_products / sum_weights;
	}

	export
	template<typename ExecutionPolicy, std::ranges::input_range R, typename P = std::identity, std::floating_point Result = stats_result_t<R, P>>;
		requires(std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>>)
	constexpr Result mean(ExecutionPolicy&& policy, R&& r, P proj = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		if constexpr (std::ranges::input_range<R> || std::ranges::forward_range<R> || std::is_constant_evaluated())
		{
			return mean(std::forward<R>(r), std::move(proj));
		}
		else
		{
			if (std::ranges::empty(r))
			{
				return inf;
			}

			auto values_transform = r |
									std::ranges::views::transform(std::move(proj));

			if (std::any_of(policy,
							std::ranges::begin(values_transform),
							std::ranges::end(values_transform),
				[](const auto v) {
					return is_invalid_input(v);
				}))
			{
				return nan;
			}

			const auto sum = std::reduce(policy,
										 std::ranges::begin(values_transform),
										 std::ranges::end(values_transform),
										 Result{});

			return sum / std::ranges::size(r);
		}
	}

	export
	template<typename ExecutionPolicy, std::ranges::input_range R, typename P1 = std::identity, std::ranges::input_range W, class P2 = std::identity, std::floating_point Result = stats_result_t<R, P1>>
		requires(std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>> &&
				 std::arithmetic<typename std::projected<std::ranges::iterator_t<W>, P2>::value_type>)
	constexpr Result mean(ExecutionPolicy&& policy, R&& r, W&& w, P1 proj1 = {}, P2 proj2 = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		if constexpr (std::ranges::input_range<R> ||
					  std::ranges::input_range<W> ||
					  std::ranges::forward_range<R> ||
					  std::ranges::forward_range<W> ||
					  std::is_constant_evaluated())
		{
			return mean(std::forward<R>(r), std::forward<Weight>(w), std::move(proj1), std::move(proj2));
		}
		else
		{
			if (std::ranges::empty(r) || std::ranges::size(r) != std::ranges::size(w))
			{
				return inf;
			}

			auto values_transform = r |
									std::ranges::views::transform(std::move(proj1));


			if (std::any_of(policy,
							std::ranges::begin(values_transform),
							std::ranges::end(values_transform),
				[](const auto v) {
					return is_invalid_input(v);
				}))
			{
				return nan;
			}

			auto weights_transform = w |
									 std::ranges::views::transform(std::move(proj2));

			if (std::any_of(policy,
							std::ranges::begin(weights_transform),
							std::ranges::end(weights_transform),
				[inf](const auto v) {
					return is_invalid_input(v);
				}))
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

			const auto sum_of_products = std::transform_reduce(policy,
															   std::ranges::begin(values_transform),
															   std::ranges::end(values_transform),
															   std::ranges::begin(weights_transform),
															   Result{});

			return sum_of_products / sum_of_weights;
		}
	}
}