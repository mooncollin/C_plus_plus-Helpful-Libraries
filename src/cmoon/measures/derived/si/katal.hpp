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
			using base_katals = catalytic_activity<Rep, Ratio, basic_mole<Rep>>;
		}

		template<class Rep>
		using basic_attokatals = details::base_katals<Rep, std::atto>;

		template<class Rep>
		using basic_femtokatals = details::base_katals<Rep, std::femto>;

		template<class Rep>
		using basic_picokatals = details::base_katals<Rep, std::pico>;

		template<class Rep>
		using basic_nanokatals = details::base_katals<Rep, std::nano>;

		template<class Rep>
		using basic_microkatals = details::base_katals<Rep, std::micro>;

		template<class Rep>
		using basic_millikatals = details::base_katals<Rep, std::milli>;

		template<class Rep>
		using basic_centikatals = details::base_katals<Rep, std::centi>;

		template<class Rep>
		using basic_decikatals = details::base_katals<Rep, std::deci>;

		template<class Rep>
		using basic_decakatals = details::base_katals<Rep, std::deca>;

		template<class Rep>
		using basic_hectokatals = details::base_katals<Rep, std::hecto>;

		template<class Rep>
		using basic_kilokatals = details::base_katals<Rep, std::kilo>;

		template<class Rep>
		using basic_megakatals = details::base_katals<Rep, std::mega>;

		template<class Rep>
		using basic_gigakatals = details::base_katals<Rep, std::giga>;

		template<class Rep>
		using basic_terakatals = details::base_katals<Rep, std::tera>;

		template<class Rep>
		using basic_exakatals = details::base_katals<Rep, std::exa>;

		template<class Rep>
		using basic_katals = details::base_katals<Rep, std::ratio<1>>;

		using attokatals = basic_attokatals<std::intmax_t>;
		using femtokatals = basic_femtokatals<std::intmax_t>;
		using picokatals = basic_picokatals<std::intmax_t>;
		using nanokatals = basic_nanokatals<std::intmax_t>;
		using microkatals = basic_microkatals<std::intmax_t>;
		using millikatals = basic_millikatals<std::intmax_t>;
		using centikatals = basic_centikatals<std::intmax_t>;
		using decikatals = basic_decikatals<std::intmax_t>;
		using decakatals = basic_decakatals<std::intmax_t>;
		using hectokatals = basic_hectokatals<std::intmax_t>;
		using kilokatals = basic_kilokatals<std::intmax_t>;
		using megakatals = basic_megakatals<std::intmax_t>;
		using gigakatals = basic_gigakatals<std::intmax_t>;
		using terakatals = basic_terakatals<std::intmax_t>;
		using exakatals = basic_exakatals<std::intmax_t>;
		using katals = basic_katals<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_katals<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("kat"))};
		};
	}
}
