module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.temperature;

import <string_view>;
import <concepts>;

import cmoon.math;
import cmoon.string;

import cmoon.measures;

namespace cmoon::measures
{
	export
	template<class Rep>
	struct temperature_values;
	
	export
	template<class Rep, class System, class Ratio = std::ratio<1>, dimension_type Dimension = 1>
	using temperature = basic_unit<Rep, Ratio, temperature_values<Rep>, System, Dimension>;

	template<class Rep, class System, class Ratio, dimension_type Dimension>
	std::true_type is_temperature_base_impl(const temperature<Rep, System, Ratio, Dimension>&);

	std::false_type is_temperature_base_impl(...);

	template<class U>
	constexpr auto is_based_in_temperature = decltype(is_temperature_base_impl(std::declval<U>()))::value;

	export
	template<class T>
	struct is_temperature : std::bool_constant<is_based_in_temperature<T>> {};

	export
	template<class T>
	constexpr bool is_temperature_v = is_temperature<T>::value;

	export
	template<class T>
	concept temperature_type = is_temperature_v<T>;

	export
	struct celsius_scale : public metric_system
	{
	};

	export
	struct fahrenheit_scale : public imperial_system
	{
	};

	export
	struct kelvin_scale : public metric_system
	{
	};

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_celsius = temperature<Rep, celsius_scale, std::ratio<1>, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_fahrenheit = temperature<Rep, fahrenheit_scale, std::ratio<1>, Dimension>;

	export
	template<class Rep, dimension_type Dimension = 1>
	using basic_kelvin = temperature<Rep, kelvin_scale, std::ratio<1>, Dimension>;

	export
	using celsius = basic_celsius<std::intmax_t>;
	
	export
	using fahrenheit = basic_fahrenheit<std::intmax_t>;
	
	export
	using kelvin = basic_kelvin<std::intmax_t>;

	export
	template<class Rep>
	struct temperature_values
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

		template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
		static constexpr ToBasicUnit system_cast(const temperature<Rep, fahrenheit_scale, Ratio, Dimension>& unit) noexcept
		{
			constexpr auto rational_5_9 = cmoon::basic_rational<typename ToBasicUnit::rep>{5, 9};
			if constexpr(std::same_as<ToSystem, celsius_scale>)
			{
				return ToBasicUnit{static_cast<typename ToBasicUnit::rep>((unit.count() - 32) * rational_5_9)};
			}
			else if constexpr (std::same_as<ToSystem, kelvin_scale>)
			{
				return ToBasicUnit{static_cast<typename ToBasicUnit::rep>((unit.count() + 459.67) * rational_5_9)};
			}
		}

		template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
		static constexpr ToBasicUnit system_cast(const temperature<Rep, kelvin_scale, Ratio, Dimension>& unit) noexcept
		{
			if constexpr (std::same_as<ToSystem, celsius_scale>)
			{
				return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() - 273.15)};
			}
			else if constexpr (std::same_as<ToSystem, fahrenheit_scale>)
			{
				constexpr auto rational_9_5 = cmoon::basic_rational<typename ToBasicUnit::rep>{9, 5};
				return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() * rational_9_5 - 459.67)};
			}
		}

		template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
		static constexpr ToBasicUnit system_cast(const temperature<Rep, celsius_scale, Ratio, Dimension>& unit) noexcept
		{
			if constexpr(std::same_as<ToSystem, fahrenheit_scale>)
			{
				constexpr auto rational_9_5 = cmoon::basic_rational<typename ToBasicUnit::rep>{9, 5};
				return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() * rational_9_5 + 32.0)};
			}
			else if constexpr (std::same_as<ToSystem, kelvin_scale>)
			{
				return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() + 273.15)};
			}
		}
	};

	export
	template<class Rep, dimension_type Dimension, class CharT>
	struct suffix<basic_fahrenheit<Rep, Dimension>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("F"))};
	};

	export
	template<class Rep, dimension_type Dimension, class CharT>
	struct suffix<basic_celsius<Rep, Dimension>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("C"))};
	};

	export
	template<class Rep, dimension_type Dimension, class CharT>
	struct suffix<basic_kelvin<Rep, Dimension>, CharT>
	{
		static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("K"))};
	};
}