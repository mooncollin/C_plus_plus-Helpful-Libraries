#pragma once

#include <ratio>
#include <limits>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <cstdint>
#include "measure.hpp"

namespace collin
{
    namespace measures
    {
        template<class Rep>
        struct mass_values;

        template<class Rep, class System, class Ratio = std::ratio<1>>
        using mass = basic_unit<Rep, Ratio, mass_values<Rep>, System>;

        template<class T>
        struct is_mass : std::false_type {};

        template<class Rep, class System, class Ratio>
        struct is_mass<mass<Rep, System, Ratio>> : std::true_type {};

        template<class T>
        constexpr bool is_mass_v = is_mass<T>::value;

        template<class Rep>
        using basic_attograms = mass<Rep, metric_system, std::atto>;
        
        template<class Rep>
        using basic_femtograms = mass<Rep, metric_system, std::femto>;
        
        template<class Rep>
        using basic_picograms = mass<Rep, metric_system, std::pico>;
        
        template<class Rep>
        using basic_nanograms = mass<Rep, metric_system, std::nano>;
        
        template<class Rep>
        using basic_micrograms = mass<Rep, metric_system, std::micro>;
        
        template<class Rep>
        using basic_milligrams = mass<Rep, metric_system, std::milli>;
        
        template<class Rep>
        using basic_centigrams = mass<Rep, metric_system, std::centi>;
        
        template<class Rep>
        using basic_decigrams = mass<Rep, metric_system, std::deci>;
        
        template<class Rep>
        using basic_grams = mass<Rep, metric_system>;
        
        template<class Rep>
        using basic_decagrams = mass<Rep, metric_system, std::deca>;
        
        template<class Rep>
        using basic_hectograms = mass<Rep, metric_system, std::hecto>;
        
        template<class Rep>
        using basic_kilograms = mass<Rep, metric_system, std::kilo>;
        
        template<class Rep>
        using basic_megagrams = mass<Rep, metric_system, std::mega>;
        
        template<class Rep>
        using basic_gigagrams = mass<Rep, metric_system, std::giga>;
        
        template<class Rep>
        using basic_teragrams = mass<Rep, metric_system, std::tera>;
        
        template<class Rep>
        using basic_petagrams = mass<Rep, metric_system, std::peta>;
        
        template<class Rep>
        using basic_exagrams = mass<Rep, metric_system, std::exa>;
        
        using attograms = basic_attograms<std::intmax_t>;
        using femtograms = basic_femtograms<std::intmax_t>;
        using picograms = basic_picograms<std::intmax_t>;
        using nanograms = basic_nanograms<std::intmax_t>;
        using micrograms = basic_micrograms<std::intmax_t>;
        using milligrams = basic_milligrams<std::intmax_t>;
        using centigrams = basic_centigrams<std::intmax_t>;
        using decigrams = basic_decigrams<std::intmax_t>;
        using grams = basic_grams<std::intmax_t>;
        using decagrams = basic_decagrams<std::intmax_t>;
        using hectograms = basic_hectograms<std::intmax_t>;
        using kilograms = basic_kilograms<std::intmax_t>;
        using megagrams = basic_megagrams<std::intmax_t>;
        using gigagrams = basic_gigagrams<std::intmax_t>;
        using teragrams = basic_teragrams<std::intmax_t>;
        using petagrams = basic_petagrams<std::intmax_t>;
        using exagrams = basic_exagrams<std::intmax_t>;

        template<class Rep>
        using basic_grains = mass<Rep, imperial_system, std::ratio<1, 7000>>;

        template<class Rep>
        using basic_drachm = mass<Rep, imperial_system, std::ratio<1, 256>>;

        template<class Rep>
        using basic_ounces = mass<Rep, imperial_system, std::ratio<1, 16>>;

        template<class Rep>
        using basic_pounds = mass<Rep, imperial_system>;

        template<class Rep>
        using basic_stones = mass<Rep, imperial_system, std::ratio<14, 1>>;

        template<class Rep>
        using basic_quarters = mass<Rep, imperial_system, std::ratio<28, 1>>;

        template<class Rep>
        using basic_hundredweights = mass<Rep, imperial_system, std::ratio<112, 1>>;

