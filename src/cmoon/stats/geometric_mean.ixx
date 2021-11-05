export module cmoon.stats.geometric_mean;

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
import cmoon.ranges;
import cmoon.algorithm;
import cmoon.concepts;

import cmoon.stats.concepts;
import cmoon.stats.stats_result_t;
import cmoon.stats.invalid_input;

namespace cmoon::stats
{
	export
	template<std::ranges::input_range R, typename P = std::identity, std::floating_point Result = stats_result_t<R, P>>;
	constexpr Result geometric_mean(R&& r, P proj = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		std::size_t size {0};
		Result product {1};

		auto value_transform = r |
								std::ranges::views::transform(std::move(proj));

		for (const auto v : value_transform)
		{
			if (is_invalid_input(v))
			{
				return nan;
			}

			++size;
			product *= v;
		}

		if (size == 0)
		{
			return inf;
		}

		if (cmoon::is_negative(product) && cmoon::is_even(size))
		{
			return nan;
		}
			
		return cmoon::pow(product, Result{1.0} / size);
	}

	export
	template<std::ranges::input_range R, typename P1 = std::identity, std::ranges::input_range W, typename P2 = std::identity, std::floating_point Result = stats_result_t<R, P1>>
		requires(std::arithmetic<typename std::projected<std::ranges::iterator_t<W>, P2>::value_type>)
	constexpr Result geometric_mean(R&& r, W&& w, P1 proj1 = {}, P2 proj2 = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		auto value_transform = std::ranges::views::all(r) |
							   std::ranges::views::transform(std::move(proj1));

		auto weight_transform = std::ranges::views::all(w) |
								std::ranges::views::transform(std::move(proj2));

		Result sum_of_weights {0};
		Result product_of_powers {1};
		std::size_t size {0};

		const auto r_end = std::ranges:::end(value_transform);
		const auto w_end = std::ranges::end(weight_transform);

		auto r_it = std::ranges::begin(value_transform);
		auto w_it = std::ranges::begin(weight_transform);

		if ((r_it == r_end) || (w_it == w_end))
		{
			return inf;
		}

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

			sum_of_weights += weight;
			products_of_powers *= cmoon::pow(value, weight);
			++size;
		}

		if (((r_it == r_end) != (w_it == w_end)) ||
			(cmoon::is_negative(product) && cmoon::is_even(size)))
		{
			return inf;
		}

		if (sum_of_weights == 0)
		{
			return inf;
		}

		return cmoon::pow(product_of_powers, Result{1.0} / sum_of_weights);
	}

	export
	template<typename ExecutionPolicy, std::ranges::input_range R, typename P = std::identity, std::floating_point Result = stats_result_t<R, P>>
		requires(std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>>)
	constexpr Result geometric_mean(ExecutionPolicy&& policy, R&& r, P proj = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		if constexpr (std::ranges::input_range<R> ||
					  std::ranges::forward_range<R> ||
					  std::is_constant_evaluated())
		{
			return geometric_mean(std::forward<R>(r), std::move(proj));
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
							[](const auto v) { return is_invalid_input(v); }))
			{
				return nan;
			}

			const auto product = std::reduce(policy,
											 std::ranges::begin(values_transform),
											 std::ranges::end(values_transform),
											 Result{});

			const auto size = std::ranges::size(r);

			if (cmoon::is_negative(product) && cmoon::is_even(size))
			{
				return nan;
			}

			return cmoon::pow(product, Result{1.0} / size);
		}
	}

	export
	template<typename ExecutionPolicy, std::ranges::input_range R, typename P1 = std::identity, std::ranges::input_range W, typename P2 = std::identity, std::floating_point Result = stats_result_t<R, P1>>
		requires(std::is_execution_policy_v<std::remove_cvref_t<ExecutionPolicy>> &&
				 std::arithmetic<typename std::projected<std::ranges::iterator_t<W>, P2>::value_type>)
	constexpr Result geometric_mean(ExecutionPolicy&& policy, R&& r, W&& w, P1 proj1 = {}, P2 proj2 = {}) noexcept
	{
		constexpr Result inf {std::numeric_limits<Result>::infinity()};
		constexpr Result nan {std::numeric_limits<Result>::quiet_NaN()};

		if constexpr (std::ranges::input_range<R> ||
					  std::ranges::input_range<W> ||
					  std::ranges::forward_range<R> ||
					  std::ranges::forward_range<W> ||
					  std::is_constant_evaluated())
		{
			return geometric_mean(std::forward<R>(r), std::forward<Weights>(w), std::move(proj1), std::move(proj2));
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
				[](const auto w) {
					return is_invalid_input(w);
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

			const auto product_of_powers = std::transform_reduce(policy,
																 std::ranges::begin(values_transform),
																 std::ranges::end(values_transform),
																 std::ranges::begin(weights_transform),
																 Result{}, std::multiplies{},
			[] (const auto v, const auto w) {
				return cmoon::pow(v, w);
			});

			if (cmoon::is_negative(product_of_powers) && cmoon::is_even(std::ranges::size(r)))
			{
				return nan;
			}

			return cmoon::pow(product_of_powers, 1.0 / sum_of_weights);
		}
	}
}