#pragma once

#include <string_view>
#include <ratio>

#include "cmoon/string.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/measures/derived/base.hpp"
#include "cmoon/measures/derived/si/lumen.hpp"

namespace cmoon
{
	namespace measures
	{
		namespace details
		{
			template<class Rep, cmoon::ratio_type Ratio>
			using base_lux = illuminance<Rep, Ratio, basic_lumens<Rep>, basic_meters<Rep>>;
		}

		template<class Rep>
		using basic_attolux = details::base_lux<Rep, std::atto>;

		template<class Rep>
		using basic_femtolux = details::base_lux<Rep, std::femto>;

		template<class Rep>
		using basic_picolux = details::base_lux<Rep, std::pico>;

		template<class Rep>
		using basic_nanolux = details::base_lux<Rep, std::nano>;

		template<class Rep>
		using basic_microlux = details::base_lux<Rep, std::micro>;

		template<class Rep>
		using basic_millilux = details::base_lux<Rep, std::milli>;

		template<class Rep>
		using basic_centilux = details::base_lux<Rep, std::centi>;

		template<class Rep>
		using basic_decilux = details::base_lux<Rep, std::deci>;

		template<class Rep>
		using basic_decalux = details::base_lux<Rep, std::deca>;

		template<class Rep>
		using basic_hectolux = details::base_lux<Rep, std::hecto>;

		template<class Rep>
		using basic_kilolux = details::base_lux<Rep, std::kilo>;

		template<class Rep>
		using basic_megalux = details::base_lux<Rep, std::mega>;

		template<class Rep>
		using basic_gigalux = details::base_lux<Rep, std::giga>;

		template<class Rep>
		using basic_teralux = details::base_lux<Rep, std::tera>;

		template<class Rep>
		using basic_exalux = details::base_lux<Rep, std::exa>;

		template<class Rep>
		using basic_lux = details::base_lux<Rep, std::ratio<1>>;

		using attolux = basic_attolux<std::intmax_t>;
		using femtolux = basic_femtolux<std::intmax_t>;
		using picolux = basic_picolux<std::intmax_t>;
		using nanolux = basic_nanolux<std::intmax_t>;
		using microlux = basic_microlux<std::intmax_t>;
		using millilux = basic_millilux<std::intmax_t>;
		using centilux = basic_centilux<std::intmax_t>;
		using decilux = basic_decilux<std::intmax_t>;
		using decalux = basic_decalux<std::intmax_t>;
		using hectolux = basic_hectolux<std::intmax_t>;
		using kilolux = basic_kilolux<std::intmax_t>;
		using megalux = basic_megalux<std::intmax_t>;
		using gigalux = basic_gigalux<std::intmax_t>;
		using teralux = basic_teralux<std::intmax_t>;
		using exalux = basic_exalux<std::intmax_t>;
		using lux = basic_lux<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_lux<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("lx"))};
		};
	}
}
