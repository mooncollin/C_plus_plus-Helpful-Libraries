#pragma once

#include <string_view>
#include <concepts>

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/math.hpp"
#include "cmoon/string.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep>
		struct temperature_values;

		template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
		using temperature = basic_unit<Rep, Ratio, temperature_values<Rep>, System, Dimension>;

		namespace details
		{
			template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
			std::true_type is_temperature_base_impl(const temperature<Rep, System, Ratio, Dimension>&);
			std::false_type is_temperature_base_impl(...);

			template<class U>
			constexpr auto is_based_in_temperature = decltype(is_temperature_base_impl(std::declval<U>()))::value;
		}

		template<class T>
		struct is_temperature : std::bool_constant<details::is_based_in_temperature<T>> {};

		template<class T>
		constexpr bool is_temperature_v = is_temperature<T>::value;

		template<class T>
		concept temperature_type = is_temperature_v<T>;

		struct celsius_scale : public metric_system
		{
		};

		struct fahrenheit_scale : public imperial_system
		{
		};

		struct kelvin_scale : public metric_system
		{
		};

		template<class Rep, dimension_type Dimension = 1>
		using basic_celsius = temperature<Rep, celsius_scale, std::ratio<1>, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_fahrenheit = temperature<Rep, fahrenheit_scale, std::ratio<1>, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_kelvin = temperature<Rep, kelvin_scale, std::ratio<1>, Dimension>;

		using celsius = basic_celsius<std::intmax_t>;
		using fahrenheit = basic_fahrenheit<std::intmax_t>;
		using kelvin = basic_kelvin<std::intmax_t>;

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

			template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
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

			template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
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

			template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
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

		template<class Rep, dimension_type Dimension, class CharT>
		struct suffix<basic_fahrenheit<Rep, Dimension>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("F"))};
		};

		template<class Rep, dimension_type Dimension, class CharT>
		struct suffix<basic_celsius<Rep, Dimension>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("C"))};
		};

		template<class Rep, dimension_type Dimension, class CharT>
		struct suffix<basic_kelvin<Rep, Dimension>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("K"))};
		};
	}
}