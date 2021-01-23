#pragma once

#include <ratio>
#include <limits>
#include <type_traits>
#include <cstdint>
#include <concepts>

#include "cmoon/measures/measure.hpp"
#include "cmoon/math.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/string.hpp"

namespace cmoon
{
    namespace measures
    {
        template<class Rep>
        struct distance_values;

        template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
        using distance = basic_unit<Rep, Ratio, distance_values<Rep>, System, Dimension>;

        namespace details
        {
            template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
		    std::true_type is_distance_base_impl(const distance<Rep, System, Ratio, Dimension>&);
		    std::false_type is_distance_base_impl(...);

		    template<class U>
		    constexpr auto is_based_in_distance = decltype(is_distance_base_impl(std::declval<U>()))::value;
        }

        template<class T>
        struct is_distance : std::bool_constant<details::is_based_in_distance<T>> {};

        template<class T>
        constexpr bool is_distance_v = is_distance<T>::value;

        template<class T>
        concept distance_type = is_distance_v<T>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_attometers = distance<Rep, metric_system, std::atto, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_femtometers = distance<Rep, metric_system, std::femto, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_picometers = distance<Rep, metric_system, std::pico, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_nanometers = distance<Rep, metric_system, std::nano, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_micrometers = distance<Rep, metric_system, std::micro, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_millimeters = distance<Rep, metric_system, std::milli, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_centimeters = distance<Rep, metric_system, std::centi, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_decimeters = distance<Rep, metric_system, std::deci, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_meters = distance<Rep, metric_system, std::ratio<1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_decameters = distance<Rep, metric_system, std::deca, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_hectometers = distance<Rep, metric_system, std::hecto, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_kilometers = distance<Rep, metric_system, std::kilo, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_megameters = distance<Rep, metric_system, std::mega, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_gigameters = distance<Rep, metric_system, std::giga, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_terameters = distance<Rep, metric_system, std::tera, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_petameters = distance<Rep, metric_system, std::peta, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_exameters = distance<Rep, metric_system, std::exa, Dimension>;

        using attometers = basic_attometers<std::intmax_t>;
        using femtometers = basic_femtometers<std::intmax_t>;
        using picometers = basic_picometers<std::intmax_t>;
        using nanometers = basic_nanometers<std::intmax_t>;
        using micrometers = basic_micrometers<std::intmax_t>;
        using millimeters = basic_millimeters<std::intmax_t>;
        using centimeters = basic_centimeters<std::intmax_t>;
        using decimeters = basic_decimeters<std::intmax_t>;
        using meters = basic_meters<std::intmax_t>;
        using decameters = basic_decameters<std::intmax_t>;
        using hectometers = basic_hectometers<std::intmax_t>;
        using kilometers = basic_kilometers<std::intmax_t>;
        using megameters = basic_megameters<std::intmax_t>;
        using gigameters = basic_gigameters<std::intmax_t>;
        using terameters = basic_terameters<std::intmax_t>;
        using petameters = basic_petameters<std::intmax_t>;
        using exameters = basic_exameters<std::intmax_t>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_inches = distance<Rep, imperial_system, std::ratio<1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_feet = distance<Rep, imperial_system, std::ratio<12, 1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_yards = distance<Rep, imperial_system, std::ratio<36, 1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_miles = distance<Rep, imperial_system, std::ratio<63360, 1>, Dimension>;

        using inches = basic_inches<std::intmax_t>;
        using feet = basic_feet<std::intmax_t>;
        using yards = basic_yards<std::intmax_t>;
        using miles = basic_miles<std::intmax_t>;

        template<class Rep>
        struct distance_values
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

            template<distance_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
                requires (std::same_as<ToSystem, metric_system>)
            static constexpr ToBasicUnit system_cast(const distance<Rep, imperial_system, Ratio, Dimension>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto inch_conversion = unit_cast<basic_inches<common_type>>(unit);
                const auto nanometer_conversion = basic_nanometers<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(inch_conversion.count() * 25400000)};
                return ToBasicUnit{nanometer_conversion};
            }

            template<distance_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
                requires(std::same_as<ToSystem, imperial_system>)
            static constexpr ToBasicUnit system_cast(const distance<Rep, metric_system, Ratio, Dimension>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto nanometer_conversion = unit_cast<basic_nanometers<common_type>>(unit);
                const auto inch_conversion = basic_inches<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(nanometer_conversion.count() * 0.00000003937)};
                return ToBasicUnit{inch_conversion};
            }
        };

        template<class Rep, cmoon::ratio_type Ratio, dimension_type Dimension, class CharT>
        struct suffix<distance<Rep, metric_system, Ratio, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("m"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_inches<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("in"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_feet<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("ft"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_yards<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("yd"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_miles<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("mi"))};
        };
    }
}