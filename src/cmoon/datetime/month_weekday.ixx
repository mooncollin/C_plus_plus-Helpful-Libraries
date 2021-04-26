export module cmoon.datetime.month_weekday;

import cmoon.datetime.month;
import cmoon.datetime.weekday_indexed;

namespace cmoon::datetime
{
	export
	class month_weekday
	{
		public:
			constexpr month_weekday(const datetime::month m, const datetime::weekday_indexed w) noexcept
				: m_{m}, w_{w} {}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr datetime::weekday_indexed weekday_indexed() const noexcept
			{
				return w_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return m_.ok() && w_.ok();
			}

			[[nodiscard]] friend constexpr bool operator==(const month_weekday&, const month_weekday&) noexcept = default;
		private:
			datetime::month m_ {};
			datetime::weekday_indexed w_ {};
	};

	export
	[[nodiscard]] constexpr month_weekday operator/(const month& m, const weekday_indexed& w) noexcept
	{
		return month_weekday{m, w};
	}

	export
	[[nodiscard]] constexpr month_weekday operator/(const int m, const weekday_indexed& w) noexcept
	{
		return month_weekday{month{static_cast<unsigned>(m)}, w};
	}

	export
	[[nodiscard]] constexpr month_weekday operator/(const weekday_indexed& w, const month& m) noexcept
	{
		return month_weekday{m, w};
	}

	export
	[[nodiscard]] constexpr month_weekday operator/(const weekday_indexed& w, const int m) noexcept
	{
		return month_weekday{month{static_cast<unsigned>(m)}, w};
	}
}