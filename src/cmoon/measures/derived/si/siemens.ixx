module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.siemens;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_siemens = electrical_conductance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attosiemens = base_siemens<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtosiemens = base_siemens<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picosiemens = base_siemens<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanosiemens = base_siemens<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microsiemens = base_siemens<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millisiemens = base_siemens<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centisiemens = base_siemens<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decisiemens = base_siemens<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decasiemens = base_siemens<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectosiemens = base_siemens<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilosiemens = base_siemens<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megasiemens = base_siemens<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigasiemens = base_siemens<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terasiemens = base_siemens<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exasiemens = base_siemens<Rep, std::exa>;

	export
	template<class Rep>
	using basic_siemens = base_siemens<Rep, std::ratio<1>>;

	export
	using attosiemens = basic_attosiemens<std::intmax_t>;
	
	export
	using femtosiemens = basic_femtosiemens<std::intmax_t>;
	
	export
	using picosiemens = basic_picosiemens<std::intmax_t>;
	
	export
	using nanosiemens = basic_nanosiemens<std::intmax_t>;
	
	export
	using microsiemens = basic_microsiemens<std::intmax_t>;
	
	export
	using millisiemens = basic_millisiemens<std::intmax_t>;
	
	export
	using centisiemens = basic_centisiemens<std::intmax_t>;
	
	export
	using decisiemens = basic_decisiemens<std::intmax_t>;
	
	export
	using decasiemens = basic_decasiemens<std::intmax_t>;
	
	export
	using hectosiemens = basic_hectosiemens<std::intmax_t>;
	
	export
	using kilosiemens = basic_kilosiemens<std::intmax_t>;
	
	export
	using megasiemens = basic_megasiemens<std::intmax_t>;
	
	export
	using gigasiemens = basic_gigasiemens<std::intmax_t>;
	
	export
	using terasiemens = basic_terasiemens<std::intmax_t>;
	
	export
	using exasiemens = basic_exasiemens<std::intmax_t>;
	
	export
	using siemens = basic_siemens<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_siemens<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("S"))};
	};
}