        template<class Rep>
        using basic_short_tons = mass<Rep, imperial_system, std::ratio<2000, 1>>;

        template<class Rep>
        using basic_tons = mass<Rep, imperial_system, std::ratio<2240, 1>>;

        using grains = basic_grains<std::intmax_t>;
        using drachm = basic_drachm<std::intmax_t>;
        using ounces = basic_ounces<std::intmax_t>;
        using pounds = basic_pounds<std::intmax_t>;
        using stones = basic_stones<std::intmax_t>;
        using quarters = basic_quarters<std::intmax_t>;
        using hundredweights = basic_hundredweights<std::intmax_t>;
        using short_tons = basic_short_tons<std::intmax_t>;
        using tons = basic_tons<std::intmax_t>;

        template<class Rep>
        struct mass_values
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

            template<class ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, typename = std::enable_if_t<
    /* requires */  is_mass_v<ToBasicUnit> && std::is_same_v<ToSystem, metric_system>
            >>
            static constexpr ToBasicUnit system_cast(const mass<Rep, imperial_system, Ratio>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto grains_conversion = unit_cast<basic_grains<common_type>>(unit);
                const auto nanograms_conversion = basic_nanograms<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(grains_conversion.count() * 64800000)};
                return ToBasicUnit{nanograms_conversion};
            }

            template<class ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, typename = std::enable_if_t<
    /* requires */  is_mass_v<ToBasicUnit> && std::is_same_v<ToSystem, imperial_system>
            >>
            static constexpr ToBasicUnit system_cast(const mass<Rep, metric_system, Ratio>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto nanograms_conversion = unit_cast<basic_nanograms<common_type>>(unit);
                const auto grains_conversion = basic_grains<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(nanograms_conversion.count() * 0.000000015432)};
                return ToBasicUnit{grains_conversion};
            }
        };

        template<class Rep>
        struct metric_system::suffix<basic_attograms<Rep>>
        {
            constexpr static std::string_view value {"ag"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_femtograms<Rep>>
        {
            constexpr static std::string_view value {"fg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_picograms<Rep>>
        {
            constexpr static std::string_view value {"pg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_nanograms<Rep>>
        {
            constexpr static std::string_view value {"ng"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_micrograms<Rep>>
        {
            constexpr static std::string_view value {"ug"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_milligrams<Rep>>
        {
            constexpr static std::string_view value {"mg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_centigrams<Rep>>
        {
            constexpr static std::string_view value {"cg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_decigrams<Rep>>
        {
            constexpr static std::string_view value {"dg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_grams<Rep>>
        {
            constexpr static std::string_view value {"g"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_decagrams<Rep>>
        {
            constexpr static std::string_view value {"dag"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_hectograms<Rep>>
        {
            constexpr static std::string_view value {"hg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_kilograms<Rep>>
        {
            constexpr static std::string_view value {"kg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_megagrams<Rep>>
        {
            constexpr static std::string_view value {"Mg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_gigagrams<Rep>>
        {
            constexpr static std::string_view value {"Gg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_teragrams<Rep>>
        {
            constexpr static std::string_view value {"Tg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_petagrams<Rep>>
        {
            constexpr static std::string_view value {"Pg"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_exagrams<Rep>>
        {
            constexpr static std::string_view value {"Eg"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_grains<Rep>>
        {
            constexpr static std::string_view value {"gr"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_drachm<Rep>>
        {
            constexpr static std::string_view value {"dr"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_ounces<Rep>>
        {
            constexpr static std::string_view value {"oz"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_pounds<Rep>>
        {
            constexpr static std::string_view value {"lb"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_stones<Rep>>
        {
            constexpr static std::string_view value {"st"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_quarters<Rep>>
        {
            constexpr static std::string_view value {"qr"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_hundredweights<Rep>>
        {
            constexpr static std::string_view value {"cwt"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_short_tons<Rep>>
        {
            constexpr static std::string_view value {"t"};
        };

        template<class Rep>
        struct imperial_system::suffix<basic_tons<Rep>>
        {
            constexpr static std::string_view value {"t"};
        };
    }
}