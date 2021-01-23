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
			struct base_becquerels : public frequency<Rep, Ratio>
			{
				using frequency<Rep, Ratio>::frequency;
			};
		}

		template<class Rep>
		using basic_attobecquerels = details::base_becquerels<Rep, std::atto>;

		template<class Rep>
		using basic_femtobecquerels = details::base_becquerels<Rep, std::femto>;

		template<class Rep>
		using basic_picobecquerels = details::base_becquerels<Rep, std::pico>;

		template<class Rep>
		using basic_nanobecquerels = details::base_becquerels<Rep, std::nano>;

		template<class Rep>
		using basic_microbecquerels = details::base_becquerels<Rep, std::micro>;

		template<class Rep>
		using basic_millibecquerels = details::base_becquerels<Rep, std::milli>;

		template<class Rep>
		using basic_centibecquerels = details::base_becquerels<Rep, std::centi>;

		template<class Rep>
		using basic_decibecquerels = details::base_becquerels<Rep, std::deci>;

		template<class Rep>
		using basic_decabecquerels = details::base_becquerels<Rep, std::deca>;

		template<class Rep>
		using basic_hectobecquerels = details::base_becquerels<Rep, std::hecto>;

		template<class Rep>
		using basic_kilobecquerels = details::base_becquerels<Rep, std::kilo>;

		template<class Rep>
		using basic_megabecquerels = details::base_becquerels<Rep, std::mega>;

		template<class Rep>
		using basic_gigabecquerels = details::base_becquerels<Rep, std::giga>;

		template<class Rep>
		using basic_terabecquerels = details::base_becquerels<Rep, std::tera>;

		template<class Rep>
		using basic_exabecquerels = details::base_becquerels<Rep, std::exa>;

		template<class Rep>
		using basic_becquerels = details::base_becquerels<Rep, std::ratio<1>>;

		using attobecquerels = basic_attobecquerels<std::intmax_t>;
		using femtobecquerels = basic_femtobecquerels<std::intmax_t>;
		using picobecquerels = basic_picobecquerels<std::intmax_t>;
		using nanobecquerels = basic_nanobecquerels<std::intmax_t>;
		using microbecquerels = basic_microbecquerels<std::intmax_t>;
		using millibecquerels = basic_millibecquerels<std::intmax_t>;
		using centibecquerels = basic_centibecquerels<std::intmax_t>;
		using decibecquerels = basic_decibecquerels<std::intmax_t>;
		using decabecquerels = basic_decabecquerels<std::intmax_t>;
		using hectobecquerels = basic_hectobecquerels<std::intmax_t>;
		using kilobecquerels = basic_kilobecquerels<std::intmax_t>;
		using megabecquerels = basic_megabecquerels<std::intmax_t>;
		using gigabecquerels = basic_gigabecquerels<std::intmax_t>;
		using terabecquerels = basic_terabecquerels<std::intmax_t>;
		using exabecquerels = basic_exabecquerels<std::intmax_t>;
		using becquerels = basic_becquerels<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_becquerels<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Bq"))};
		};
	}
}
