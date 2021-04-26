module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.katal;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_katals = catalytic_activity<Rep, Ratio, basic_mole<Rep>>;

	export
	template<class Rep>
	using basic_attokatals = base_katals<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtokatals = base_katals<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picokatals = base_katals<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanokatals = base_katals<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microkatals = base_katals<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millikatals = base_katals<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centikatals = base_katals<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decikatals = base_katals<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decakatals = base_katals<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectokatals = base_katals<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilokatals = base_katals<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megakatals = base_katals<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigakatals = base_katals<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terakatals = base_katals<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exakatals = base_katals<Rep, std::exa>;

	export
	template<class Rep>
	using basic_katals = base_katals<Rep, std::ratio<1>>;

	export
	using attokatals = basic_attokatals<std::intmax_t>;
	
	export
	using femtokatals = basic_femtokatals<std::intmax_t>;
	
	export
	using picokatals = basic_picokatals<std::intmax_t>;
	
	export
	using nanokatals = basic_nanokatals<std::intmax_t>;
	
	export
	using microkatals = basic_microkatals<std::intmax_t>;
	
	export
	using millikatals = basic_millikatals<std::intmax_t>;
	
	export
	using centikatals = basic_centikatals<std::intmax_t>;
	
	export
	using decikatals = basic_decikatals<std::intmax_t>;
	
	export
	using decakatals = basic_decakatals<std::intmax_t>;
	
	export
	using hectokatals = basic_hectokatals<std::intmax_t>;
	
	export
	using kilokatals = basic_kilokatals<std::intmax_t>;
	
	export
	using megakatals = basic_megakatals<std::intmax_t>;
	
	export
	using gigakatals = basic_gigakatals<std::intmax_t>;
	
	export
	using terakatals = basic_terakatals<std::intmax_t>;
	
	export
	using exakatals = basic_exakatals<std::intmax_t>;
	
	export
	using katals = basic_katals<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_katals<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("kat"))};
	};
}
