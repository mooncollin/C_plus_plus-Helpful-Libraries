module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.weber;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	namespace details
	{
		template<class Rep, cmoon::ratio_type Ratio>
		using base_webers = magnetic_flux<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
	}

	export
	template<class Rep>
	using basic_attowebers = base_webers<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtowebers = base_webers<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picowebers = base_webers<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanowebers = base_webers<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microwebers = base_webers<Rep, std::micro>;

	export
	template<class Rep>
	using basic_milliwebers = base_webers<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centiwebers = base_webers<Rep, std::centi>;

	export
	template<class Rep>
	using basic_deciwebers = base_webers<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decawebers = base_webers<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectowebers = base_webers<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilowebers = base_webers<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megawebers = base_webers<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigawebers = base_webers<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terawebers = base_webers<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exawebers = base_webers<Rep, std::exa>;

	export
	template<class Rep>
	using basic_webers = base_webers<Rep, std::ratio<1>>;

	export
	using attowebers = basic_attowebers<std::intmax_t>;
	
	export
	using femtowebers = basic_femtowebers<std::intmax_t>;
	
	export
	using picowebers = basic_picowebers<std::intmax_t>;
	
	export
	using nanowebers = basic_nanowebers<std::intmax_t>;
	
	export
	using microwebers = basic_microwebers<std::intmax_t>;
	
	export
	using milliwebers = basic_milliwebers<std::intmax_t>;
	
	export
	using centiwebers = basic_centiwebers<std::intmax_t>;
	
	export
	using deciwebers = basic_deciwebers<std::intmax_t>;
	
	export
	using decawebers = basic_decawebers<std::intmax_t>;
	
	export
	using hectowebers = basic_hectowebers<std::intmax_t>;
	
	export
	using kilowebers = basic_kilowebers<std::intmax_t>;
	
	export
	using megawebers = basic_megawebers<std::intmax_t>;
	
	export
	using gigawebers = basic_gigawebers<std::intmax_t>;
	
	export
	using terawebers = basic_terawebers<std::intmax_t>;
	
	export
	using exawebers = basic_exawebers<std::intmax_t>;
	
	export
	using webers = basic_webers<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_webers<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Wb"))};
	};
}
