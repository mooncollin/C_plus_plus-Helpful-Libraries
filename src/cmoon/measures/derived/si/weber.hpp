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
			using base_webers = magnetic_flux<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attowebers = details::base_webers<Rep, std::atto>;

		template<class Rep>
		using basic_femtowebers = details::base_webers<Rep, std::femto>;

		template<class Rep>
		using basic_picowebers = details::base_webers<Rep, std::pico>;

		template<class Rep>
		using basic_nanowebers = details::base_webers<Rep, std::nano>;

		template<class Rep>
		using basic_microwebers = details::base_webers<Rep, std::micro>;

		template<class Rep>
		using basic_milliwebers = details::base_webers<Rep, std::milli>;

		template<class Rep>
		using basic_centiwebers = details::base_webers<Rep, std::centi>;

		template<class Rep>
		using basic_deciwebers = details::base_webers<Rep, std::deci>;

		template<class Rep>
		using basic_decawebers = details::base_webers<Rep, std::deca>;

		template<class Rep>
		using basic_hectowebers = details::base_webers<Rep, std::hecto>;

		template<class Rep>
		using basic_kilowebers = details::base_webers<Rep, std::kilo>;

		template<class Rep>
		using basic_megawebers = details::base_webers<Rep, std::mega>;

		template<class Rep>
		using basic_gigawebers = details::base_webers<Rep, std::giga>;

		template<class Rep>
		using basic_terawebers = details::base_webers<Rep, std::tera>;

		template<class Rep>
		using basic_exawebers = details::base_webers<Rep, std::exa>;

		template<class Rep>
		using basic_webers = details::base_webers<Rep, std::ratio<1>>;

		using attowebers = basic_attowebers<std::intmax_t>;
		using femtowebers = basic_femtowebers<std::intmax_t>;
		using picowebers = basic_picowebers<std::intmax_t>;
		using nanowebers = basic_nanowebers<std::intmax_t>;
		using microwebers = basic_microwebers<std::intmax_t>;
		using milliwebers = basic_milliwebers<std::intmax_t>;
		using centiwebers = basic_centiwebers<std::intmax_t>;
		using deciwebers = basic_deciwebers<std::intmax_t>;
		using decawebers = basic_decawebers<std::intmax_t>;
		using hectowebers = basic_hectowebers<std::intmax_t>;
		using kilowebers = basic_kilowebers<std::intmax_t>;
		using megawebers = basic_megawebers<std::intmax_t>;
		using gigawebers = basic_gigawebers<std::intmax_t>;
		using terawebers = basic_terawebers<std::intmax_t>;
		using exawebers = basic_exawebers<std::intmax_t>;
		using webers = basic_webers<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_webers<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Wb"))};
		};
	}
}
