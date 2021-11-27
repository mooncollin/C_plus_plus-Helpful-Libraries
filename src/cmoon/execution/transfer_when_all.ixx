export module cmoon.execution.transfer_when_all;

import <utility>;

import cmoon.functional;
import cmoon.meta;

import cmoon.execution.sender;
import cmoon.execution.typed_sender;
import cmoon.execution.scheduler;
import cmoon.execution.transfer;
import cmoon.execution.when_all;
import cmoon.execution.into_variant;

namespace cmoon::execution
{
	export
	struct transfer_when_all_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch, class... S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(transfer_when_all_t t, Sch&& sch, S&&... s) {
					{ tag_invoke(t, std::forward<Sch>(sch), std::forward<S>(s)...) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<transfer_when_all_t, Sch, S...>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<scheduler Sch, typed_sender... S>
			constexpr decltype(auto) operator()(Sch&& sch, S&&... s) const noexcept(choose<Sch, S...>().no_throw)
			{
				constexpr auto choice {choose<Sch, S...>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Sch>(sch), std::forward<S>(s)...);
				}
				else if constexpr (choice.strategy == state::other)
				{
					return transfer(when_all(std::forward<S>(s)...), std::forward<Sch>(sch));
				}
			}
	};

	export
	inline constexpr transfer_when_all_t transfer_when_all{};

	export
	struct transfer_when_all_with_variant_t
	{
		private:
			enum class state { tag_invoke_fn, other };

			template<class Sch, class... S>
			[[nodiscard]] static consteval cmoon::meta::choice_t<state> choose() noexcept
			{
				if constexpr (requires(transfer_when_all_with_variant_t t, Sch&& sch, S&&... s) {
					{ tag_invoke(t, std::forward<Sch>(sch), std::forward<S>(s)...) } -> sender;
				})
				{
					return {state::tag_invoke_fn, cmoon::nothrow_tag_invocable<transfer_when_all_with_variant_t, Sch, S...>};
				}
				else
				{
					return {state::other};
				}
			}
		public:
			template<scheduler Sch, typed_sender... S>
			constexpr decltype(auto) operator()(Sch&& sch, S&&... s) const noexcept(choose<Sch, S...>().no_throw)
			{
				constexpr auto choice {choose<Sch, S...>()};

				if constexpr (choice.strategy == state::tag_invoke_fn)
				{
					return tag_invoke(*this, std::forward<Sch>(sch), std::forward<S>(s)...);
				}
				else if constexpr (choice.strategy == state::other)
				{
					return transfer_when_all(std::forward<Sch>(sch), into_variant(std::forward<S>(s))...);
				}
			}
	};

	export
	inline constexpr transfer_when_all_with_variant_t transfer_when_all_with_variant{};
}