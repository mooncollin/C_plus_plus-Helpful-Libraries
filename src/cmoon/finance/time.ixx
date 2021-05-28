export module cmoon.finance.time;

import <cstdint>;
import <ratio>;
import <cstddef>;
import <chrono>;

namespace cmoon::finance
{
	export
	template<class Rep, std::size_t DaysInOneYear>
	using base_year = std::chrono::duration<Rep, std::ratio_multiply<std::ratio<DaysInOneYear>, std::ratio<86400>>>;

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

	static_assert(std::chrono::duration_cast<std::chrono::days>(years360{1}) == std::chrono::days{360});
	static_assert(std::chrono::duration_cast<std::chrono::days>(years365{1}) == std::chrono::days{365});
}