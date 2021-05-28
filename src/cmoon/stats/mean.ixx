export module cmoon.stats.mean;

import <functional>;
import <ranges>;
import <type_traits>;
import <concepts>;
import <algorithm>;
import <iterator>;
import <execution>;
import <vector>;

import cmoon.stats.concepts;
import cmoon.stats.stats_error;

namespace cmoon::stats
{
	export
	template<typename R, typename P = std::identity, typename Result = std::conditional_t<
																			std::integral<typename std::projected<std::ranges::iterator_t<R>, P>::value_type>,
																			double,
																			typename std::projected<std::ranges::iterator_t<R>, P>::value_type>;
		requires(std::stats_range<R, P> && std::is_arithmetic_v<Result>)
	constexpr Result mean(R&& r, P proj = P{})
	{
		if (std::ranges::empty(r))
		{
			throw stats_error{};
		}

		if constexpr (std::ranges::input_range<R>)
		{
			std::size_t size {0};
			Result sum {0};

			for (const auto& x : r)
			{
				++size;
				sum = std::move(sum) + std::invoke(proj, x);
			}

			return sum / size;
		}
		else
		{
			return std::transform_reduce(std::ranges::begin(r), std::ranges::end(r), Result{}, std::plus{}, std::ref(proj)) / std::ranges::distance(r);
		}
	}

	export
	template<typename R, typename P1 = std::identity, typename Weights, typename P2 = std::identity, typename Result = std::conditional_t<
																															std::integral<typename std::projected<std::ranges::iterator_t<R>, P1>::value_type>,
																															double,
																															typename std::projected<
																																std::ranges::iterator_t<R>, P1>::value_type>
		requires(std::weighted_stats_range<R, P1, Weights, P2> && std::is_arithmetic_v<Result>)
	constexpr Result mean(R&& r, Weights&& w, P1 proj1 = P1{}, P2 proj2 = P2{})
	{
		if (std::ranges::empty(r) || std::ranges::empty(w))
		{
			throw stats_error{};
		}

		if constexpr (std::ranges::input_range<R>)
		{
			std::vector<Result> projected_range;
			std::vector<Result> projected_weights;

			std::transform(std::ranges::begin(r), std::ranges::end(r), std::back_inserter(projected_range), std::ref(proj1));
			std::transform(std::ranges::begin(w), std::ranges::end(w), std::back_inserter(projected_weights), std::ref(proj2));

			if (projected_weights.size() != projected_range.size())
			{
				throw stats_error{};
			}

			const auto sum_of_weights = std::reduce(std::begin(projected_weights), std::end(projected_weights));

			if (sum_of_weights == 0)
			{
				throw stats_error{};
			}

			const auto sum_of_products = std::transform_reduce(std::begin(projected_range), std::end(projected_range), std::begin(projected_weights), Result{});

			return sum_of_products / sum_of_weights;
		}
		else
		{
			if (std::ranges::distance(r) != std::ranges::distance(w))
			{
				throw stats_error{};
			}

			const auto sum_of_weights = std::transform_reduce(std::ranges::begin(w), std::ranges::end(w), Result{}, std::plus{}, std::ref(proj2));

			if (sum_of_weights == 0)
			{
				throw stats_error{};
			}

			const auto sum_of_products = std::transform_reduce(std::ranges::begin(r), std::ranges::end(r), std::ranges::begin(w), Result{}, std::plus{},
			[proj1 = std::ref(proj1), proj2 = std::ref(proj2)] (const auto& x, const auto& y) {
				return std::invoke(proj1, x) * std::invoke(proj2, y);
			});

			return sum_of_products / sum_of_weights;
		}
	}

	export
	template<typename ExecutionPolicy, typename R, typename P = std::identity, typename Result = std::conditional_t<
																			std::integral<typename std::projected<std::ranges::iterator_t<R>, P>::value_type>,
																			double,
																			typename std::projected<std::ranges::iterator_t<R>, P>::value_type>;
		requires(std::is_execution_policy_v<st::decay_t<ExecutionPolicy>> && std::stats_range<R, P> && std::is_arithmetic_v<Result>)
	constexpr Result mean(ExecutionPolicy&& policy, R&& r, P proj = P{})
	{
		if constexpr (std::ranges::input_range<R> || std::is_constant_evaluated())
		{
			return mean(std::forward<R>(r), std::ref(proj));
		}
		else
		{
			if (std::ranges::empty(r))
			{
				throw stats_error{};
			}

			return std::transform_reduce(policy, std::ranges::begin(r), std::ranges::end(r), Result{}, std::plus{}, std::ref(proj)) / std::ranges::distance(r);
		}
	}

	export
	template<typename ExecutionPolicy, typename R, typename P1 = std::identity, typename Weights, typename P2 = std::identity, typename Result = std::conditional_t<
																															std::integral<typename std::projected<std::ranges::iterator_t<R>, P1>::value_type>,
																															double,
																															typename std::projected<
																																std::ranges::iterator_t<R>, P1>::value_type>
		requires(std::is_execution_policy_v<std::decay_t<ExecutionPolicy>> && std::weighted_stats_range<R, P1, Weights, P2> && std::is_arithmetic_v<Result>)
	constexpr Result mean(ExecutionPolicy&& policy, R&& r, Weights&& w, P1 proj1 = P1{}, P2 proj2 = P2{})
	{
		if constexpr (std::ranges::input_range<R> || std::is_constant_evaluated())
		{
			return mean(std::forward<R>(r), std::forward<Weight>(w), std::ref(proj1), std::ref(proj2));
		}
		else
		{
			if (std::ranges::empty(r) || std::ranges::empty(w) || std::ranges::distance(r) != std::ranges::distance(w))
			{
				throw stats_error{};
			}

			const auto sum_of_weights = std::transform_reduce(policy, std::ranges::begin(w), Result{}, std::plus{}, std::ref(proj2));

			if (sum_of_weights == 0)
			{
				throw stats_error{};
			}

			const auto sum_of_products = std::transform_reduce(policy, std::ranges::begin(r), std::ranges::end(r), std::ranges::begin(w), Result{}, std::plus{},
			[proj1 = std::ref(proj1), proj2 = std::ref(proj2)] (const auto& x, const auto& y) {
				return std::invoke(proj1, x) * std::invoke(proj2, y);
			});

			return sum_of_products / sum_of_weights;
		}
	}
}