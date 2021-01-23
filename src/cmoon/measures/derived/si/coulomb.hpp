#pragma once

#include <string_view>
#include <ratio>

#include "cmoon/string.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/measures/derived/base.hpp"

namespace cmoon
{
	namespace measures
	{
		namespace details
		{
			template<class Rep, cmoon::ratio_type Ratio>
			using base_coulombs = electric_charge<Rep, Ratio, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attocoulombs = details::base_coulombs<Rep, std::atto>;

		template<class Rep>
		using basic_femtocoulombs = details::base_coulombs<Rep, std::femto>;

		template<class Rep>
		using basic_picocoulombs = details::base_coulombs<Rep, std::pico>;

		template<class Rep>
		using basic_nanocoulombs = details::base_coulombs<Rep, std::nano>;

		template<class Rep>
		using basic_microcoulombs = details::base_coulombs<Rep, std::micro>;

		template<class Rep>
		using basic_millicoulombs = details::base_coulombs<Rep, std::milli>;

		template<class Rep>
		using basic_centicoulombs = details::base_coulombs<Rep, std::centi>;

		template<class Rep>
		using basic_decicoulombs = details::base_coulombs<Rep, std::deci>;

		template<class Rep>
		using basic_decacoulombs = details::base_coulombs<Rep, std::deca>;

		template<class Rep>
		using basic_hectocoulombs = details::base_coulombs<Rep, std::hecto>;

		template<class Rep>
		using basic_kilocoulombs = details::base_coulombs<Rep, std::kilo>;

		template<class Rep>
		using basic_megacoulombs = details::base_coulombs<Rep, std::mega>;

		template<class Rep>
		using basic_gigacoulombs = details::base_coulombs<Rep, std::giga>;

		template<class Rep>
		using basic_teracoulombs = details::base_coulombs<Rep, std::tera>;

		template<class Rep>
		using basic_exacoulombs = details::base_coulombs<Rep, std::exa>;

		template<class Rep>
		using basic_coulombs = details::base_coulombs<Rep, std::ratio<1>>;

		using attocoulombs = basic_attocoulombs<std::intmax_t>;
		using femtocoulombs = basic_femtocoulombs<std::intmax_t>;
		using picocoulombs = basic_picocoulombs<std::intmax_t>;
		using nanocoulombs = basic_nanocoulombs<std::intmax_t>;
		using microcoulombs = basic_microcoulombs<std::intmax_t>;
		using millicoulombs = basic_millicoulombs<std::intmax_t>;
		using centicoulombs = basic_centicoulombs<std::intmax_t>;
		using decicoulombs = basic_decicoulombs<std::intmax_t>;
		using decacoulombs = basic_decacoulombs<std::intmax_t>;
		using hectocoulombs = basic_hectocoulombs<std::intmax_t>;
		using kilocoulombs = basic_kilocoulombs<std::intmax_t>;
		using megacoulombs = basic_megacoulombs<std::intmax_t>;
		using gigacoulombs = basic_gigacoulombs<std::intmax_t>;
		using teracoulombs = basic_teracoulombs<std::intmax_t>;
		using exacoulombs = basic_exacoulombs<std::intmax_t>;
		using coulombs = basic_coulombs<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_coulombs<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("C"))};
		};
	}
}
