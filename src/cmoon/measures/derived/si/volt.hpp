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
			using base_volts = voltage<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attovolts = details::base_volts<Rep, std::atto>;

		template<class Rep>
		using basic_femtovolts = details::base_volts<Rep, std::femto>;

		template<class Rep>
		using basic_picovolts = details::base_volts<Rep, std::pico>;

		template<class Rep>
		using basic_nanovolts = details::base_volts<Rep, std::nano>;

		template<class Rep>
		using basic_microvolts = details::base_volts<Rep, std::micro>;

		template<class Rep>
		using basic_millivolts = details::base_volts<Rep, std::milli>;

		template<class Rep>
		using basic_centivolts = details::base_volts<Rep, std::centi>;

		template<class Rep>
		using basic_decivolts = details::base_volts<Rep, std::deci>;

		template<class Rep>
		using basic_decavolts = details::base_volts<Rep, std::deca>;

		template<class Rep>
		using basic_hectovolts = details::base_volts<Rep, std::hecto>;

		template<class Rep>
		using basic_kilovolts = details::base_volts<Rep, std::kilo>;

		template<class Rep>
		using basic_megavolts = details::base_volts<Rep, std::mega>;

		template<class Rep>
		using basic_gigavolts = details::base_volts<Rep, std::giga>;

		template<class Rep>
		using basic_teravolts = details::base_volts<Rep, std::tera>;

		template<class Rep>
		using basic_exavolts = details::base_volts<Rep, std::exa>;

		template<class Rep>
		using basic_volts = details::base_volts<Rep, std::ratio<1>>;

		using attovolts = basic_attovolts<std::intmax_t>;
		using femtovolts = basic_femtovolts<std::intmax_t>;
		using picovolts = basic_picovolts<std::intmax_t>;
		using nanovolts = basic_nanovolts<std::intmax_t>;
		using microvolts = basic_microvolts<std::intmax_t>;
		using millivolts = basic_millivolts<std::intmax_t>;
		using centivolts = basic_centivolts<std::intmax_t>;
		using decivolts = basic_decivolts<std::intmax_t>;
		using decavolts = basic_decavolts<std::intmax_t>;
		using hectovolts = basic_hectovolts<std::intmax_t>;
		using kilovolts = basic_kilovolts<std::intmax_t>;
		using megavolts = basic_megavolts<std::intmax_t>;
		using gigavolts = basic_gigavolts<std::intmax_t>;
		using teravolts = basic_teravolts<std::intmax_t>;
		using exavolts = basic_exavolts<std::intmax_t>;
		using volts = basic_volts<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_volts<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("V"))};
		};
	}
}
