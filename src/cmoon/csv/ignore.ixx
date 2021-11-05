export module cmoon.csv.ignore;

import <tuple>;

namespace cmoon::csv
{
	export
	using csv_ignore_t = decltype(std::ignore);

	export
	inline constexpr csv_ignore_t csv_ignore {};
}