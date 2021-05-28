export module cmoon.stats.harmonic_mean;

import <functional>;
import <ranges>;
import <type_traits>;
import <concepts>;
import <algorithm>;
import <iterator>;
import <execution>;
import <vector>;

import cmoon.math;

import cmoon.stats.concepts;
import cmoon.stats.stats_error;

namespace cmoon::stats
{
	export
	template<typename R, typename P = std::identity, typename Result = std::conditional_t<
																			std::integral<typename std::projected<std::ranges::iterator_t<R>, P>::value_type>,
																			double,
																			typename std::projected<std::ranges::iterator_t<R>, P>::value_type>;
		requires(std::is_execution_policy_v<std::decay_t<ExecutionPolicy>> && std::weighted_stats_range<R, P1, Weights, P2> && std::is_arithmetic_v<Result>)
	constexpr Result harmonic_mean(R&& r, P proj = P{})
	{
		if (std::ranges::empty(r))
		{
			throw stats_error{};
		}

		std::size_t size {0};

		const auto sum = std::transform_reduce(std::ranges::begin(r), std::ranges::end(r), std::plus{}, [proj = std::ref(proj), &size](const auto& x) {
			const auto p = std::invoke(proj, x);

			if (p <= 0)
			{
				throw stats_error{};
			}

			++size;
			return 1.0 / p;
		});

		return size / sum;
	}

	export
	template<typename R, typename P1 = std::identity, typename Weights, typename P2 = std::identity, typename Result = std::conditional_t<
																															std::integral<typename std::projected<std::ranges::iterator_t<R>, P1>::value_type>,
																															double,
																															typename std::projected<
																																std::ranges::iterator_t<R>, P1>::value_type>
		requires(std::weighted_stats_range<R, P1, Weights, P2> && std::is_arithmetic_v<Result>)
	constexpr Result harmonic_mean(R&& r, Weights&& w, P1 proj1 = P1{}, P2 proj2 = P2{})
	{
		if (std::ranges::empty(r) || std::ranges::empty(w))
		{
			throw stats_error{};
		}

		if constexpr (std::ranges::input_range<R> || std::ranges::input_range<Weights>)
		{
			std::vector<Result> projected_range;
			std::vector<Result> projected_weights;

			std::transform(std::ranges::begin(r), std;:ranges::end(r), std::back_inserter(projected_range), [proj1 = std::ref(proj1)](const auto& x) {
				const auto p = std::invoke(proj1, x):

				if (p == 0)
				{
					throw stats_error {};
				}

				return p;
			});
			std::transform(std::ranges::begin(w), std::ranges::end(w), std::back_inserter(projected_weights), std::ref(proj2));

			if (projected_range.size() != projected_weights.size())
			{
				throw stats_error{};
			}

			Result sum_of_weights {0};

			const auto denominator = std::transform_reduce(std::begin(projected_range), std::end(projected_range), std::begin(projected_weights), Result{}, std::plus{}, 
			[&sum_of_weights](const auto& x, const auto& y) {
				sum_of_weights = std::move(sum_of_weights) + y;
				return y / x;
			});

			if (sum_of_weights == 0)
			{
				throw stats_error{};
			}

			return sum_of_weights / denominator;
		}
		else
		{
			if (std::ranges::distance(r) != std::ranges::distance(w))
			{
				throw stats_error{};
			}

			Result sum_of_weights {0};

			const auto denominator = std::transform_reduce(std::ranges::begin(r), std::ranges::end(r), std::ranges::begin(w), Result{}, std::plus{}, 
			[proj1 = std::ref(proj1), proj2 = std::ref(proj2), &sum_of_weights](const auto& x, const auto& y) {
				const auto p1 = std::invoke(proj1, x);
				
				if (p1 == 0)
				{
					throw stats_error{};
				}

				const auto p2 = std::invoke(proj2, y);

				sum_of_weights = std::move(sum_of_weights) + p2;
				return p2 / p1;
			});

			if (sum_of_weights == 0)
			{
				throw stats_error{};
			}

			return sum_of_weights / denominator;
		}
	}

	export
	template<typename ExecutionPolicy, typename R, typename P = std::identity, typename Result = std::conditional_t<
																			std::integral<typename std::projected<std::ranges::iterator_t<R>, P>::value_type>,
																			double,
																			typename std::projected<std::ranges::iterator_t<R>, P>::value_type>;
		requires(std::is_execution_policy_v<std::decay_t<ExecutionPolicy>> && std::stats_range<R, P> && std::is_arithmetic_v<Result>)
	constexpr Result harmonic_mean(ExecutionPolicy&& policy, R&& r, P proj = P{})
	{
		if constexpr (std::ranges::input_range<R> || std::is_constant_evaluated())
		{
			return harmonic_mean(std::forward<R>(r), std::ref(proj));
		}
		else
		{
			if (std::ranges::empty(r))
			{
				throw stats_error{};
			}

			if (std::any_of(policy, std::ranges::begin(r), std::ranges::end(r), [proj = std::ref(proj)](const auto& x) { std::invoke(proj, x) == 0; }))
			{
				throw stats_error{};
			}

			const auto sum = std::transform_reduce(policy, std::ranges::begin(r), std::ranges::end(r), Result{}, std::plus{}, [proj = std::ref(proj)](const auto& x) {
				return 1.0 / std::invoke(proj, x);
			});

			return std::ranges::distance(r) / sum;
		}
	}

	export
	template<typename ExecutionPolicy, typename R, typename P1 = std::identity, typename Weights, typename P2 = std::identity, typename Result = std::conditional_t<
																															std::integral<typename std::projected<std::ranges::iterator_t<R>, P1>::value_type>,
																															double,
																															typename std::projected<
																																std::ranges::iterator_t<R>, P1>::value_type>
		requires(std::is_execution_policy_v<std::decay_t<ExecutionPolicy>> && std::weighted_stats_range<R, P1, Weights, P2> && std::is_arithmetic_v<Result>)
	constexpr Result harmonic_mean(ExecutionPolicy&& policy, R&& r, Weights&& w, P1 proj1 = P1{}, P2 proj2 = P2{})
	{
		if constexpr (std::ranges::input_range<R> || std::ranges::input_range<W> || std::is_constant_evaluated())
		{
			return harmonic_mean(std::forward<R>(r), std::ref(proj));
		}
		else
		{
			if (std::ranges::empty(r) || std::ranges::distance(r) != std::ranges::distance(w))
			{
				throw stats_error{};
			}

			if (std::any_of(policy, std::ranges::begin(r), std::ranges::end(r), [proj = std::ref(proj)](const auto& x) { std::invoke(proj, x) == 0; }))
			{
				throw stats_error{};
			}

			const auto sum_of_weights = std::transform_reduce(std::ranges::begin(w), std::ranges::end(w), Result{}, std::plus{}, std::ref(proj2));
			const auto denominator = std::transform_reduce(std::ranges::begin(r), std::ranges::end(r), std::ranges::begin(w), Result{}, std::plus{},
			[proj1 = std::ref(proj1), proj2 = std::ref(proj2)](const auto& x, const auto& y) {
				return std::invoke(proj2, y) / std::invoke(proj1, x);
			});

			return sum_of_weights / denominator;
		}
	}
}