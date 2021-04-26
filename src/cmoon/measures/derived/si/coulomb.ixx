module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.derived.coulomb;

import <string_view>;
import <ratio>;

import cmoon.measures;
import cmoon.measures.derived.base;

namespace cmoon::measures
{
	template<class Rep, cmoon::ratio_type Ratio>
	using base_coulombs = electric_charge<Rep, Ratio, basic_ampere<Rep>>;

	export
	template<class Rep>
	using basic_attocoulombs = base_coulombs<Rep, std::atto>;

	export
	template<class Rep>
	using basic_femtocoulombs = base_coulombs<Rep, std::femto>;

	export
	template<class Rep>
	using basic_picocoulombs = base_coulombs<Rep, std::pico>;

	export
	template<class Rep>
	using basic_nanocoulombs = base_coulombs<Rep, std::nano>;

	export
	template<class Rep>
	using basic_microcoulombs = base_coulombs<Rep, std::micro>;

	export
	template<class Rep>
	using basic_millicoulombs = base_coulombs<Rep, std::milli>;

	export
	template<class Rep>
	using basic_centicoulombs = base_coulombs<Rep, std::centi>;

	export
	template<class Rep>
	using basic_decicoulombs = base_coulombs<Rep, std::deci>;

	export
	template<class Rep>
	using basic_decacoulombs = base_coulombs<Rep, std::deca>;

	export
	template<class Rep>
	using basic_hectocoulombs = base_coulombs<Rep, std::hecto>;

	export
	template<class Rep>
	using basic_kilocoulombs = base_coulombs<Rep, std::kilo>;

	export
	template<class Rep>
	using basic_megacoulombs = base_coulombs<Rep, std::mega>;

	export
	template<class Rep>
	using basic_gigacoulombs = base_coulombs<Rep, std::giga>;

	export
	template<class Rep>
	using basic_teracoulombs = base_coulombs<Rep, std::tera>;

	export
	template<class Rep>
	using basic_exacoulombs = base_coulombs<Rep, std::exa>;

	export
	template<class Rep>
	using basic_coulombs = base_coulombs<Rep, std::ratio<1>>;

	export
	using attocoulombs = basic_attocoulombs<std::intmax_t>;
	
	export
	using femtocoulombs = basic_femtocoulombs<std::intmax_t>;
	
	export
	using picocoulombs = basic_picocoulombs<std::intmax_t>;
	
	export
	using nanocoulombs = basic_nanocoulombs<std::intmax_t>;
	
	export
	using microcoulombs = basic_microcoulombs<std::intmax_t>;
	
	export
	using millicoulombs = basic_millicoulombs<std::intmax_t>;
	
	export
	using centicoulombs = basic_centicoulombs<std::intmax_t>;
	
	export
	using decicoulombs = basic_decicoulombs<std::intmax_t>;
	
	export
	using decacoulombs = basic_decacoulombs<std::intmax_t>;
	
	export
	using hectocoulombs = basic_hectocoulombs<std::intmax_t>;
	
	export
	using kilocoulombs = basic_kilocoulombs<std::intmax_t>;
	
	export
	using megacoulombs = basic_megacoulombs<std::intmax_t>;
	
	export
	using gigacoulombs = basic_gigacoulombs<std::intmax_t>;
	
	export
	using teracoulombs = basic_teracoulombs<std::intmax_t>;
	
	export
	using exacoulombs = basic_exacoulombs<std::intmax_t>;
	
	export
	using coulombs = basic_coulombs<std::intmax_t>;

	export
	template<class Rep, cmoon::ratio_type Ratio, class CharT>
	struct suffix<base_coulombs<Rep, Ratio>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("C"))};
	};
}
