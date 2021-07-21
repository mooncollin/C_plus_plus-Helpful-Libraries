module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.lumen;

import <string_view>;
import <ratio>;

import cmoon.string;

import cmoon.measures;
import cmoon.measures.luminous;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, class Ratio>
	struct base_lumens : public luminous_intensity<Rep, metric_system, Ratio, 1>
	{
		using luminous_intensity<Rep, metric_system, Ratio, 1>::luminous_intensity;
	};

	export
	template<class Rep>
	using basic_attolumens = base_lumens<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtolumens = base_lumens<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picolumens = base_lumens<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanolumens = base_lumens<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microlumens = base_lumens<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millilumens = base_lumens<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centilumens = base_lumens<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decilumens = base_lumens<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decalumens = base_lumens<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectolumens = base_lumens<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilolumens = base_lumens<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megalumens = base_lumens<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigalumens = base_lumens<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teralumens = base_lumens<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exalumens = base_lumens<Rep, std::exa>;

	export
	template<class Rep>
	using basic_lumens = base_lumens<Rep, std::ratio<1>>;

	export
	using attolumens = basic_attolumens<std::intmax_t>;
	
	export
	using femtolumens = basic_femtolumens<std::intmax_t>;
	
	export
	using picolumens = basic_picolumens<std::intmax_t>;
	
	export
	using nanolumens = basic_nanolumens<std::intmax_t>;
	
	export
	using microlumens = basic_microlumens<std::intmax_t>;
	
	export
	using millilumens = basic_millilumens<std::intmax_t>;
	
	export
	using centilumens = basic_centilumens<std::intmax_t>;
	
	export
	using decilumens = basic_decilumens<std::intmax_t>;
	
	export
	using decalumens = basic_decalumens<std::intmax_t>;
	
	export
	using hectolumens = basic_hectolumens<std::intmax_t>;
	
	export
	using kilolumens = basic_kilolumens<std::intmax_t>;
	
	export
	using megalumens = basic_megalumens<std::intmax_t>;
	
	export
	using gigalumens = basic_gigalumens<std::intmax_t>;
	
	export
	using teralumens = basic_teralumens<std::intmax_t>;
	
	export
	using exalumens = basic_exalumens<std::intmax_t>;
	
	export
	using lumens = basic_lumens<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_lumens<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("lm"))};
	};
}
