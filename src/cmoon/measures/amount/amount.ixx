module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.amount;

import <type_traits>;
import <utility>;
import <cstdint>;

import cmoon.string;

import cmoon.measures;

namespace cmoon::measures
{
	export
	template<class Rep>
	struct substance_amount_values;

	export
	template<class Rep, class System, class Ratio = std::ratio<1>, dimension_type Dimension = 1>
	using substance_amount = basic_unit<Rep, Ratio, substance_amount_values<Rep>, System, Dimension>;

	template<class Rep, class System, class Ratio, std::intmax_t Dimension>
	std::true_type is_substance_amount_base_impl(const substance_amount<Rep, System, Ratio, Dimension>&);

	std::false_type is_substance_amount_base_impl(...);

	template<class U>
	constexpr auto is_based_in_substance_amount = decltype(is_substance_amount_base_impl(std::declval<U>()))::value;

	export
	template<class T>
	struct is_substance_amount : std::bool_constant<is_based_in_substance_amount<T>> {};

	export
	template<class T>
	constexpr bool is_substance_amount_v = is_substance_amount<T>::value;

	export
	template<class T>
	concept substance_amount_type = is_substance_amount_v<T>;

	export
	template<class T, dimension_type Dimension = 1>
	using basic_mole = substance_amount<T, metric_system, std::ratio<1>, Dimension>;

	export
	using mole = basic_mole<std::intmax_t>;

	export
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

	export
	template<class Rep, dimension_type Dimension, class CharT>
	struct suffix<basic_mole<Rep, Dimension>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("mol"))};
	};
}