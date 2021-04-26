module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.hertz;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	struct base_hertz : public frequency<Rep, Ratio>
	{
		using frequency<Rep, Ratio>::frequency;
	};

	export
	template<class Rep>
	using basic_attohertz = base_hertz<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtohertz = base_hertz<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picohertz = base_hertz<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanohertz = base_hertz<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microhertz = base_hertz<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millihertz = base_hertz<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centihertz = base_hertz<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decihertz = base_hertz<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decahertz = base_hertz<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectohertz = base_hertz<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilohertz = base_hertz<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megahertz = base_hertz<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigahertz = base_hertz<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terahertz = base_hertz<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exahertz = base_hertz<Rep, std::exa>;

	export
	template<class Rep>
	using basic_hertz = base_hertz<Rep, std::ratio<1>>;

	export
	using attohertz = basic_attohertz<std::intmax_t>;
	
	export
	using femtohertz = basic_femtohertz<std::intmax_t>;
	
	export
	using picohertz = basic_picohertz<std::intmax_t>;
	
	export
	using nanohertz = basic_nanohertz<std::intmax_t>;
	
	export
	using microhertz = basic_microhertz<std::intmax_t>;
	
	export
	using millihertz = basic_millihertz<std::intmax_t>;
	
	export
	using centihertz = basic_centihertz<std::intmax_t>;
	
	export
	using decihertz = basic_decihertz<std::intmax_t>;
	
	export
	using decahertz = basic_decahertz<std::intmax_t>;
	
	export
	using hectohertz = basic_hectohertz<std::intmax_t>;
	
	export
	using kilohertz = basic_kilohertz<std::intmax_t>;
	
	export
	using megahertz = basic_megahertz<std::intmax_t>;
	
	export
	using gigahertz = basic_gigahertz<std::intmax_t>;
	
	export
	using terahertz = basic_terahertz<std::intmax_t>;
	
	export
	using exahertz = basic_exahertz<std::intmax_t>;
	
	export
	using hertz = basic_hertz<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_hertz<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Hz"))};
	};
}
