export module cmoon.execution.transfer_just;

import <utility>;

import cmoon.concepts;
import cmoon.meta;
import cmoon.functional;

import cmoon.execution.scheduler;
import cmoon.execution.transfer;
import cmoon.execution.just;
import cmoon.execution.typed_sender;

namespace cmoon::execution
{
	export
	struct transfer_just_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch, class... Args>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(transfer_just_t t, Sch&& sch, Args&&... args) {
					{ tag_invoke(t, std::forward<Sch>(sch), std::forward<Args>(args)...) } -> typed_sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<transfer_just_t, Sch, Args...>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<scheduler Sch, class... Args>
			constexpr decltype(auto) operator()(Sch&& sch, Args&&... args) const noexcept(choose<Sch, Args...>().no_throw)
			{
				constexpr auto choice {choose<Sch, Args...>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Sch>(sch), std::forward<Args>(args)...);
				}
				else if constexpr (choice.strategy == state::other)
				{
					return execution::transfer(execution::just(std::forward<Args>(args)...), std::forward<Sch>(sch));
				}
			}
	};

	export
	inline constexpr transfer_just_t transfer_just{};
}