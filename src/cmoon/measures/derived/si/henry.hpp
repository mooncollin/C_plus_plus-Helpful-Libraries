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
			using base_henrys = electrical_inductance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attohenrys = details::base_henrys<Rep, std::atto>;

		template<class Rep>
		using basic_femtohenrys = details::base_henrys<Rep, std::femto>;

		template<class Rep>
		using basic_picohenrys = details::base_henrys<Rep, std::pico>;

		template<class Rep>
		using basic_nanohenrys = details::base_henrys<Rep, std::nano>;

		template<class Rep>
		using basic_microhenrys = details::base_henrys<Rep, std::micro>;

		template<class Rep>
		using basic_millihenrys = details::base_henrys<Rep, std::milli>;

		template<class Rep>
		using basic_centihenrys = details::base_henrys<Rep, std::centi>;

		template<class Rep>
		using basic_decihenrys = details::base_henrys<Rep, std::deci>;

		template<class Rep>
		using basic_decahenrys = details::base_henrys<Rep, std::deca>;

		template<class Rep>
		using basic_hectohenrys = details::base_henrys<Rep, std::hecto>;

		template<class Rep>
		using basic_kilohenrys = details::base_henrys<Rep, std::kilo>;

		template<class Rep>
		using basic_megahenrys = details::base_henrys<Rep, std::mega>;

		template<class Rep>
		using basic_gigahenrys = details::base_henrys<Rep, std::giga>;

		template<class Rep>
		using basic_terahenrys = details::base_henrys<Rep, std::tera>;

		template<class Rep>
		using basic_exahenrys = details::base_henrys<Rep, std::exa>;

		template<class Rep>
		using basic_henrys = details::base_henrys<Rep, std::ratio<1>>;

		using attohenrys = basic_attohenrys<std::intmax_t>;
		using femtohenrys = basic_femtohenrys<std::intmax_t>;
		using picohenrys = basic_picohenrys<std::intmax_t>;
		using nanohenrys = basic_nanohenrys<std::intmax_t>;
		using microhenrys = basic_microhenrys<std::intmax_t>;
		using millihenrys = basic_millihenrys<std::intmax_t>;
		using centihenrys = basic_centihenrys<std::intmax_t>;
		using decihenrys = basic_decihenrys<std::intmax_t>;
		using decahenrys = basic_decahenrys<std::intmax_t>;
		using hectohenrys = basic_hectohenrys<std::intmax_t>;
		using kilohenrys = basic_kilohenrys<std::intmax_t>;
		using megahenrys = basic_megahenrys<std::intmax_t>;
		using gigahenrys = basic_gigahenrys<std::intmax_t>;
		using terahenrys = basic_terahenrys<std::intmax_t>;
		using exahenrys = basic_exahenrys<std::intmax_t>;
		using henrys = basic_henrys<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_henrys<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("H"))};
		};
	}
}
