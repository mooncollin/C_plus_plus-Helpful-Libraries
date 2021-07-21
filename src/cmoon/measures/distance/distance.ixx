module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.distance;

import <ratio>;
import <limits>;
import <type_traits>;
import <cstdint>;
import <concepts>;

import cmoon.math;
import cmoon.string;

import cmoon.measures;

namespace cmoon::measures
{
    export
    template<class Rep>
    struct distance_values;

    export
    template<class Rep, class System, class Ratio = std::ratio<1>, dimension_type Dimension = 1>
    using distance = basic_unit<Rep, Ratio, distance_values<Rep>, System, Dimension>;

    template<class Rep, class System, class Ratio, dimension_type Dimension>
	std::true_type is_distance_base_impl(const distance<Rep, System, Ratio, Dimension>&);

	std::false_type is_distance_base_impl(...);

	template<class U>
	constexpr auto is_based_in_distance = decltype(is_distance_base_impl(std::declval<U>()))::value;

    export
    template<class T>
    struct is_distance : std::bool_constant<is_based_in_distance<T>> {};

    export
    template<class T>
    constexpr bool is_distance_v = is_distance<T>::value;

    export
    template<class T>
    concept distance_type = is_distance_v<T>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_attometers = distance<Rep, metric_system, std::atto, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_femtometers = distance<Rep, metric_system, std::femto, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_picometers = distance<Rep, metric_system, std::pico, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_nanometers = distance<Rep, metric_system, std::nano, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_micrometers = distance<Rep, metric_system, std::micro, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_millimeters = distance<Rep, metric_system, std::milli, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_centimeters = distance<Rep, metric_system, std::centi, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_decimeters = distance<Rep, metric_system, std::deci, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_meters = distance<Rep, metric_system, std::ratio<1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_decameters = distance<Rep, metric_system, std::deca, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_hectometers = distance<Rep, metric_system, std::hecto, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_kilometers = distance<Rep, metric_system, std::kilo, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_megameters = distance<Rep, metric_system, std::mega, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_gigameters = distance<Rep, metric_system, std::giga, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_terameters = distance<Rep, metric_system, std::tera, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_petameters = distance<Rep, metric_system, std::peta, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_exameters = distance<Rep, metric_system, std::exa, Dimension>;

    export
    using attometers = basic_attometers<std::intmax_t>;
    
    export
    using femtometers = basic_femtometers<std::intmax_t>;
    
    export
    using picometers = basic_picometers<std::intmax_t>;
    
    export
    using nanometers = basic_nanometers<std::intmax_t>;
    
    export
    using micrometers = basic_micrometers<std::intmax_t>;
    
    export
    using millimeters = basic_millimeters<std::intmax_t>;
    
    export
    using centimeters = basic_centimeters<std::intmax_t>;
    
    export
    using decimeters = basic_decimeters<std::intmax_t>;
    
    export
    using meters = basic_meters<std::intmax_t>;
    
    export
    using decameters = basic_decameters<std::intmax_t>;
    
    export
    using hectometers = basic_hectometers<std::intmax_t>;
    
    export
    using kilometers = basic_kilometers<std::intmax_t>;
    
    export
    using megameters = basic_megameters<std::intmax_t>;
    
    export
    using gigameters = basic_gigameters<std::intmax_t>;
    
    export
    using terameters = basic_terameters<std::intmax_t>;
    
    export
    using petameters = basic_petameters<std::intmax_t>;
    
    export
    using exameters = basic_exameters<std::intmax_t>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_inches = distance<Rep, imperial_system, std::ratio<1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_feet = distance<Rep, imperial_system, std::ratio<12, 1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_yards = distance<Rep, imperial_system, std::ratio<36, 1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_miles = distance<Rep, imperial_system, std::ratio<63360, 1>, Dimension>;

    export
    using inches = basic_inches<std::intmax_t>;
    
    export
    using feet = basic_feet<std::intmax_t>;
    
    export
    using yards = basic_yards<std::intmax_t>;
    
    export
    using miles = basic_miles<std::intmax_t>;

    export
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

        template<distance_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
            requires (std::same_as<ToSystem, metric_system>)
        static constexpr ToBasicUnit system_cast(const distance<Rep, imperial_system, Ratio, Dimension>& unit) noexcept
        {
            using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
            const auto inch_conversion = unit_cast<basic_inches<common_type>>(unit);
            const auto nanometer_conversion = basic_nanometers<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(inch_conversion.count() * 25400000)};
            return ToBasicUnit{nanometer_conversion};
        }

        template<distance_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
            requires(std::same_as<ToSystem, imperial_system>)
        static constexpr ToBasicUnit system_cast(const distance<Rep, metric_system, Ratio, Dimension>& unit) noexcept
        {
            using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
            const auto nanometer_conversion = unit_cast<basic_nanometers<common_type>>(unit);
            const auto inch_conversion = basic_inches<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(nanometer_conversion.count() * 0.00000003937)};
            return ToBasicUnit{inch_conversion};
        }
    };

    export
    template<class Rep, class Ratio, dimension_type Dimension, class CharT>
    struct suffix<distance<Rep, metric_system, Ratio, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("m"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_inches<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("in"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_feet<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("ft"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_yards<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("yd"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_miles<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("mi"))};
    };
}