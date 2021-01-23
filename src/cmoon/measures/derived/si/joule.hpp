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
			using base_joules = work<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;
		}

		template<class Rep>
		using basic_attojoules = details::base_joules<Rep, std::atto>;

		template<class Rep>
		using basic_femtojoules = details::base_joules<Rep, std::femto>;

		template<class Rep>
		using basic_picojoules = details::base_joules<Rep, std::pico>;

		template<class Rep>
		using basic_nanojoules = details::base_joules<Rep, std::nano>;

		template<class Rep>
		using basic_microjoules = details::base_joules<Rep, std::micro>;

		template<class Rep>
		using basic_millijoules = details::base_joules<Rep, std::milli>;

		template<class Rep>
		using basic_centijoules = details::base_joules<Rep, std::centi>;

		template<class Rep>
		using basic_decijoules = details::base_joules<Rep, std::deci>;

		template<class Rep>
		using basic_decajoules = details::base_joules<Rep, std::deca>;

		template<class Rep>
		using basic_hectojoules = details::base_joules<Rep, std::hecto>;

		template<class Rep>
		using basic_kilojoules = details::base_joules<Rep, std::kilo>;

		template<class Rep>
		using basic_megajoules = details::base_joules<Rep, std::mega>;

		template<class Rep>
		using basic_gigajoules = details::base_joules<Rep, std::giga>;

		template<class Rep>
		using basic_terajoules = details::base_joules<Rep, std::tera>;

		template<class Rep>
		using basic_exajoules = details::base_joules<Rep, std::exa>;

		template<class Rep>
		using basic_joules = details::base_joules<Rep, std::ratio<1>>;

		using attojoules = basic_attojoules<std::intmax_t>;
		using femtojoules = basic_femtojoules<std::intmax_t>;
		using picojoules = basic_picojoules<std::intmax_t>;
		using nanojoules = basic_nanojoules<std::intmax_t>;
		using microjoules = basic_microjoules<std::intmax_t>;
		using millijoules = basic_millijoules<std::intmax_t>;
		using centijoules = basic_centijoules<std::intmax_t>;
		using decijoules = basic_decijoules<std::intmax_t>;
		using decajoules = basic_decajoules<std::intmax_t>;
		using hectojoules = basic_hectojoules<std::intmax_t>;
		using kilojoules = basic_kilojoules<std::intmax_t>;
		using megajoules = basic_megajoules<std::intmax_t>;
		using gigajoules = basic_gigajoules<std::intmax_t>;
		using terajoules = basic_terajoules<std::intmax_t>;
		using exajoules = basic_exajoules<std::intmax_t>;
		using joules = basic_joules<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_joules<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("J"))};
		};
	}
}
