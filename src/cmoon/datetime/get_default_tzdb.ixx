export module cmoon.datetime.get_default_tzdb;

import cmoon.datetime.tzdb;

namespace cmoon::datetime
{
	export
	[[nodiscard]] tzdb get_default_tzdb()
	{
		return tzdb{};
	}
}