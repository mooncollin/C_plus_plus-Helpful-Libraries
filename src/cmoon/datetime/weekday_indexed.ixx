export module cmoon.datetime.weekday_indexed;

import cmoon.datetime.weekday;

namespace cmoon::datetime
{
	export
	class weekday_indexed
	{
		public:
			constexpr weekday_indexed() = default;
			constexpr weekday_indexed(const weekday& wd, unsigned index) noexcept
				: wd_{wd}, index_{index} {}

			[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
			{
				return wd_;
			}

			[[nodiscard]] constexpr unsigned index() const noexcept
			{
				return index_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return wd_.ok() && (index_ >= 1u && index_ <= 5u);
			}

			[[nodiscard]] friend constexpr bool operator==(const weekday_indexed& x, const weekday_indexed& y) noexcept = default;
		private:
			datetime::weekday wd_ {};
			unsigned index_ {};
	};
}