export module cmoon.datetime.tzdb;

import <string>;
import <vector>;

import cmoon.datetime.time_zone;

namespace cmoon::datetime
{
	export
	struct tzdb
	{
		public:
			std::string version;
			std::vector<time_zone> zones;

		private:
			tzdb()
				: version{"2020f"} {}

			friend tzdb get_default_tzdb();
	};
}