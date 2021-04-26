module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.henry;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_henrys = electrical_inductance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attohenrys = base_henrys<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtohenrys = base_henrys<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picohenrys = base_henrys<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanohenrys = base_henrys<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microhenrys = base_henrys<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millihenrys = base_henrys<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centihenrys = base_henrys<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decihenrys = base_henrys<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decahenrys = base_henrys<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectohenrys = base_henrys<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilohenrys = base_henrys<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megahenrys = base_henrys<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigahenrys = base_henrys<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terahenrys = base_henrys<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exahenrys = base_henrys<Rep, std::exa>;

	export
	template<class Rep>
	using basic_henrys = base_henrys<Rep, std::ratio<1>>;

	export
	using attohenrys = basic_attohenrys<std::intmax_t>;
	
	export
	using femtohenrys = basic_femtohenrys<std::intmax_t>;
	
	export
	using picohenrys = basic_picohenrys<std::intmax_t>;
	
	export
	using nanohenrys = basic_nanohenrys<std::intmax_t>;
	
	export
	using microhenrys = basic_microhenrys<std::intmax_t>;
	
	export
	using millihenrys = basic_millihenrys<std::intmax_t>;
	
	export
	using centihenrys = basic_centihenrys<std::intmax_t>;
	
	export
	using decihenrys = basic_decihenrys<std::intmax_t>;
	
	export
	using decahenrys = basic_decahenrys<std::intmax_t>;
	
	export
	using hectohenrys = basic_hectohenrys<std::intmax_t>;
	
	export
	using kilohenrys = basic_kilohenrys<std::intmax_t>;
	
	export
	using megahenrys = basic_megahenrys<std::intmax_t>;
	
	export
	using gigahenrys = basic_gigahenrys<std::intmax_t>;
	
	export
	using terahenrys = basic_terahenrys<std::intmax_t>;
	
	export
	using exahenrys = basic_exahenrys<std::intmax_t>;
	
	export
	using henrys = basic_henrys<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_henrys<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("H"))};
	};
}
