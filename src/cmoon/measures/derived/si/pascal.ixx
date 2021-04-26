module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.pascal;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_pascals = pressure<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;

	export
	template<class Rep>
	using basic_attopascals = base_pascals<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtopascals = base_pascals<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picopascals = base_pascals<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanopascals = base_pascals<Rep, std::nano>;

	export
	template<class Rep>
	using basic_micropascals = base_pascals<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millipascals = base_pascals<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centipascals = base_pascals<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decipascals = base_pascals<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decapascals = base_pascals<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectopascals = base_pascals<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilopascals = base_pascals<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megapascals = base_pascals<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigapascals = base_pascals<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terapascals = base_pascals<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exapascals = base_pascals<Rep, std::exa>;

	export
	template<class Rep>
	using basic_pascals = base_pascals<Rep, std::ratio<1>>;

	export
	using attopascals = basic_attopascals<std::intmax_t>;
	
	export
	using femtopascals = basic_femtopascals<std::intmax_t>;
	
	export
	using picopascals = basic_picopascals<std::intmax_t>;
	
	export
	using nanopascals = basic_nanopascals<std::intmax_t>;
	
	export
	using micropascals = basic_micropascals<std::intmax_t>;
	
	export
	using millipascals = basic_millipascals<std::intmax_t>;
	
	export
	using centipascals = basic_centipascals<std::intmax_t>;
	
	export
	using decipascals = basic_decipascals<std::intmax_t>;
	
	export
	using decapascals = basic_decapascals<std::intmax_t>;
	
	export
	using hectopascals = basic_hectopascals<std::intmax_t>;
	
	export
	using kilopascals = basic_kilopascals<std::intmax_t>;
	
	export
	using megapascals = basic_megapascals<std::intmax_t>;
	
	export
	using gigapascals = basic_gigapascals<std::intmax_t>;
	
	export
	using terapascals = basic_terapascals<std::intmax_t>;
	
	export
	using exapascals = basic_exapascals<std::intmax_t>;
	
	export
	using pascals = basic_pascals<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_pascals<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Pa"))};
	};
}