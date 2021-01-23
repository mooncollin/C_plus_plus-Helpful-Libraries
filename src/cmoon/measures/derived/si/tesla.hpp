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
			using base_teslas = magnetic_induction<Rep, Ratio, basic_kilograms<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attoteslas = details::base_teslas<Rep, std::atto>;

		template<class Rep>
		using basic_femtoteslas = details::base_teslas<Rep, std::femto>;

		template<class Rep>
		using basic_picoteslas = details::base_teslas<Rep, std::pico>;

		template<class Rep>
		using basic_nanoteslas = details::base_teslas<Rep, std::nano>;

		template<class Rep>
		using basic_microteslas = details::base_teslas<Rep, std::micro>;

		template<class Rep>
		using basic_milliteslas = details::base_teslas<Rep, std::milli>;

		template<class Rep>
		using basic_centiteslas = details::base_teslas<Rep, std::centi>;

		template<class Rep>
		using basic_deciteslas = details::base_teslas<Rep, std::deci>;

		template<class Rep>
		using basic_decateslas = details::base_teslas<Rep, std::deca>;

		template<class Rep>
		using basic_hectoteslas = details::base_teslas<Rep, std::hecto>;

		template<class Rep>
		using basic_kiloteslas = details::base_teslas<Rep, std::kilo>;

		template<class Rep>
		using basic_megateslas = details::base_teslas<Rep, std::mega>;

		template<class Rep>
		using basic_gigateslas = details::base_teslas<Rep, std::giga>;

		template<class Rep>
		using basic_terateslas = details::base_teslas<Rep, std::tera>;

		template<class Rep>
		using basic_exateslas = details::base_teslas<Rep, std::exa>;

		template<class Rep>
		using basic_teslas = details::base_teslas<Rep, std::ratio<1>>;

		using attoteslas = basic_attoteslas<std::intmax_t>;
		using femtoteslas = basic_femtoteslas<std::intmax_t>;
		using picoteslas = basic_picoteslas<std::intmax_t>;
		using nanoteslas = basic_nanoteslas<std::intmax_t>;
		using microteslas = basic_microteslas<std::intmax_t>;
		using milliteslas = basic_milliteslas<std::intmax_t>;
		using centiteslas = basic_centiteslas<std::intmax_t>;
		using deciteslas = basic_deciteslas<std::intmax_t>;
		using decateslas = basic_decateslas<std::intmax_t>;
		using hectoteslas = basic_hectoteslas<std::intmax_t>;
		using kiloteslas = basic_kiloteslas<std::intmax_t>;
		using megateslas = basic_megateslas<std::intmax_t>;
		using gigateslas = basic_gigateslas<std::intmax_t>;
		using terateslas = basic_terateslas<std::intmax_t>;
		using exateslas = basic_exateslas<std::intmax_t>;
		using teslas = basic_teslas<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_teslas<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("T"))};
		};
	}
}
