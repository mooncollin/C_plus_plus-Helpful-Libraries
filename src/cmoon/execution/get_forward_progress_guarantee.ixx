export module cmoon.execution.get_forward_progress_guarantee;

import <utility>;
import <concepts>;

import cmoon.meta;
import cmoon.functional;

import cmoon.execution.scheduler;
import cmoon.execution.forward_progress_guarantee;

namespace cmoon::execution
{
	export
	struct get_forward_progress_guarantee_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(get_forward_progress_guarantee_t t, Sch&& sch) {
					{ tag_invoke(t, std::forward<Sch>(sch)) } noexcept -> std::same_as<forward_progress_guarantee>;
				})
				{
					return {state::tag_invoke_fn, true};
				}
				else
				{
					return {state::other, true};
				}
			}
		public:
			template<scheduler Sch>
			constexpr decltype(auto) operator()(Sch&& sch) const noexcept(choose<Sch>().no_throw)
			{
				constexpr auto choice {choose<Sch>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Sch>(sch));
				}
				else if constexpr (choice.strategy == state::other)
				{
					return forward_progress_guarantee::weakly_parallel;
				}
			}
	};

	export
	inline constexpr get_forward_progress_guarantee_t get_forward_progress_guarantee{};
}