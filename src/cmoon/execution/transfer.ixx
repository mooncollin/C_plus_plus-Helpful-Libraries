export module cmoon.execution.transfer;

import <utility>;

import cmoon.functional;
import cmoon.meta;

import cmoon.execution.get_completion_scheduler;
import cmoon.execution.sender;
import cmoon.execution.scheduler;
import cmoon.execution.schedule_from;
import cmoon.execution.set_value;

namespace cmoon::execution
{
	export
	struct transfer_t
	{
		private:
			enum class state { completion_scheduler_fn, tag_invoke_fn, other };

			template<class S, class Sch>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(transfer_t t, S&& s, Sch&& sch) {
					{ tag_invoke(t, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), std::forward<Sch>(sch)) } -> sender;
				})
				{
					using cs = decltype(get_completion_scheduler<set_value_t>(std::declval<S>()));
					return {state::completion_scheduler_fn, cmoon::nothrow_tag_invocable<transfer_t, cs, S, Sch>};
				}
				else if constexpr (requires(transfer_t t, S&& s, Sch&& sch) {
					{ tag_invoke(t, std::forward<S>(s), std::forward<Sch>(sch)) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<transfer_t, S, Sch>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<sender S, scheduler Sch>
			constexpr decltype(auto) operator()(S&& s, Sch&& sch) const noexcept(choose<S, Sch>().no_throw)
			{
				constexpr auto choice {choose<S, Sch>()};

				if constexpr (choice.strategy == state::completion_scheduler_fn)
				{
					return tag_invoke(*this, get_completion_scheduler<set_value_t>(s), std::forward<S>(s), std::forward<Sch>(sch));
				}
				else if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<S>(s), std::forward<Sch>(sch));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return execution::schedule_from(std::forward<Sch>(sch), std::forward<S>(s));
				}
			}
	};

	export
	inline constexpr transfer_t transfer{};
}