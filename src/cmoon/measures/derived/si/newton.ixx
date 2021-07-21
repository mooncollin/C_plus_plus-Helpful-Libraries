module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.newton;

import <string_view>;
import <ratio>;

import cmoon.string;

import cmoon.measures;
import cmoon.measures.mass;
import cmoon.measures.distance;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, class Ratio>
	using base_newtons = force<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;

	export
	template<class Rep>
	using basic_attonewtons = base_newtons<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtonewtons = base_newtons<Rep, std::femto>;

	export
	template<class Rep>
	using basic_piconewtons = base_newtons<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanonewtons = base_newtons<Rep, std::nano>;

	export
	template<class Rep>
	using basic_micronewtons = base_newtons<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millinewtons = base_newtons<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centinewtons = base_newtons<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decinewtons = base_newtons<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decanewtons = base_newtons<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectonewtons = base_newtons<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilonewtons = base_newtons<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_meganewtons = base_newtons<Rep, std::mega>;

	export
	template<class Rep>
	using basic_giganewtons = base_newtons<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teranewtons = base_newtons<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exanewtons = base_newtons<Rep, std::exa>;

	export
	template<class Rep>
	using basic_newtons = base_newtons<Rep, std::ratio<1>>;

	export
	using attonewtons = basic_attonewtons<std::intmax_t>;
	
	export
	using femtonewtons = basic_femtonewtons<std::intmax_t>;
	
	export
	using piconewtons = basic_piconewtons<std::intmax_t>;
	
	export
	using nanonewtons = basic_nanonewtons<std::intmax_t>;
	
	export
	using micronewtons = basic_micronewtons<std::intmax_t>;
	
	export
	using millinewtons = basic_millinewtons<std::intmax_t>;
	
	export
	using centinewtons = basic_centinewtons<std::intmax_t>;
	
	export
	using decinewtons = basic_decinewtons<std::intmax_t>;
	
	export
	using decanewtons = basic_decanewtons<std::intmax_t>;
	
	export
	using hectonewtons = basic_hectonewtons<std::intmax_t>;
	
	export
	using kilonewtons = basic_kilonewtons<std::intmax_t>;
	
	export
	using meganewtons = basic_meganewtons<std::intmax_t>;
	
	export
	using giganewtons = basic_giganewtons<std::intmax_t>;
	
	export
	using teranewtons = basic_teranewtons<std::intmax_t>;
	
	export
	using exanewtons = basic_exanewtons<std::intmax_t>;
	
	export
	using newtons = basic_newtons<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_newtons<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("N"))};
	};
}
