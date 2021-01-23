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
			struct base_hertz : public frequency<Rep, Ratio>
			{
				using frequency<Rep, Ratio>::frequency;
			};
		}

		template<class Rep>
		using basic_attohertz = details::base_hertz<Rep, std::atto>;

		template<class Rep>
		using basic_femtohertz = details::base_hertz<Rep, std::femto>;

		template<class Rep>
		using basic_picohertz = details::base_hertz<Rep, std::pico>;

		template<class Rep>
		using basic_nanohertz = details::base_hertz<Rep, std::nano>;

		template<class Rep>
		using basic_microhertz = details::base_hertz<Rep, std::micro>;

		template<class Rep>
		using basic_millihertz = details::base_hertz<Rep, std::milli>;

		template<class Rep>
		using basic_centihertz = details::base_hertz<Rep, std::centi>;

		template<class Rep>
		using basic_decihertz = details::base_hertz<Rep, std::deci>;

		template<class Rep>
		using basic_decahertz = details::base_hertz<Rep, std::deca>;

		template<class Rep>
		using basic_hectohertz = details::base_hertz<Rep, std::hecto>;

		template<class Rep>
		using basic_kilohertz = details::base_hertz<Rep, std::kilo>;

		template<class Rep>
		using basic_megahertz = details::base_hertz<Rep, std::mega>;

		template<class Rep>
		using basic_gigahertz = details::base_hertz<Rep, std::giga>;

		template<class Rep>
		using basic_terahertz = details::base_hertz<Rep, std::tera>;

		template<class Rep>
		using basic_exahertz = details::base_hertz<Rep, std::exa>;

		template<class Rep>
		using basic_hertz = details::base_hertz<Rep, std::ratio<1>>;

		using attohertz = basic_attohertz<std::intmax_t>;
		using femtohertz = basic_femtohertz<std::intmax_t>;
		using picohertz = basic_picohertz<std::intmax_t>;
		using nanohertz = basic_nanohertz<std::intmax_t>;
		using microhertz = basic_microhertz<std::intmax_t>;
		using millihertz = basic_millihertz<std::intmax_t>;
		using centihertz = basic_centihertz<std::intmax_t>;
		using decihertz = basic_decihertz<std::intmax_t>;
		using decahertz = basic_decahertz<std::intmax_t>;
		using hectohertz = basic_hectohertz<std::intmax_t>;
		using kilohertz = basic_kilohertz<std::intmax_t>;
		using megahertz = basic_megahertz<std::intmax_t>;
		using gigahertz = basic_gigahertz<std::intmax_t>;
		using terahertz = basic_terahertz<std::intmax_t>;
		using exahertz = basic_exahertz<std::intmax_t>;
		using hertz = basic_hertz<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_hertz<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Hz"))};
		};
	}
}
