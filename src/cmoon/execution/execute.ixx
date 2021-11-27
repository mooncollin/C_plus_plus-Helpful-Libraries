export module cmoon.execution.execute;

import <utility>;
import <concepts>;

import cmoon.meta;
import cmoon.functional;

import cmoon.execution.scheduler;
import cmoon.execution.schedule;
import cmoon.execution.start_detached;
import cmoon.execution.then;

namespace cmoon::execution
{
	export
	struct execute_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch, class F>
			static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(execute_t t, Sch&& sch, F&& f) {
					{ tag_invoke(t, std::forward<Sch>(sch), std::forward<F>(f)) } -> std::same_as<void>;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<execute_t, Sch, F>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<scheduler Sch, std::invocable<> F>
			constexpr void operator()(Sch&& sch, F&& f) const noexcept(choose<Sch, F>().no_throw)
			{
				constexpr auto choice {choose<Sch, F>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					tag_invoke(*this, std::forward<Sch>(sch), std::forward<F>(f));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return execution::start_detached(execution::then(execution::schedule(sch), std::forward<F>(f)));
				}
			}
	};

	export
	inline constexpr execute_t execute{};
}