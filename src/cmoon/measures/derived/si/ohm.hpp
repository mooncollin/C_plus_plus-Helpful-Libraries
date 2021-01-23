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
			using base_ohms = electrical_resistance<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>, basic_ampere<Rep>>;
		}

		template<class Rep>
		using basic_attoohms = details::base_ohms<Rep, std::atto>;

		template<class Rep>
		using basic_femtoohms = details::base_ohms<Rep, std::femto>;

		template<class Rep>
		using basic_picoohms = details::base_ohms<Rep, std::pico>;

		template<class Rep>
		using basic_nanoohms = details::base_ohms<Rep, std::nano>;

		template<class Rep>
		using basic_microohms = details::base_ohms<Rep, std::micro>;

		template<class Rep>
		using basic_milliohms = details::base_ohms<Rep, std::milli>;

		template<class Rep>
		using basic_centiohms = details::base_ohms<Rep, std::centi>;

		template<class Rep>
		using basic_deciohms = details::base_ohms<Rep, std::deci>;

		template<class Rep>
		using basic_decaohms = details::base_ohms<Rep, std::deca>;

		template<class Rep>
		using basic_hectoohms = details::base_ohms<Rep, std::hecto>;

		template<class Rep>
		using basic_kiloohms = details::base_ohms<Rep, std::kilo>;

		template<class Rep>
		using basic_megaohms = details::base_ohms<Rep, std::mega>;

		template<class Rep>
		using basic_gigaohms = details::base_ohms<Rep, std::giga>;

		template<class Rep>
		using basic_teraohms = details::base_ohms<Rep, std::tera>;

		template<class Rep>
		using basic_exaohms = details::base_ohms<Rep, std::exa>;

		template<class Rep>
		using basic_ohms = details::base_ohms<Rep, std::ratio<1>>;

		using attoohms = basic_attoohms<std::intmax_t>;
		using femtoohms = basic_femtoohms<std::intmax_t>;
		using picoohms = basic_picoohms<std::intmax_t>;
		using nanoohms = basic_nanoohms<std::intmax_t>;
		using microohms = basic_microohms<std::intmax_t>;
		using milliohms = basic_milliohms<std::intmax_t>;
		using centiohms = basic_centiohms<std::intmax_t>;
		using deciohms = basic_deciohms<std::intmax_t>;
		using decaohms = basic_decaohms<std::intmax_t>;
		using hectoohms = basic_hectoohms<std::intmax_t>;
		using kiloohms = basic_kiloohms<std::intmax_t>;
		using megaohms = basic_megaohms<std::intmax_t>;
		using gigaohms = basic_gigaohms<std::intmax_t>;
		using teraohms = basic_teraohms<std::intmax_t>;
		using exaohms = basic_exaohms<std::intmax_t>;
		using ohms = basic_ohms<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_ohms<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("O"))};
		};
	}
}
