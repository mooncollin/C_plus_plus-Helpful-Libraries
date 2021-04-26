export module cmoon.datetime.month_weekday_last;

import cmoon.datetime.month;
import cmoon.datetime.weekday_last;

namespace cmoon::datetime
{
	export
	class month_weekday_last
	{
		public:
			constexpr month_weekday_last(const datetime::month& m, const datetime::weekday_last& w)
				: m_{m}, w_{w} {}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr datetime::weekday_last weekday_last() const noexcept
			{
				return w_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return m_.ok() && w_.ok();
			}

			[[nodiscard]] friend constexpr bool operator==(const month_weekday_last&, const month_weekday_last&) noexcept = default;
		private:
			datetime::month m_ {};
			datetime::weekday_last w_;
	};

	export
	[[nodiscard]] constexpr month_weekday_last operator/(const month& m, const weekday_last& w) noexcept
	{
		return month_weekday_last{m, w};
	}

	export
	[[nodiscard]] constexpr month_weekday_last operator/(const int m, const weekday_last& w) noexcept
	{
		return month_weekday_last{month{static_cast<unsigned>(m)}, w};
	}

	export
	[[nodiscard]] constexpr month_weekday_last operator/(const weekday_last& w, const month& m) noexcept
	{
		return month_weekday_last{m, w};
	}

	export
	[[nodiscard]] constexpr month_weekday_last operator/(const weekday_last& w, const int m) noexcept
	{
		return month_weekday_last{month{static_cast<unsigned>(m)}, w};
	}
}