#pragma once

#include <string_view>

#include "measure.hpp"
#include "collin/math.hpp"

namespace collin
{
	namespace measures
	{
		template<class Rep>
		struct temperature_values;

		template<class Rep, class System, class Ratio = std::ratio<1>>
		using temperature = basic_unit<Rep, Ratio, temperature_values<Rep>, System>;

		template<class T>
		struct is_temperature : std::false_type {};

		template<class Rep, class System, class Ratio>
		struct is_temperature<temperature<Rep, System, Ratio>> : std::true_type {};

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

		template<class Rep>
		using basic_celsius = temperature<Rep, celsius_scale>;

		template<class Rep>
		using basic_fahrenheit = temperature<Rep, fahrenheit_scale>;

		template<class Rep>
		using basic_kelvin = temperature<Rep, kelvin_scale>;

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

			template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, ratio_type Ratio>
			static constexpr ToBasicUnit system_cast(const temperature<Rep, fahrenheit_scale, Ratio>& unit) noexcept
			{
				constexpr auto rational_5_9 = collin::math::basic_rational<typename ToBasicUnit::rep>{5, 9};
				if constexpr(collin::concepts::same<ToSystem, celsius_scale>)
				{
					return ToBasicUnit{static_cast<typename ToBasicUnit::rep>((unit.count() - 32) * rational_5_9)};
				}
				else if constexpr (collin::concepts::same<ToSystem, kelvin_scale>)
				{
					return ToBasicUnit{static_cast<typename ToBasicUnit::rep>((unit.count() + 459.67) * rational_5_9)};
				}
			}

			template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, ratio_type Ratio>
			static constexpr ToBasicUnit system_cast(const temperature<Rep, kelvin_scale, Ratio>& unit) noexcept
			{
				if constexpr (collin::concepts::same<ToSystem, celsius_scale>)
				{
					return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() - 273.15)};
				}
				else if constexpr (collin::concepts::same<ToSystem, fahrenheit_scale>)
				{
					constexpr auto rational_9_5 = collin::math::basic_rational<typename ToBasicUnit::rep>{9, 5};
					return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() * rational_9_5 - 459.67)};
				}
			}

			template<temperature_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, ratio_type Ratio>
			static constexpr ToBasicUnit system_cast(const temperature<Rep, celsius_scale, Ratio>& unit) noexcept
			{
				if constexpr(collin::concepts::same<ToSystem, fahrenheit_scale>)
				{
					constexpr auto rational_9_5 = collin::math::basic_rational<typename ToBasicUnit::rep>{9, 5};
					return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() * rational_9_5 + 32.0)};
				}
				else if constexpr (collin::concepts::same<ToSystem, kelvin_scale>)
				{
					return ToBasicUnit{static_cast<typename ToBasicUnit::rep>(unit.count() + 273.15)};
				}
			}
		};

		template<class Rep>
		struct fahrenheit_scale::suffix<basic_fahrenheit<Rep>>
		{
			constexpr static std::string_view value {"F"};
		};

		template<class Rep>
		struct celsius_scale::suffix<basic_celsius<Rep>>
		{
			constexpr static std::string_view value {"C"};
		};

		template<class Rep>
		struct kelvin_scale::suffix<basic_kelvin<Rep>>
		{
			constexpr static std::string_view value {"K"};
		};
	}
}