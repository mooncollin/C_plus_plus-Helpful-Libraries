#pragma once

#include <ratio>
#include <limits>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <cstdint>

#include "collin/measures/measure.hpp"
#include "collin/math.hpp"
#include "collin/ratio.hpp"

namespace collin
{
    namespace measures
    {
        template<class Rep>
        struct distance_values;

        template<class Rep, class System, collin::ratio::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
        using distance = basic_unit<Rep, Ratio, distance_values<Rep>, System, Dimension>;

        template<class T>
        struct is_distance : std::false_type {};

        template<class Rep, class System, collin::ratio::ratio_type Ratio, dimension_type Dimension>
        struct is_distance<distance<Rep, System, Ratio, Dimension>> : std::true_type {};

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

            template<distance_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, collin::ratio::ratio_type Ratio, dimension_type Dimension>
                requires (collin::concepts::same<ToSystem, metric_system>)
            static constexpr ToBasicUnit system_cast(const distance<Rep, imperial_system, Ratio, Dimension>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto inch_conversion = unit_cast<basic_inches<common_type>>(unit);
                const auto nanometer_conversion = basic_nanometers<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(inch_conversion.count() * 25400000)};
                return ToBasicUnit{nanometer_conversion};
            }

            template<distance_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, collin::ratio::ratio_type Ratio, dimension_type Dimension>
                requires(collin::concepts::same<ToSystem, imperial_system>)
            static constexpr ToBasicUnit system_cast(const distance<Rep, metric_system, Ratio, Dimension>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto nanometer_conversion = unit_cast<basic_nanometers<common_type>>(unit);
                const auto inch_conversion = basic_inches<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(nanometer_conversion.count() * 0.00000003937)};
                return ToBasicUnit{inch_conversion};
            }
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_attometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"am"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_femtometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"fm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_picometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"pm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_nanometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"nm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_micrometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"um"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_millimeters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"mm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_centimeters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"cm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_decimeters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"dm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_meters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"m"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_decameters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"dam"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_hectometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"hm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_kilometers<Rep, Dimension>>
        {
            constexpr static std::string_view value {"km"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_megameters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Mm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_gigameters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Gm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_terameters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Tm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_petameters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Pm"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_exameters<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Em"};
        };

        template<class Rep, dimension_type Dimension>
        struct imperial_system::suffix<basic_inches<Rep, Dimension>>
        {
            constexpr static std::string_view value {"in"};
        };

        template<class Rep, dimension_type Dimension>
        struct imperial_system::suffix<basic_feet<Rep, Dimension>>
        {
            constexpr static std::string_view value {"ft"};
        };

        template<class Rep, dimension_type Dimension>
        struct imperial_system::suffix<basic_yards<Rep, Dimension>>
        {
            constexpr static std::string_view value {"yd"};
        };

        template<class Rep, dimension_type Dimension>
        struct imperial_system::suffix<basic_miles<Rep, Dimension>>
        {
            constexpr static std::string_view value {"mi"};
        };
    }
}