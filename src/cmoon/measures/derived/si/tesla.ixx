module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.tesla;

import <string_view>;
import <ratio>;

import cmoon.string;
import cmoon.ratio;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_teslas = magnetic_induction<Rep, Ratio, basic_kilograms<Rep>, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attoteslas = base_teslas<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtoteslas = base_teslas<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picoteslas = base_teslas<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanoteslas = base_teslas<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microteslas = base_teslas<Rep, std::micro>;

	export
	template<class Rep>
	using basic_milliteslas = base_teslas<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centiteslas = base_teslas<Rep, std::centi>;

	export
	template<class Rep>
	using basic_deciteslas = base_teslas<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decateslas = base_teslas<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectoteslas = base_teslas<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kiloteslas = base_teslas<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megateslas = base_teslas<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigateslas = base_teslas<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terateslas = base_teslas<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exateslas = base_teslas<Rep, std::exa>;

	export
	template<class Rep>
	using basic_teslas = base_teslas<Rep, std::ratio<1>>;

	export
	using attoteslas = basic_attoteslas<std::intmax_t>;
	
	export
	using femtoteslas = basic_femtoteslas<std::intmax_t>;
	
	export
	using picoteslas = basic_picoteslas<std::intmax_t>;
	
	export
	using nanoteslas = basic_nanoteslas<std::intmax_t>;
	
	export
	using microteslas = basic_microteslas<std::intmax_t>;
	
	export
	using milliteslas = basic_milliteslas<std::intmax_t>;
	
	export
	using centiteslas = basic_centiteslas<std::intmax_t>;
	
	export
	using deciteslas = basic_deciteslas<std::intmax_t>;
	
	export
	using decateslas = basic_decateslas<std::intmax_t>;
	
	export
	using hectoteslas = basic_hectoteslas<std::intmax_t>;
	
	export
	using kiloteslas = basic_kiloteslas<std::intmax_t>;
	
	export
	using megateslas = basic_megateslas<std::intmax_t>;
	
	export
	using gigateslas = basic_gigateslas<std::intmax_t>;
	
	export
	using terateslas = basic_terateslas<std::intmax_t>;
	
	export
	using exateslas = basic_exateslas<std::intmax_t>;
	
	export
	using teslas = basic_teslas<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_teslas<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("T"))};
	};
}
