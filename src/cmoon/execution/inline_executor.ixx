export module cmoon.execution.inline_executor;

import <concepts>;

namespace cmoon::execution
{
	export
	struct inline_executor
	{
		[[nodiscard]] friend constexpr bool operator==(const inline_executor&, const inline_executor&) noexcept
		{
			return true;
		}

		[[nodiscard]] friend constexpr bool operator!=(const inline_executor&, const inline_executor&) noexcept = default;

		template<std::invocable<> Function>
		void execute(Function&& f) const noexcept
		{
			f();
		}
	};
}