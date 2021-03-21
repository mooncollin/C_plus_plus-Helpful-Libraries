#pragma once

namespace cmoon
{
	namespace execution
	{
		struct inline_executor
		{
			[[nodiscard]] friend constexpr bool operator==(const inline_executor&, const inline_executor&) noexcept
			{
				return true;
			}

			[[nodiscard]] friend constexpr bool operator!=(const inline_executor&, const inline_executor&) noexcept = default;

			template<class Function>
			void execute(Function&& f) const noexcept
			{
				f();
			}
		};
	}
}