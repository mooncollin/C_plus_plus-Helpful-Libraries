export module cmoon.datetime.weekday_last;

import cmoon.datetime.weekday;

namespace cmoon::datetime
{
	export
	class weekday_last
	{
		public:
			explicit constexpr weekday_last(const datetime::weekday& wd) noexcept
				: wd_{wd} {}

			[[nodiscard]] constexpr datetime::weekday weekday() const noexcept
			{
				return wd_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return wd_.ok();
			}

			[[nodiscard]] friend constexpr bool operator==(const weekday_last& x, const weekday_last& y) noexcept = default;
		private:
			datetime::weekday wd_ {};
	};
}