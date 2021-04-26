export module cmoon.datetime.utility;

import cmoon.measures.time;

namespace cmoon::datetime
{
	export
	[[nodiscard]] constexpr bool is_am(const cmoon::measures::hours& h) noexcept
	{
		return cmoon::measures::hours{0} <= h && h <= cmoon::measures::hours{11};
	}

	export
	[[nodiscard]] constexpr bool is_pm(const cmoon::measures::hours& h) noexcept
	{
		return cmoon::measures::hours{12} <= h && h <= cmoon::measures::hours{23};
	}

	export
	[[nodiscard]] constexpr cmoon::measures::hours make12(const cmoon::measures::hours& h) noexcept
	{
		switch (h.count())
		{
			case 0:
				return cmoon::measures::hours{12};
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
				return h - cmoon::measures::hours{12};
			default:
				return h;
		}
	}

	export
	[[nodiscard]] constexpr cmoon::measures::hours make24(const cmoon::measures::hours& h, const bool is_pm) noexcept
	{
		if (is_pm)
		{
			switch (h.count())
			{
				case 12:
					return h;
				default:
					return h + cmoon::measures::hours{12};
			}
		}
		else
		{
			switch (h.count())
			{
				case 12:
					return cmoon::measures::hours{0};
				default:
					return h;
			}
		}
	}
}