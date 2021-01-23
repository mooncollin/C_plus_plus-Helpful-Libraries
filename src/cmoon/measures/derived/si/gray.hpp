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
			using base_gray = absorbed_dose_radiation<Rep, Ratio, basic_meters<Rep>>;
		}

		template<class Rep>
		using basic_attogray = details::base_gray<Rep, std::atto>;

		template<class Rep>
		using basic_femtogray = details::base_gray<Rep, std::femto>;

		template<class Rep>
		using basic_picogray = details::base_gray<Rep, std::pico>;

		template<class Rep>
		using basic_nanogray = details::base_gray<Rep, std::nano>;

		template<class Rep>
		using basic_microgray = details::base_gray<Rep, std::micro>;

		template<class Rep>
		using basic_milligray = details::base_gray<Rep, std::milli>;

		template<class Rep>
		using basic_centigray = details::base_gray<Rep, std::centi>;

		template<class Rep>
		using basic_decigray = details::base_gray<Rep, std::deci>;

		template<class Rep>
		using basic_decagray = details::base_gray<Rep, std::deca>;

		template<class Rep>
		using basic_hectogray = details::base_gray<Rep, std::hecto>;

		template<class Rep>
		using basic_kilogray = details::base_gray<Rep, std::kilo>;

		template<class Rep>
		using basic_megagray = details::base_gray<Rep, std::mega>;

		template<class Rep>
		using basic_gigagray = details::base_gray<Rep, std::giga>;

		template<class Rep>
		using basic_teragray = details::base_gray<Rep, std::tera>;

		template<class Rep>
		using basic_exagray = details::base_gray<Rep, std::exa>;

		template<class Rep>
		using basic_gray = details::base_gray<Rep, std::ratio<1>>;

		using attogray = basic_attogray<std::intmax_t>;
		using femtogray = basic_femtogray<std::intmax_t>;
		using picogray = basic_picogray<std::intmax_t>;
		using nanogray = basic_nanogray<std::intmax_t>;
		using microgray = basic_microgray<std::intmax_t>;
		using milligray = basic_milligray<std::intmax_t>;
		using centigray = basic_centigray<std::intmax_t>;
		using decigray = basic_decigray<std::intmax_t>;
		using decagray = basic_decagray<std::intmax_t>;
		using hectogray = basic_hectogray<std::intmax_t>;
		using kilogray = basic_kilogray<std::intmax_t>;
		using megagray = basic_megagray<std::intmax_t>;
		using gigagray = basic_gigagray<std::intmax_t>;
		using teragray = basic_teragray<std::intmax_t>;
		using exagray = basic_exagray<std::intmax_t>;
		using gray = basic_gray<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_gray<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("Gy"))};
		};
	}
}
