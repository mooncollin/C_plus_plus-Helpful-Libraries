export module cmoon.execution.bulk_execute;

import <utility>;
import <type_traits>;
import <concepts>;

import cmoon.meta;
import cmoon.property;
import cmoon.utility;

import cmoon.execution.sender;
import cmoon.execution.bulk_guarantee_t;
import cmoon.execution.executor_index;

namespace cmoon::execution
{
	namespace bulk_execute_cpo
	{
		void bulk_execute();

		template<class N>
		concept correct_size_type = std::convertible_to<N, std::size_t>;

		template<class S, class F, class N>
		concept has_adl =
			requires(S&& s, F&& f, N&& n)
		{
			bulk_execute(std::forward<S>(s), std::forward<F>(f), std::forward<N>(n));
		};

		template<class S, class F, class N>
		concept can_member_call =
			requires(S&& s, F&& f, N&& n)
		{
			std::forward<S>(s).bulk_execute(std::forward<F>(f), std::forward<N>(n));
		};

		template<class S, class F>
		concept can_execute = std::invocable<F> &&
								std::invocable<executor_index_t<std::remove_cvref_t<S>>> &&
								(cmoon::query(S{}, execution::bulk_guarentee) == execution::bulk_guarentee.unsequenced);

		struct cpo
		{
			private:
				enum class state { none, member_call, non_member_call, execution };

				template<class S, class F, class N>
				[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
				{
					if constexpr (can_member_call<S, F, N>)
					{
						return {state::member_call, noexcept(std::declval<S>().bulk_execute(std::declval<F>(), std::declval<N>()))};
					}
					else if constexpr (has_adl<S, F, N>)
					{
						return {state::non_member_call, noexcept(bulk_execute(std::declval<S>(), std::declval<F>(), std::declval<N>()))};
					}
					else if constexpr (can_execute<S, F>)
					{
						return {state::execution, noexcept(cmoon::decay_copy(std::declval<F&&>())) && noexcept(decltype(cmoon::decay_copy(std::declval<F&&>()))(std::declval<N>()))};
					}
					else
					{
						return {state::none};
					}
				}

				template<class S, class F, class N>
				static constexpr auto choice = choose<S, F, N>();
			public:
				template<class S, class F, class N>
					requires(choice<S, F, N>.strategy != state::none)
				constexpr decltype(auto) operator()(S&& s, F&& f, N&& n) const noexcept(choice<S, F, N>.no_throw)
				{
					if constexpr (choice<S, F, N>.strategy == state::member_call)
					{
						return std::forward<S>(s).bulk_execute(std::forward<F>(f), std::forward<N>(n));
					}
					else if constexpr (choice<S, F, N>.strategy == state::non_member_call)
					{
						return bulk_execute(std::forward<S>(s), std::forward<F>(f), std::forward<N>(n));
					}
					else if constexpr (choice<S, F, N>.strategy == state::execution)
					{
						auto cf = cmoon::decay_copy(std::forward<F>(f));
							
						for (std::size_t i {0}; i < n; ++i)
						{
							cf(i);
						}
					}
					else
					{
						static_assert(false, "should be unreachable");
					}
				}
		};
	}

	namespace cpos
	{
		export
		inline constexpr bulk_execute_cpo::cpo bulk_execute {};
	}

	using namespace cpos;
}