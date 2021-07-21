module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.lux;

import <string_view>;
import <ratio>;

import cmoon.string;

import cmoon.measures;
import cmoon.measures.distance;
import cmoon.measures.derived.base;
import cmoon.measures.derived.lumen;

namespace cmoon::measures
{
	template<class Rep, class Ratio>
	using base_lux = illuminance<Rep, Ratio, basic_lumens<Rep>, basic_meters<Rep>>;

	export
	template<class Rep>
	using basic_attolux = base_lux<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtolux = base_lux<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picolux = base_lux<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanolux = base_lux<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microlux = base_lux<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millilux = base_lux<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centilux = base_lux<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decilux = base_lux<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decalux = base_lux<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectolux = base_lux<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilolux = base_lux<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megalux = base_lux<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigalux = base_lux<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teralux = base_lux<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exalux = base_lux<Rep, std::exa>;

	export
	template<class Rep>
	using basic_lux = base_lux<Rep, std::ratio<1>>;

	export
	using attolux = basic_attolux<std::intmax_t>;
	
	export
	using femtolux = basic_femtolux<std::intmax_t>;
	
	export
	using picolux = basic_picolux<std::intmax_t>;
	
	export
	using nanolux = basic_nanolux<std::intmax_t>;
	
	export
	using microlux = basic_microlux<std::intmax_t>;
	
	export
	using millilux = basic_millilux<std::intmax_t>;
	
	export
	using centilux = basic_centilux<std::intmax_t>;
	
	export
	using decilux = basic_decilux<std::intmax_t>;
	
	export
	using decalux = basic_decalux<std::intmax_t>;
	
	export
	using hectolux = basic_hectolux<std::intmax_t>;
	
	export
	using kilolux = basic_kilolux<std::intmax_t>;
	
	export
	using megalux = basic_megalux<std::intmax_t>;
	
	export
	using gigalux = basic_gigalux<std::intmax_t>;
	
	export
	using teralux = basic_teralux<std::intmax_t>;
	
	export
	using exalux = basic_exalux<std::intmax_t>;
	
	export
	using lux = basic_lux<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_lux<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("lx"))};
	};
}
