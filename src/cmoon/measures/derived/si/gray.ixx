module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.gray;

import <string_view>;
import <ratio>;

import cmoon.string;

import cmoon.measures;
import cmoon.measures.distance;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, class Ratio>
	using base_gray = absorbed_dose_radiation<Rep, Ratio, basic_meters<Rep>>;

	export
	template<class Rep>
	using basic_attogray = base_gray<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtogray = base_gray<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picogray = base_gray<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanogray = base_gray<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microgray = base_gray<Rep, std::micro>;

	export
	template<class Rep>
	using basic_milligray = base_gray<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centigray = base_gray<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decigray = base_gray<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decagray = base_gray<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectogray = base_gray<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilogray = base_gray<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megagray = base_gray<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigagray = base_gray<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teragray = base_gray<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exagray = base_gray<Rep, std::exa>;

	export
	template<class Rep>
	using basic_gray = base_gray<Rep, std::ratio<1>>;

	export
	using attogray = basic_attogray<std::intmax_t>;
	
	export
	using femtogray = basic_femtogray<std::intmax_t>;
	
	export
	using picogray = basic_picogray<std::intmax_t>;
	
	export
	using nanogray = basic_nanogray<std::intmax_t>;
	
	export
	using microgray = basic_microgray<std::intmax_t>;
	
	export
	using milligray = basic_milligray<std::intmax_t>;
	
	export
	using centigray = basic_centigray<std::intmax_t>;
	
	export
	using decigray = basic_decigray<std::intmax_t>;
	
	export
	using decagray = basic_decagray<std::intmax_t>;
	
	export
	using hectogray = basic_hectogray<std::intmax_t>;
	
	export
	using kilogray = basic_kilogray<std::intmax_t>;
	
	export
	using megagray = basic_megagray<std::intmax_t>;
	
	export
	using gigagray = basic_gigagray<std::intmax_t>;
	
	export
	using teragray = basic_teragray<std::intmax_t>;
	
	export
	using exagray = basic_exagray<std::intmax_t>;
	
	export
	using gray = basic_gray<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_gray<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Gy"))};
	};
}
