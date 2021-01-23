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
			using base_pascals = pressure<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;
		}

		template<class Rep>
		using basic_attopascals = details::base_pascals<Rep, std::atto>;

		template<class Rep>
		using basic_femtopascals = details::base_pascals<Rep, std::femto>;

		template<class Rep>
		using basic_picopascals = details::base_pascals<Rep, std::pico>;

		template<class Rep>
		using basic_nanopascals = details::base_pascals<Rep, std::nano>;

		template<class Rep>
		using basic_micropascals = details::base_pascals<Rep, std::micro>;

		template<class Rep>
		using basic_millipascals = details::base_pascals<Rep, std::milli>;

		template<class Rep>
		using basic_centipascals = details::base_pascals<Rep, std::centi>;

		template<class Rep>
		using basic_decipascals = details::base_pascals<Rep, std::deci>;

		template<class Rep>
		using basic_decapascals = details::base_pascals<Rep, std::deca>;

		template<class Rep>
		using basic_hectopascals = details::base_pascals<Rep, std::hecto>;

		template<class Rep>
		using basic_kilopascals = details::base_pascals<Rep, std::kilo>;

		template<class Rep>
		using basic_megapascals = details::base_pascals<Rep, std::mega>;

		template<class Rep>
		using basic_gigapascals = details::base_pascals<Rep, std::giga>;

		template<class Rep>
		using basic_terapascals = details::base_pascals<Rep, std::tera>;

		template<class Rep>
		using basic_exapascals = details::base_pascals<Rep, std::exa>;

		template<class Rep>
		using basic_pascals = details::base_pascals<Rep, std::ratio<1>>;

		using attopascals = basic_attopascals<std::intmax_t>;
		using femtopascals = basic_femtopascals<std::intmax_t>;
		using picopascals = basic_picopascals<std::intmax_t>;
		using nanopascals = basic_nanopascals<std::intmax_t>;
		using micropascals = basic_micropascals<std::intmax_t>;
		using millipascals = basic_millipascals<std::intmax_t>;
		using centipascals = basic_centipascals<std::intmax_t>;
		using decipascals = basic_decipascals<std::intmax_t>;
		using decapascals = basic_decapascals<std::intmax_t>;
		using hectopascals = basic_hectopascals<std::intmax_t>;
		using kilopascals = basic_kilopascals<std::intmax_t>;
		using megapascals = basic_megapascals<std::intmax_t>;
		using gigapascals = basic_gigapascals<std::intmax_t>;
		using terapascals = basic_terapascals<std::intmax_t>;
		using exapascals = basic_exapascals<std::intmax_t>;
		using pascals = basic_pascals<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_pascals<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Pa"))};
		};
	}
}