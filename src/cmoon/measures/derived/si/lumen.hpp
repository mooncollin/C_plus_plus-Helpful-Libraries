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
			struct base_lumens : public luminous_intensity<Rep, metric_system, Ratio, 1>
			{
				using luminous_intensity<Rep, metric_system, Ratio, 1>::luminous_intensity;
			};
		}

		template<class Rep>
		using basic_attolumens = details::base_lumens<Rep, std::atto>;

		template<class Rep>
		using basic_femtolumens = details::base_lumens<Rep, std::femto>;

		template<class Rep>
		using basic_picolumens = details::base_lumens<Rep, std::pico>;

		template<class Rep>
		using basic_nanolumens = details::base_lumens<Rep, std::nano>;

		template<class Rep>
		using basic_microlumens = details::base_lumens<Rep, std::micro>;

		template<class Rep>
		using basic_millilumens = details::base_lumens<Rep, std::milli>;

		template<class Rep>
		using basic_centilumens = details::base_lumens<Rep, std::centi>;

		template<class Rep>
		using basic_decilumens = details::base_lumens<Rep, std::deci>;

		template<class Rep>
		using basic_decalumens = details::base_lumens<Rep, std::deca>;

		template<class Rep>
		using basic_hectolumens = details::base_lumens<Rep, std::hecto>;

		template<class Rep>
		using basic_kilolumens = details::base_lumens<Rep, std::kilo>;

		template<class Rep>
		using basic_megalumens = details::base_lumens<Rep, std::mega>;

		template<class Rep>
		using basic_gigalumens = details::base_lumens<Rep, std::giga>;

		template<class Rep>
		using basic_teralumens = details::base_lumens<Rep, std::tera>;

		template<class Rep>
		using basic_exalumens = details::base_lumens<Rep, std::exa>;

		template<class Rep>
		using basic_lumens = details::base_lumens<Rep, std::ratio<1>>;

		using attolumens = basic_attolumens<std::intmax_t>;
		using femtolumens = basic_femtolumens<std::intmax_t>;
		using picolumens = basic_picolumens<std::intmax_t>;
		using nanolumens = basic_nanolumens<std::intmax_t>;
		using microlumens = basic_microlumens<std::intmax_t>;
		using millilumens = basic_millilumens<std::intmax_t>;
		using centilumens = basic_centilumens<std::intmax_t>;
		using decilumens = basic_decilumens<std::intmax_t>;
		using decalumens = basic_decalumens<std::intmax_t>;
		using hectolumens = basic_hectolumens<std::intmax_t>;
		using kilolumens = basic_kilolumens<std::intmax_t>;
		using megalumens = basic_megalumens<std::intmax_t>;
		using gigalumens = basic_gigalumens<std::intmax_t>;
		using teralumens = basic_teralumens<std::intmax_t>;
		using exalumens = basic_exalumens<std::intmax_t>;
		using lumens = basic_lumens<std::intmax_t>;

		template<class Rep, cmoon::ratio_type Ratio, class CharT>
		struct suffix<details::base_lumens<Rep, Ratio>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("lm"))};
		};
	}
}
