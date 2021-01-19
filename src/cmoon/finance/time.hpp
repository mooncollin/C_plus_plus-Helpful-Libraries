#pragma once

#include <cstdint>
#include <ratio>

#include "cmoon/measures/measure.hpp"
#include "cmoon/measures/time.hpp"

namespace cmoon
{
	namespace finance
	{
		template<class Rep, std::size_t DaysInOneYear>
		using base_year = cmoon::measures::time<Rep, cmoon::measures::metric_system, std::ratio_multiply<std::ratio<DaysInOneYear>, typename cmoon::measures::basic_days<Rep>::ratio>>;

		template<class Rep>
		using basic_years365 = base_year<Rep, 365>;

		template<class Rep>
		using basic_years360 = base_year<Rep, 360>;

		using years365 = basic_years365<std::intmax_t>;

		using years360 = basic_years360<std::intmax_t>;

		static_assert(cmoon::measures::unit_cast<cmoon::measures::days>(years360{1}) == cmoon::measures::days{360});
		static_assert(cmoon::measures::unit_cast<cmoon::measures::days>(years365{1}) == cmoon::measures::days{365});
	}
}