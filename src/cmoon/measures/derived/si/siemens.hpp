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
			using base_siemens = electrical_conductance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attosiemens = details::base_siemens<Rep, std::atto>;

		template<class Rep>
		using basic_femtosiemens = details::base_siemens<Rep, std::femto>;

		template<class Rep>
		using basic_picosiemens = details::base_siemens<Rep, std::pico>;

		template<class Rep>
		using basic_nanosiemens = details::base_siemens<Rep, std::nano>;

		template<class Rep>
		using basic_microsiemens = details::base_siemens<Rep, std::micro>;

		template<class Rep>
		using basic_millisiemens = details::base_siemens<Rep, std::milli>;

		template<class Rep>
		using basic_centisiemens = details::base_siemens<Rep, std::centi>;

		template<class Rep>
		using basic_decisiemens = details::base_siemens<Rep, std::deci>;

		template<class Rep>
		using basic_decasiemens = details::base_siemens<Rep, std::deca>;

		template<class Rep>
		using basic_hectosiemens = details::base_siemens<Rep, std::hecto>;

		template<class Rep>
		using basic_kilosiemens = details::base_siemens<Rep, std::kilo>;

		template<class Rep>
		using basic_megasiemens = details::base_siemens<Rep, std::mega>;

		template<class Rep>
		using basic_gigasiemens = details::base_siemens<Rep, std::giga>;

		template<class Rep>
		using basic_terasiemens = details::base_siemens<Rep, std::tera>;

		template<class Rep>
		using basic_exasiemens = details::base_siemens<Rep, std::exa>;

		template<class Rep>
		using basic_siemens = details::base_siemens<Rep, std::ratio<1>>;

		using attosiemens = basic_attosiemens<std::intmax_t>;
		using femtosiemens = basic_femtosiemens<std::intmax_t>;
		using picosiemens = basic_picosiemens<std::intmax_t>;
		using nanosiemens = basic_nanosiemens<std::intmax_t>;
		using microsiemens = basic_microsiemens<std::intmax_t>;
		using millisiemens = basic_millisiemens<std::intmax_t>;
		using centisiemens = basic_centisiemens<std::intmax_t>;
		using decisiemens = basic_decisiemens<std::intmax_t>;
		using decasiemens = basic_decasiemens<std::intmax_t>;
		using hectosiemens = basic_hectosiemens<std::intmax_t>;
		using kilosiemens = basic_kilosiemens<std::intmax_t>;
		using megasiemens = basic_megasiemens<std::intmax_t>;
		using gigasiemens = basic_gigasiemens<std::intmax_t>;
		using terasiemens = basic_terasiemens<std::intmax_t>;
		using exasiemens = basic_exasiemens<std::intmax_t>;
		using siemens = basic_siemens<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_siemens<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("S"))};
		};
	}
}
