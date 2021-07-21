module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.mass;

import <ratio>;
import <limits>;
import <type_traits>;
import <cstdint>;
import <concepts>;

import cmoon.string;

import cmoon.measures;

namespace cmoon::measures
{
    export
    template<class Rep>
    struct mass_values;

    export
    template<class Rep, class System, class Ratio = std::ratio<1>, dimension_type Dimension = 1>
    using mass = basic_unit<Rep, Ratio, mass_values<Rep>, System, Dimension>;

    template<class Rep, class System, class Ratio, dimension_type Dimension>
    std::true_type is_mass_base_impl(const mass<Rep, System, Ratio, Dimension>&);

    std::false_type is_mass_base_impl(...);

    template<class U>
    constexpr auto is_based_in_mass = decltype(is_mass_base_impl(std::declval<U>()))::value;

    export
    template<class T>
    struct is_mass : std::bool_constant<is_based_in_mass<T>> {};

    export
    template<class T>
    constexpr bool is_mass_v = is_mass<T>::value;

    export
    template<class T>
    concept mass_type = is_mass_v<T>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_attograms = mass<Rep, metric_system, std::atto, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_femtograms = mass<Rep, metric_system, std::femto, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_picograms = mass<Rep, metric_system, std::pico, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_nanograms = mass<Rep, metric_system, std::nano, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_micrograms = mass<Rep, metric_system, std::micro, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_milligrams = mass<Rep, metric_system, std::milli, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_centigrams = mass<Rep, metric_system, std::centi, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_decigrams = mass<Rep, metric_system, std::deci, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_grams = mass<Rep, metric_system, std::ratio<1>, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_decagrams = mass<Rep, metric_system, std::deca, Dimension>;
        
    template<class Rep, dimension_type Dimension = 1>
    using basic_hectograms = mass<Rep, metric_system, std::hecto, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_kilograms = mass<Rep, metric_system, std::kilo, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_megagrams = mass<Rep, metric_system, std::mega, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_gigagrams = mass<Rep, metric_system, std::giga, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_teragrams = mass<Rep, metric_system, std::tera, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_petagrams = mass<Rep, metric_system, std::peta, Dimension>;
    
    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_exagrams = mass<Rep, metric_system, std::exa, Dimension>;
    
    export
    using attograms = basic_attograms<std::intmax_t>;
    
    export
    using femtograms = basic_femtograms<std::intmax_t>;
    
    export
    using picograms = basic_picograms<std::intmax_t>;
    
    export
    using nanograms = basic_nanograms<std::intmax_t>;
    
    export
    using micrograms = basic_micrograms<std::intmax_t>;
    
    export
    using milligrams = basic_milligrams<std::intmax_t>;
    
    export
    using centigrams = basic_centigrams<std::intmax_t>;
    
    export
    using decigrams = basic_decigrams<std::intmax_t>;
    
    export
    using grams = basic_grams<std::intmax_t>;
    
    export
    using decagrams = basic_decagrams<std::intmax_t>;
    
    export
    using hectograms = basic_hectograms<std::intmax_t>;
    
    export
    using kilograms = basic_kilograms<std::intmax_t>;
    
    export
    using megagrams = basic_megagrams<std::intmax_t>;
    
    export
    using gigagrams = basic_gigagrams<std::intmax_t>;
    
    export
    using teragrams = basic_teragrams<std::intmax_t>;
    
    export
    using petagrams = basic_petagrams<std::intmax_t>;
    
    export
    using exagrams = basic_exagrams<std::intmax_t>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_grains = mass<Rep, imperial_system, std::ratio<1, 7000>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_drachm = mass<Rep, imperial_system, std::ratio<1, 256>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_ounces = mass<Rep, imperial_system, std::ratio<1, 16>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_pounds = mass<Rep, imperial_system, std::ratio<1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_stones = mass<Rep, imperial_system, std::ratio<14, 1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_quarters = mass<Rep, imperial_system, std::ratio<28, 1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_hundredweights = mass<Rep, imperial_system, std::ratio<112, 1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_short_tons = mass<Rep, imperial_system, std::ratio<2000, 1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_tons = mass<Rep, imperial_system, std::ratio<2240, 1>, Dimension>;

    export
    using grains = basic_grains<std::intmax_t>;
    
    export
    using drachm = basic_drachm<std::intmax_t>;
    
    export
    using ounces = basic_ounces<std::intmax_t>;
    
    export
    using pounds = basic_pounds<std::intmax_t>;
    
    export
    using stones = basic_stones<std::intmax_t>;
    
    export
    using quarters = basic_quarters<std::intmax_t>;
    
    export
    using hundredweights = basic_hundredweights<std::intmax_t>;
    
    export
    using short_tons = basic_short_tons<std::intmax_t>;
    
    export
    using tons = basic_tons<std::intmax_t>;

    export
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

        template<mass_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
            requires(std::same_as<ToSystem, metric_system>)
        static constexpr ToBasicUnit system_cast(const mass<Rep, imperial_system, Ratio, Dimension>& unit) noexcept
        {
            using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
            const auto grains_conversion = unit_cast<basic_grains<common_type>>(unit);
            const auto nanograms_conversion = basic_nanograms<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(grains_conversion.count() * 64800000)};
            return ToBasicUnit{nanograms_conversion};
        }

        template<mass_type ToBasicUnit, class ToSystem = typename ToBasicUnit::system, class Ratio, dimension_type Dimension>
            requires(std::same_as<ToSystem, imperial_system>)
        static constexpr ToBasicUnit system_cast(const mass<Rep, metric_system, Ratio, Dimension>& unit) noexcept
        {
            using common_type = std::common_type_t<Rep, typename ToBasicUnit::rep>;
            const auto nanograms_conversion = unit_cast<basic_nanograms<common_type>>(unit);
            const auto grains_conversion = basic_grains<typename ToBasicUnit::rep>{static_cast<typename ToBasicUnit::rep>(nanograms_conversion.count() * 0.000000015432)};
            return ToBasicUnit{grains_conversion};
        }
    };

    export
    template<class Rep, class Ratio, dimension_type Dimension, class CharT>
    struct suffix<mass<Rep, metric_system, Ratio, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("g"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_grains<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("gr"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_drachm<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("dr"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_ounces<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("oz"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_pounds<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("lb"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_stones<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("st"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_quarters<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("qr"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_hundredweights<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("cwt"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_short_tons<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("t"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_tons<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("t"))};
    };
}