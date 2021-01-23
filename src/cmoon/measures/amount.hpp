#pragma once

#include <type_traits>

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep>
		struct substance_amount_values;

		template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
		using substance_amount = basic_unit<Rep, Ratio, substance_amount_values<Rep>, System, Dimension>;

		template<class T>
		struct is_substance_amount : std::false_type {};

		template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
		struct is_substance_amount<substance_amount<Rep, System, Ratio, Dimension>> : std::true_type {};

		template<class T>
		constexpr bool is_substance_amount_v = is_substance_amount<T>::value;

		template<class T>
		concept substance_amount_type = is_substance_amount_v<T>;

		template<class T, dimension_type Dimension = 1>
		using basic_mole = substance_amount<T, metric_system, std::ratio<1>, Dimension>;

		using mole = basic_mole<std::intmax_t>;

		template<class Rep>
		struct substance_amount_values
		{
			static constexpr Rep zero() noexcept
			{
				return Rep(0);
			}

			static constexpr Rep min() noexcept
			{
				return std::numeric_limits<Rep>::lowest();
			}

			static constexpr Rep max() noexcept
			{
				return std::numeric_limits<Rep>::max();
			}
		};

		template<class Rep, dimension_type Dimension, class CharT>
		struct suffix<basic_mole<Rep, Dimension>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("mol"))};
		};
	}
}