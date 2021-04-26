export module cmoon.finance.time;

import <cstdint>;
import <ratio>;

import cmoon.measures;
import cmoon.measures.time;

namespace cmoon::finance
{
	export
	template<class Rep, std::size_t DaysInOneYear>
	using base_year = cmoon::measures::time<Rep, cmoon::measures::metric_system, std::ratio_multiply<std::ratio<DaysInOneYear>, typename cmoon::measures::basic_days<Rep>::ratio>>;

	export
	template<class Rep>
	using basic_years365 = base_year<Rep, 365>;

	export
	template<class Rep>
	using basic_years360 = base_year<Rep, 360>;

	export
	using years365 = basic_years365<std::intmax_t>;

	export
	using years360 = basic_years360<std::intmax_t>;

	static_assert(cmoon::measures::unit_cast<cmoon::measures::days>(years360{1}) == cmoon::measures::days{360});
	static_assert(cmoon::measures::unit_cast<cmoon::measures::days>(years365{1}) == cmoon::measures::days{365});
}