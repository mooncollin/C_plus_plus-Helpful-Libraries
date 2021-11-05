export module cmoon.execution.bulk_execute;

import <concepts>;
import <cstddef>;
import <type_traits>;
import <functional>;

import cmoon.meta;
import cmoon.property;
import cmoon.utility;

import cmoon.execution.bulk_guarantee_t;
import cmoon.execution.executor_index;

namespace cmoon::execution
{
	namespace bulk_execute_ns
	{
		void bulk_execute();

		class bulk_execute_t
		{
			private:
				enum class state { none, member_fn, default_fn, inline_fn };

				template<class S, class F, std::convertible_to<std::size_t> N>
				static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (requires(S&& s, F&& f, N n) {
						std::forward<S>(s).bulk_execute(std::forward<F>(f), n);
					})
					{
						return {state::member_fn, noexcept(std::declval<S>().bulk_execute(std::declval<F>(), std::declval<N>()))};
					}
					else if constexpr (requires(S&& s, F&& f, N n) {
						bulk_execute(std::forward<S>(s), std::forward<F>(f), n);
					})
					{
						return {state::default_fn, noexcept(bulk_execute(std::declval<S>(), std::declval<F>(), std::declval<N>()))};
					}
					else if constexpr (std::invocable<F, executor_index_t<std::remove_cvref_t<S>>> &&
									   cmoon::query(std::remove_cvref_t<S>{}, execution::bulk_guarantee) == execution::bulk_guarantee.unsequenced)
					{
						return {state::inline_fn};
					}
					else
					{
						return {state::none};
					}
				}
			public:
				template<class S, class F, std::convertible_to<std::size_t> N>
					requires(choose<S, F, N>().strategy != state::none)
				constexpr auto operator()(S&& s, F&& f, N n) const noexcept(choose<S, F, N>().no_throw)
				{
					constexpr auto choice {choose<S, F, N>()};

					if constexpr (choice.strategy == state::member_fn)
					{
						return std::forward<S>(s).bulk_execute(std::forward<F>(f), n);
					}
					else if constexpr (choice.strategy == state::default_fn)
					{
						return bulk_execute(std::forward<S>(s), std::forward<F>(f), n);
					}
					else if constexpr (choice.strategy == state::inline_fn)
					{
						auto cf = cmoon::decay_copy(std::forward<F>(f));
						for (N i {0}; i < n; ++i)
						{
							std::invoke(cf, i);
						}
						return std::forward<S>(s);
					}
				}
		};
	}

	export
	inline constexpr bulk_execute_ns::bulk_execute_t bulk_execute {};
}