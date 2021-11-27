export module cmoon.execution.get_stop_token;

import <utility>;
import <type_traits>;

import cmoon.functional;
import cmoon.thread;

import cmoon.execution.receiver;

namespace cmoon::execution
{
	export
	struct get_stop_token_t
	{
		template<receiver R>
		constexpr decltype(auto) operator()(R&& r) const noexcept
		{
			if constexpr (requires(get_stop_token_t t, R&& r) {
				{ tag_invoke(t, std::forward<R>(r)) } noexcept -> cmoon::stoppable_token;
			})
			{
				return tag_invoke(*this, std::forward<R>(r));
			}
			else
			{
				return cmoon::never_stop_token{};
			}
		}
	};

	export
	inline constexpr get_stop_token_t get_stop_token{};

	export
	template<class R>
	using stop_token_of_t = std::invoke_result_t<get_stop_token_t, R>;
}