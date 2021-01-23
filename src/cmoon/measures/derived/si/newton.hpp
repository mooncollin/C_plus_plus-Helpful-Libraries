#pragma once

#include <string_view>
#include <ratio>

#include "cmoon/string.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/measures/derived/base.hpp"

namespace cmoon
{
	namespace measures
	{
		namespace details
		{
			template<class Rep, cmoon::ratio_type Ratio>
			using base_newtons = force<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;
		}

		template<class Rep>
		using basic_attonewtons = details::base_newtons<Rep, std::atto>;

		template<class Rep>
		using basic_femtonewtons = details::base_newtons<Rep, std::femto>;

		template<class Rep>
		using basic_piconewtons = details::base_newtons<Rep, std::pico>;

		template<class Rep>
		using basic_nanonewtons = details::base_newtons<Rep, std::nano>;

		template<class Rep>
		using basic_micronewtons = details::base_newtons<Rep, std::micro>;

		template<class Rep>
		using basic_millinewtons = details::base_newtons<Rep, std::milli>;

		template<class Rep>
		using basic_centinewtons = details::base_newtons<Rep, std::centi>;

		template<class Rep>
		using basic_decinewtons = details::base_newtons<Rep, std::deci>;

		template<class Rep>
		using basic_decanewtons = details::base_newtons<Rep, std::deca>;

		template<class Rep>
		using basic_hectonewtons = details::base_newtons<Rep, std::hecto>;

		template<class Rep>
		using basic_kilonewtons = details::base_newtons<Rep, std::kilo>;

		template<class Rep>
		using basic_meganewtons = details::base_newtons<Rep, std::mega>;

		template<class Rep>
		using basic_giganewtons = details::base_newtons<Rep, std::giga>;

		template<class Rep>
		using basic_teranewtons = details::base_newtons<Rep, std::tera>;

		template<class Rep>
		using basic_exanewtons = details::base_newtons<Rep, std::exa>;

		template<class Rep>
		using basic_newtons = details::base_newtons<Rep, std::ratio<1>>;

		using attonewtons = basic_attonewtons<std::intmax_t>;
		using femtonewtons = basic_femtonewtons<std::intmax_t>;
		using piconewtons = basic_piconewtons<std::intmax_t>;
		using nanonewtons = basic_nanonewtons<std::intmax_t>;
		using micronewtons = basic_micronewtons<std::intmax_t>;
		using millinewtons = basic_millinewtons<std::intmax_t>;
		using centinewtons = basic_centinewtons<std::intmax_t>;
		using decinewtons = basic_decinewtons<std::intmax_t>;
		using decanewtons = basic_decanewtons<std::intmax_t>;
		using hectonewtons = basic_hectonewtons<std::intmax_t>;
		using kilonewtons = basic_kilonewtons<std::intmax_t>;
		using meganewtons = basic_meganewtons<std::intmax_t>;
		using giganewtons = basic_giganewtons<std::intmax_t>;
		using teranewtons = basic_teranewtons<std::intmax_t>;
		using exanewtons = basic_exanewtons<std::intmax_t>;
		using newtons = basic_newtons<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_newtons<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("N"))};
		};
	}
}
