export module cmoon.datetime.month_day_last;

import <compare>;

import cmoon.datetime.month;

namespace cmoon::datetime
{
	export
	class month_day_last
	{
		public:
			explicit constexpr month_day_last(const datetime::month& m) noexcept
				: m_{m} {}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return m_.ok();
			}

			[[nodiscard]] friend constexpr bool operator==(const month_day_last&, const month_day_last&) noexcept = default;
			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const month_day_last&, const month_day_last&) noexcept = default;
		private:
			datetime::month m_ {};
	};

	export
	[[nodiscard]] constexpr month_day_last operator/(const month& m, last_spec) noexcept
	{
		return month_day_last{m};
	}

	export
	[[nodiscard]] constexpr month_day_last operator/(const int m, last_spec) noexcept
	{
		return month_day_last{month{static_cast<unsigned>(m)}};
	}

	export
	[[nodiscard]] constexpr month_day_last operator/(last_spec, const month& m) noexcept
	{
		return month_day_last{m};
	}

	export
	[[nodiscard]] constexpr month_day_last operator/(last_spec, const int m) noexcept
	{
		return month_day_last{month{static_cast<unsigned>(m)}};
	}
}