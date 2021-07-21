module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.watt;

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
	using base_watts = power<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;

	export
	template<class Rep>
	using basic_attowatts = base_watts<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtowatts = base_watts<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picowatts = base_watts<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanowatts = base_watts<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microwatts = base_watts<Rep, std::micro>;

	export
	template<class Rep>
	using basic_milliwatts = base_watts<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centiwatts = base_watts<Rep, std::centi>;

	export
	template<class Rep>
	using basic_deciwatts = base_watts<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decawatts = base_watts<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectowatts = base_watts<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilowatts = base_watts<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megawatts = base_watts<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigawatts = base_watts<Rep, std::giga>;

	export
	template<class Rep>
	using basic_terawatts = base_watts<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exawatts = base_watts<Rep, std::exa>;

	export
	template<class Rep>
	using basic_watts = base_watts<Rep, std::ratio<1>>;

	export
	using attowatts = basic_attowatts<std::intmax_t>;
	
	export
	using femtowatts = basic_femtowatts<std::intmax_t>;
	
	export
	using picowatts = basic_picowatts<std::intmax_t>;
	
	export
	using nanowatts = basic_nanowatts<std::intmax_t>;
	
	export
	using microwatts = basic_microwatts<std::intmax_t>;
	
	export
	using milliwatts = basic_milliwatts<std::intmax_t>;
	
	export
	using centiwatts = basic_centiwatts<std::intmax_t>;
	
	export
	using deciwatts = basic_deciwatts<std::intmax_t>;
	
	export
	using decawatts = basic_decawatts<std::intmax_t>;
	
	export
	using hectowatts = basic_hectowatts<std::intmax_t>;
	
	export
	using kilowatts = basic_kilowatts<std::intmax_t>;
	
	export
	using megawatts = basic_megawatts<std::intmax_t>;
	
	export
	using gigawatts = basic_gigawatts<std::intmax_t>;
	
	export
	using terawatts = basic_terawatts<std::intmax_t>;
	
	export
	using exawatts = basic_exawatts<std::intmax_t>;
	
	export
	using watts = basic_watts<std::intmax_t>;

	export
	template<class Rep, class Ratio, class CharT>
	struct suffix<base_watts<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("W"))};
	};
}
