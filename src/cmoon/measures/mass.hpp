#pragma once

#include <ratio>
#include <limits>
#include <type_traits>
#include <cstdint>
#include <concepts>

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/string.hpp"

namespace cmoon
{
    namespace measures
    {
        template<class Rep>
        struct mass_values;

        template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
        using mass = basic_unit<Rep, Ratio, mass_values<Rep>, System, Dimension>;

        namespace details
        {
            template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
            std::true_type is_mass_base_impl(const mass<Rep, System, Ratio, Dimension>&);
            std::false_type is_mass_base_impl(...);

            template<class U>
            constexpr auto is_based_in_mass = decltype(is_mass_base_impl(std::declval<U>()))::value;
        }

        template<class T>
        struct is_mass : std::bool_constant<details::is_based_in_mass<T>> {};

        template<class T>
        constexpr bool is_mass_v = is_mass<T>::value;

        template<class T>
        concept mass_type = is_mass_v<T>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_attograms = mass<Rep, metric_system, std::atto, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_femtograms = mass<Rep, metric_system, std::femto, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_picograms = mass<Rep, metric_system, std::pico, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_nanograms = mass<Rep, metric_system, std::nano, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_micrograms = mass<Rep, metric_system, std::micro, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_milligrams = mass<Rep, metric_system, std::milli, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_centigrams = mass<Rep, metric_system, std::centi, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_decigrams = mass<Rep, metric_system, std::deci, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_grams = mass<Rep, metric_system, std::ratio<1>, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_decagrams = mass<Rep, metric_system, std::deca, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_hectograms = mass<Rep, metric_system, std::hecto, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_kilograms = mass<Rep, metric_system, std::kilo, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_megagrams = mass<Rep, metric_system, std::mega, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_gigagrams = mass<Rep, metric_system, std::giga, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_teragrams = mass<Rep, metric_system, std::tera, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_petagrams = mass<Rep, metric_system, std::peta, Dimension>;
        
        template<class Rep, dimension_type Dimension = 1>
        using basic_exagrams = mass<Rep, metric_system, std::exa, Dimension>;
        
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

        template<class Rep, dimension_type Dimension = 1>
        using basic_grains = mass<Rep, imperial_system, std::ratio<1, 7000>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_drachm = mass<Rep, imperial_system, std::ratio<1, 256>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_ounces = mass<Rep, imperial_system, std::ratio<1, 16>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_pounds = mass<Rep, imperial_system, std::ratio<1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_stones = mass<Rep, imperial_system, std::ratio<14, 1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_quarters = mass<Rep, imperial_system, std::ratio<28, 1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_hundredweights = mass<Rep, imperial_system, std::ratio<112, 1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_short_tons = mass<Rep, imperial_system, std::ratio<2000, 1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_tons = mass<Rep, imperial_system, std::ratio<2240, 1>, Dimension>;

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

            template<mass_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
                requires(std::same_as<ToSystem, metric_system>)
            static constexpr ToBasicUnit system_cast(const mass<Rep, imperial_system, Ratio, Dimension>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto grains_conversion = unit_cast<basic_grains<common_type>>(unit);
                const auto nanograms_conversion = basic_nanograms<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(grains_conversion.count() * 64800000)};
                return ToBasicUnit{nanograms_conversion};
            }

            template<mass_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, cmoon::ratio_type Ratio, dimension_type Dimension>
                requires(std::same_as<ToSystem, imperial_system>)
            static constexpr ToBasicUnit system_cast(const mass<Rep, metric_system, Ratio, Dimension>& unit) noexcept
            {
                using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
                const auto nanograms_conversion = unit_cast<basic_nanograms<common_type>>(unit);
                const auto grains_conversion = basic_grains<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(nanograms_conversion.count() * 0.000000015432)};
                return ToBasicUnit{grains_conversion};
            }
        };

        template<class Rep, cmoon::ratio_type Ratio, dimension_type Dimension, class CharT>
        struct suffix<mass<Rep, metric_system, Ratio, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("g"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_grains<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("gr"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_drachm<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("dr"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_ounces<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("oz"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_pounds<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("lb"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_stones<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("st"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_quarters<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("qr"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_hundredweights<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("cwt"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_short_tons<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("t"))};
        };

        template<class Rep, dimension_type Dimension, class CharT>
        struct suffix<basic_tons<Rep, Dimension>, CharT>
        {
            static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("t"))};
        };
    }
}