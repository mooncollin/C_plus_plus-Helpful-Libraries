module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.farad;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.mass;
import cmoon.measures.distance;
import cmoon.measures.electric_current;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_farads = electrical_capacitance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attofarads = base_farads<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtofarads = base_farads<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picofarads = base_farads<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanofarads = base_farads<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microfarads = base_farads<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millifarads = base_farads<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centifarads = base_farads<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decifarads = base_farads<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decafarads = base_farads<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectofarads = base_farads<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilofarads = base_farads<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megafarads = base_farads<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigafarads = base_farads<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terafarads = base_farads<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exafarads = base_farads<Rep, std::exa>;

	export
	template<class Rep>
	using basic_farads = base_farads<Rep, std::ratio<1>>;

	export
	using attofarads = basic_attofarads<std::intmax_t>;
	
	export
	using femtofarads = basic_femtofarads<std::intmax_t>;
	
	export
	using picofarads = basic_picofarads<std::intmax_t>;
	
	export
	using nanofarads = basic_nanofarads<std::intmax_t>;
	
	export
	using microfarads = basic_microfarads<std::intmax_t>;
	
	export
	using millifarads = basic_millifarads<std::intmax_t>;
	
	export
	using centifarads = basic_centifarads<std::intmax_t>;
	
	export
	using decifarads = basic_decifarads<std::intmax_t>;
	
	export
	using decafarads = basic_decafarads<std::intmax_t>;
	
	export
	using hectofarads = basic_hectofarads<std::intmax_t>;
	
	export
	using kilofarads = basic_kilofarads<std::intmax_t>;
	
	export
	using megafarads = basic_megafarads<std::intmax_t>;
	
	export
	using gigafarads = basic_gigafarads<std::intmax_t>;
	
	export
	using terafarads = basic_terafarads<std::intmax_t>;
	
	export
	using exafarads = basic_exafarads<std::intmax_t>;
	
	export
	using farads = basic_farads<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_farads<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("F"))};
	};
}
