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
			using base_farads = electrical_capacitance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attofarads = details::base_farads<Rep, std::atto>;

		template<class Rep>
		using basic_femtofarads = details::base_farads<Rep, std::femto>;

		template<class Rep>
		using basic_picofarads = details::base_farads<Rep, std::pico>;

		template<class Rep>
		using basic_nanofarads = details::base_farads<Rep, std::nano>;

		template<class Rep>
		using basic_microfarads = details::base_farads<Rep, std::micro>;

		template<class Rep>
		using basic_millifarads = details::base_farads<Rep, std::milli>;

		template<class Rep>
		using basic_centifarads = details::base_farads<Rep, std::centi>;

		template<class Rep>
		using basic_decifarads = details::base_farads<Rep, std::deci>;

		template<class Rep>
		using basic_decafarads = details::base_farads<Rep, std::deca>;

		template<class Rep>
		using basic_hectofarads = details::base_farads<Rep, std::hecto>;

		template<class Rep>
		using basic_kilofarads = details::base_farads<Rep, std::kilo>;

		template<class Rep>
		using basic_megafarads = details::base_farads<Rep, std::mega>;

		template<class Rep>
		using basic_gigafarads = details::base_farads<Rep, std::giga>;

		template<class Rep>
		using basic_terafarads = details::base_farads<Rep, std::tera>;

		template<class Rep>
		using basic_exafarads = details::base_farads<Rep, std::exa>;

		template<class Rep>
		using basic_farads = details::base_farads<Rep, std::ratio<1>>;

		using attofarads = basic_attofarads<std::intmax_t>;
		using femtofarads = basic_femtofarads<std::intmax_t>;
		using picofarads = basic_picofarads<std::intmax_t>;
		using nanofarads = basic_nanofarads<std::intmax_t>;
		using microfarads = basic_microfarads<std::intmax_t>;
		using millifarads = basic_millifarads<std::intmax_t>;
		using centifarads = basic_centifarads<std::intmax_t>;
		using decifarads = basic_decifarads<std::intmax_t>;
		using decafarads = basic_decafarads<std::intmax_t>;
		using hectofarads = basic_hectofarads<std::intmax_t>;
		using kilofarads = basic_kilofarads<std::intmax_t>;
		using megafarads = basic_megafarads<std::intmax_t>;
		using gigafarads = basic_gigafarads<std::intmax_t>;
		using terafarads = basic_terafarads<std::intmax_t>;
		using exafarads = basic_exafarads<std::intmax_t>;
		using farads = basic_farads<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_farads<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("F"))};
		};
	}
}
