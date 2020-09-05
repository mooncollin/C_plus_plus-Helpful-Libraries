#pragma once

#include <cstdint>
#include <ratio>

#include "collin/measures/time.hpp"

namespace collin
{
	namespace finance
	{
		template<class Rep, std::size_t DaysInOneYear>
		using base_year = collin::measures::time<Rep, collin::measures::metric_system, std::ratio_multiply<std::ratio<DaysInOneYear>, typename collin::measures::basic_days<Rep>::ratio>>;

		template<class Rep>
		using basic_years365 = base_year<Rep, 365>;

		template<class Rep>
		using basic_years360 = base_year<Rep, 360>;

		using years365 = basic_years365<std::intmax_t>;

		using years360 = basic_years360<std::intmax_t>;

		static_assert(collin::measures::unit_cast<collin::measures::days>(years360{1}) == collin::measures::days{360});
		static_assert(collin::measures::unit_cast<collin::measures::days>(years365{1}) == collin::measures::days{365});
	}
}