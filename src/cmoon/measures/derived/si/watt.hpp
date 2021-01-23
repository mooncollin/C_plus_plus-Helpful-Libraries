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
			using base_watts = power<Rep, Ratio, basic_kilograms<Rep>, basic_meters<Rep>>;
		}

		template<class Rep>
		using basic_attowatts = details::base_watts<Rep, std::atto>;

		template<class Rep>
		using basic_femtowatts = details::base_watts<Rep, std::femto>;

		template<class Rep>
		using basic_picowatts = details::base_watts<Rep, std::pico>;

		template<class Rep>
		using basic_nanowatts = details::base_watts<Rep, std::nano>;

		template<class Rep>
		using basic_microwatts = details::base_watts<Rep, std::micro>;

		template<class Rep>
		using basic_milliwatts = details::base_watts<Rep, std::milli>;

		template<class Rep>
		using basic_centiwatts = details::base_watts<Rep, std::centi>;

		template<class Rep>
		using basic_deciwatts = details::base_watts<Rep, std::deci>;

		template<class Rep>
		using basic_decawatts = details::base_watts<Rep, std::deca>;

		template<class Rep>
		using basic_hectowatts = details::base_watts<Rep, std::hecto>;

		template<class Rep>
		using basic_kilowatts = details::base_watts<Rep, std::kilo>;

		template<class Rep>
		using basic_megawatts = details::base_watts<Rep, std::mega>;

		template<class Rep>
		using basic_gigawatts = details::base_watts<Rep, std::giga>;

		template<class Rep>
		using basic_terawatts = details::base_watts<Rep, std::tera>;

		template<class Rep>
		using basic_exawatts = details::base_watts<Rep, std::exa>;

		template<class Rep>
		using basic_watts = details::base_watts<Rep, std::ratio<1>>;

		using attowatts = basic_attowatts<std::intmax_t>;
		using femtowatts = basic_femtowatts<std::intmax_t>;
		using picowatts = basic_picowatts<std::intmax_t>;
		using nanowatts = basic_nanowatts<std::intmax_t>;
		using microwatts = basic_microwatts<std::intmax_t>;
		using milliwatts = basic_milliwatts<std::intmax_t>;
		using centiwatts = basic_centiwatts<std::intmax_t>;
		using deciwatts = basic_deciwatts<std::intmax_t>;
		using decawatts = basic_decawatts<std::intmax_t>;
		using hectowatts = basic_hectowatts<std::intmax_t>;
		using kilowatts = basic_kilowatts<std::intmax_t>;
		using megawatts = basic_megawatts<std::intmax_t>;
		using gigawatts = basic_gigawatts<std::intmax_t>;
		using terawatts = basic_terawatts<std::intmax_t>;
		using exawatts = basic_exawatts<std::intmax_t>;
		using watts = basic_watts<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_watts<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("W"))};
		};
	}
}
