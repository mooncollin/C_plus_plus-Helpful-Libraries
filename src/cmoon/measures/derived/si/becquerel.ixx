module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.becquerel;

import <string_view>;
import <ratio>;

import cmoon.string;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, class Ratio>
	struct base_becquerels : public frequency<Rep, Ratio>
	{
		using frequency<Rep, Ratio>::frequency;
	};

	export
	template<class Rep>
	using basic_attobecquerels = base_becquerels<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtobecquerels = base_becquerels<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picobecquerels = base_becquerels<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanobecquerels = base_becquerels<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microbecquerels = base_becquerels<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millibecquerels = base_becquerels<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centibecquerels = base_becquerels<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decibecquerels = base_becquerels<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decabecquerels = base_becquerels<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectobecquerels = base_becquerels<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilobecquerels = base_becquerels<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megabecquerels = base_becquerels<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigabecquerels = base_becquerels<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terabecquerels = base_becquerels<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exabecquerels = base_becquerels<Rep, std::exa>;

	export
	template<class Rep>
	using basic_becquerels = base_becquerels<Rep, std::ratio<1>>;

	export
	using attobecquerels = basic_attobecquerels<std::intmax_t>;
	
	export
	using femtobecquerels = basic_femtobecquerels<std::intmax_t>;
	
	export
	using picobecquerels = basic_picobecquerels<std::intmax_t>;
	
	export
	using nanobecquerels = basic_nanobecquerels<std::intmax_t>;
	
	export
	using microbecquerels = basic_microbecquerels<std::intmax_t>;
	
	export
	using millibecquerels = basic_millibecquerels<std::intmax_t>;
	
	export
	using centibecquerels = basic_centibecquerels<std::intmax_t>;
	
	export
	using decibecquerels = basic_decibecquerels<std::intmax_t>;
	
	export
	using decabecquerels = basic_decabecquerels<std::intmax_t>;
	
	export
	using hectobecquerels = basic_hectobecquerels<std::intmax_t>;
	
	export
	using kilobecquerels = basic_kilobecquerels<std::intmax_t>;
	
	export
	using megabecquerels = basic_megabecquerels<std::intmax_t>;
	
	export
	using gigabecquerels = basic_gigabecquerels<std::intmax_t>;
	
	export
	using terabecquerels = basic_terabecquerels<std::intmax_t>;
	
	export
	using exabecquerels = basic_exabecquerels<std::intmax_t>;
	
	export
	using becquerels = basic_becquerels<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_becquerels<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Bq"))};
	};
}
