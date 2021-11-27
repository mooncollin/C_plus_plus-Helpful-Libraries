export module cmoon.execution.get_completion_scheduler;

import <utility>;
import <type_traits>;
import <concepts>;

import cmoon.functional;

import cmoon.execution.set_done;
import cmoon.execution.set_value;
import cmoon.execution.set_error;
import cmoon.execution.sender;
import cmoon.execution.scheduler;

namespace cmoon::execution
{
	export
	template<class CPO>
		requires(std::same_as<std::remove_cvref_t<CPO>, set_done_t> ||
				 std::same_as<std::remove_cvref_t<CPO>, set_value_t> ||
				 std::same_as<std::remove_cvref_t<CPO>, set_error_t>)
	struct get_completion_scheduler_t
	{
		template<sender S>
			requires(requires(get_completion_scheduler_t t, S&& s) {
				{ tag_invoke(t, std::as_const(s)) } noexcept -> scheduler;
			})
		constexpr decltype(auto) operator()(S&& s) const noexcept
		{
			return tag_invoke(*this, std::as_const(s));
		}
	};

	export
	template<class CPO>
	inline constexpr get_completion_scheduler_t<CPO> get_completion_scheduler{};
}