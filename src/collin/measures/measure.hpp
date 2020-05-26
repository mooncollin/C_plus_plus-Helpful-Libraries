#pragma once

#include <ratio>
#include <string_view>
#include <type_traits>
#include <stdexcept>

namespace collin
{
    namespace measures
    {
        template<intmax_t Pn>
        struct static_sign : std::integral_constant<intmax_t, (Pn < 0) ? -1 : 1> {};

        template<intmax_t Pn>
        struct static_abs : std::integral_constant<intmax_t, Pn * static_sign<Pn>::value> {};
        
        template<intmax_t Pn, intmax_t Qn>
        struct static_gcd : static_gcd<Qn, (Pn % Qn)> {};

        template<intmax_t Pn>
        struct static_gcd<Pn, 0> : std::integral_constant<intmax_t, static_abs<Pn>::value> {};

        template<intmax_t Qn>
        struct static_gcd<0, Qn> : std::integral_constant<intmax_t, static_abs<Qn>::value> {};

        template<class T>
        struct is_ratio : std::false_type {};

        template<std::intmax_t N, std::intmax_t D>
        struct is_ratio<std::ratio<N, D>> : std::true_type {};

        template<class T>
        constexpr bool is_ratio_v = is_ratio<T>::value;

        template<class Ratio, typename = std::enable_if_t<
/* requires */ is_ratio_v<Ratio>
        >>
        struct suffix_impl
        {
            constexpr static std::string_view value {""};
        };

        template<>
        struct suffix_impl<std::atto>
        {
            constexpr static std::string_view value {"a"};
        };

        template<>
        struct suffix_impl<std::femto>
        {
            constexpr static std::string_view value {"f"};
        };

        template<>
        struct suffix_impl<std::pico>
        {
            constexpr static std::string_view value {"p"};
        };

        template<>
        struct suffix_impl<std::nano>
        {
            constexpr static std::string_view value {"n"};
        };

        template<>
        struct suffix_impl<std::micro>
        {
            constexpr static std::string_view value {"µ"};
        };

        template<>
        struct suffix_impl<std::milli>
        {
            constexpr static std::string_view value {"m"};
        };

        template<>
        struct suffix_impl<std::centi>
        {
            constexpr static std::string_view value {"c"};
        };

        template<>
        struct suffix_impl<std::deci>
        {
            constexpr static std::string_view value {"d"};
        };

        template<>
        struct suffix_impl<std::deca>
        {
            constexpr static std::string_view value {"da"};
        };

        template<>
        struct suffix_impl<std::hecto>
        {
            constexpr static std::string_view value {"h"};
        };

        template<>
        struct suffix_impl<std::kilo>
        {
            constexpr static std::string_view value {"k"};
        };

        template<>
        struct suffix_impl<std::mega>
        {
            constexpr static std::string_view value {"M"};
        };

        template<>
        struct suffix_impl<std::giga>
        {
            constexpr static std::string_view value {"G"};
        };

        template<>
        struct suffix_impl<std::tera>
        {
            constexpr static std::string_view value {"T"};
        };

        template<>
        struct suffix_impl<std::peta>
        {
            constexpr static std::string_view value {"P"};
        };

        template<>
        struct suffix_impl<std::exa>
        {
            constexpr static std::string_view value {"E"};
        };

        template<class Ratio, typename = std::enable_if_t<
/* requires */  is_ratio_v<Ratio>
        >>
        struct suffix
        {
            constexpr static auto value = suffix_impl<Ratio>::value;
        };

        template<class T>
        constexpr auto suffix_v = suffix<T>::value;
    }
}
