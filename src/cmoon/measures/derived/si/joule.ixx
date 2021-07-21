module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.joule;

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
	using base_joules = work<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;

	export
	template<class Rep>
	using basic_attojoules = base_joules<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtojoules = base_joules<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picojoules = base_joules<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanojoules = base_joules<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microjoules = base_joules<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millijoules = base_joules<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centijoules = base_joules<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decijoules = base_joules<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decajoules = base_joules<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectojoules = base_joules<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilojoules = base_joules<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megajoules = base_joules<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigajoules = base_joules<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terajoules = base_joules<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exajoules = base_joules<Rep, std::exa>;

	export
	template<class Rep>
	using basic_joules = base_joules<Rep, std::ratio<1>>;

	export
	using attojoules = basic_attojoules<std::intmax_t>;
	
	export
	using femtojoules = basic_femtojoules<std::intmax_t>;
	
	export
	using picojoules = basic_picojoules<std::intmax_t>;
	
	export
	using nanojoules = basic_nanojoules<std::intmax_t>;
	
	export
	using microjoules = basic_microjoules<std::intmax_t>;
	
	export
	using millijoules = basic_millijoules<std::intmax_t>;
	
	export
	using centijoules = basic_centijoules<std::intmax_t>;
	
	export
	using decijoules = basic_decijoules<std::intmax_t>;
	
	export
	using decajoules = basic_decajoules<std::intmax_t>;
	
	export
	using hectojoules = basic_hectojoules<std::intmax_t>;
	
	export
	using kilojoules = basic_kilojoules<std::intmax_t>;
	
	export
	using megajoules = basic_megajoules<std::intmax_t>;
	
	export
	using gigajoules = basic_gigajoules<std::intmax_t>;
	
	export
	using terajoules = basic_terajoules<std::intmax_t>;
	
	export
	using exajoules = basic_exajoules<std::intmax_t>;
	
	export
	using joules = basic_joules<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_joules<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("J"))};
	};
}
