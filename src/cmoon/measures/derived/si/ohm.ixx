module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.ohm;

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
	using base_ohms = electrical_resistance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attoohms = base_ohms<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtoohms = base_ohms<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picoohms = base_ohms<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanoohms = base_ohms<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microohms = base_ohms<Rep, std::micro>;

	export
	template<class Rep>
	using basic_milliohms = base_ohms<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centiohms = base_ohms<Rep, std::centi>;

	export
	template<class Rep>
	using basic_deciohms = base_ohms<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decaohms = base_ohms<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectoohms = base_ohms<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kiloohms = base_ohms<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megaohms = base_ohms<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigaohms = base_ohms<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teraohms = base_ohms<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exaohms = base_ohms<Rep, std::exa>;

	export
	template<class Rep>
	using basic_ohms = base_ohms<Rep, std::ratio<1>>;

	export
	using attoohms = basic_attoohms<std::intmax_t>;
	
	export
	using femtoohms = basic_femtoohms<std::intmax_t>;
	
	export
	using picoohms = basic_picoohms<std::intmax_t>;
	
	export
	using nanoohms = basic_nanoohms<std::intmax_t>;
	
	export
	using microohms = basic_microohms<std::intmax_t>;
	
	export
	using milliohms = basic_milliohms<std::intmax_t>;
	
	export
	using centiohms = basic_centiohms<std::intmax_t>;
	
	export
	using deciohms = basic_deciohms<std::intmax_t>;
	
	export
	using decaohms = basic_decaohms<std::intmax_t>;
	
	export
	using hectoohms = basic_hectoohms<std::intmax_t>;
	
	export
	using kiloohms = basic_kiloohms<std::intmax_t>;
	
	export
	using megaohms = basic_megaohms<std::intmax_t>;
	
	export
	using gigaohms = basic_gigaohms<std::intmax_t>;
	
	export
	using teraohms = basic_teraohms<std::intmax_t>;
	
	export
	using exaohms = basic_exaohms<std::intmax_t>;
	
	export
	using ohms = basic_ohms<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_ohms<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("O"))};
	};
}
