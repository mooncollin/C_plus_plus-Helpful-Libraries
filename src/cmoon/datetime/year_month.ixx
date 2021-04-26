export module cmoon.datetime.year_month;

import <compare>;

import cmoon.datetime.year;
import cmoon.datetime.month;

namespace cmoon::datetime
{
	export
	class year_month
	{
		public:
			constexpr year_month() = default;
			constexpr year_month(const datetime::year& y, const datetime::month& m) noexcept
				: y_{y}, m_{m} {}

			[[nodiscard]] constexpr datetime::year year() const noexcept
			{
				return y_;
			}

			[[nodiscard]] constexpr datetime::month month() const noexcept
			{
				return m_;
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return m_.ok();
			}

			[[nodiscard]] friend constexpr year_month operator+(const year_month& ym, const measures::years& y) noexcept
			{
				return year_month{ym.year() + y, ym.month()};
			}

			[[nodiscard]] friend constexpr year_month operator+(const measures::years& y, const year_month& ym) noexcept
			{
				return ym + y;
			}

			[[nodiscard]] friend constexpr year_month operator+(const year_month& ym, const measures::months& m) noexcept
			{
				const auto year_change = measures::unit_cast<measures::years>(m);
				const auto month_change = m - measures::unit_cast<measures::months>(year_change);
				return year_month{ym.year() + year_change, ym.month() + month_change};
			}

			[[nodiscard]] friend constexpr year_month operator+(const measures::months& m, const year_month& ym) noexcept
			{
				return ym + m;
			}

			[[nodiscard]] friend constexpr year_month operator-(const year_month& ym, const measures::years& y) noexcept
			{
				return ym + -y;
			}

			[[nodiscard]] friend constexpr year_month operator-(const year_month& ym, const measures::months& m) noexcept
			{
				return ym + -m;
			}

			[[nodiscard]] friend constexpr measures::months operator-(const year_month& ym1, const year_month& ym2) noexcept
			{
				return ym1.year() - ym2.year() + measures::months{static_cast<int>(unsigned{ym1.month()}) - static_cast<int>(unsigned{ym2.month()})};
			}

			constexpr year_month& operator+=(const measures::years& y) noexcept
			{
				*this = *this + y;
				return *this;
			}

			constexpr year_month& operator+=(const measures::months& m) noexcept
			{
				*this = *this + m;
				return *this;
			}

			constexpr year_month& operator-=(const measures::years& y) noexcept
			{
				*this = *this - y;
				return *this;
			}

			constexpr year_month& operator-=(const measures::months& m) noexcept
			{
				*this = *this - m;
				return *this;
			}

			[[nodiscard]] friend constexpr bool operator==(const year_month&, const year_month&) noexcept = default;
			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const year_month& x, const year_month& y) noexcept
			{
				const auto r = x.year() <=> y.year();
				if (r != std::strong_ordering::equal)
				{
					return r;
				}

				return x.month() <=> y.month();
			}
		private:
			datetime::year y_ {};
			datetime::month m_ {};
	};

	export
	[[nodiscard]] constexpr year_month operator/(const year& y, const month& m) noexcept
	{
		return year_month{y, m};
	}

	export
	[[nodiscard]] constexpr year_month operator/(const year& y, const int m) noexcept
	{
		return year_month{y, month{static_cast<unsigned>(m)}};
	}
}