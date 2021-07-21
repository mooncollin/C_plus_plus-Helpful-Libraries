module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.volt;

import <string_view>;
import <ratio>;

import cmoon.string;

import cmoon.measures;
import cmoon.measures.mass;
import cmoon.measures.distance;
import cmoon.measures.electric_current;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, class Ratio>
	using base_volts = voltage<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attovolts = base_volts<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtovolts = base_volts<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picovolts = base_volts<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanovolts = base_volts<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microvolts = base_volts<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millivolts = base_volts<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centivolts = base_volts<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decivolts = base_volts<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decavolts = base_volts<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectovolts = base_volts<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilovolts = base_volts<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megavolts = base_volts<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigavolts = base_volts<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teravolts = base_volts<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exavolts = base_volts<Rep, std::exa>;

	export
	template<class Rep>
	using basic_volts = base_volts<Rep, std::ratio<1>>;

	export
	using attovolts = basic_attovolts<std::intmax_t>;
	
	export
	using femtovolts = basic_femtovolts<std::intmax_t>;
	
	export
	using picovolts = basic_picovolts<std::intmax_t>;
	
	export
	using nanovolts = basic_nanovolts<std::intmax_t>;
	
	export
	using microvolts = basic_microvolts<std::intmax_t>;
	
	export
	using millivolts = basic_millivolts<std::intmax_t>;
	
	export
	using centivolts = basic_centivolts<std::intmax_t>;
	
	export
	using decivolts = basic_decivolts<std::intmax_t>;
	
	export
	using decavolts = basic_decavolts<std::intmax_t>;
	
	export
	using hectovolts = basic_hectovolts<std::intmax_t>;
	
	export
	using kilovolts = basic_kilovolts<std::intmax_t>;
	
	export
	using megavolts = basic_megavolts<std::intmax_t>;
	
	export
	using gigavolts = basic_gigavolts<std::intmax_t>;
	
	export
	using teravolts = basic_teravolts<std::intmax_t>;
	
	export
	using exavolts = basic_exavolts<std::intmax_t>;
	
	export
	using volts = basic_volts<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_volts<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("V"))};
	};
}
