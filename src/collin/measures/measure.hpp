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

        template<class Ratio>
        constexpr std::string_view suffix()
        {
            if constexpr(std::ratio_equal<Ratio, std::atto::type>::value)
            {
                return "a";
            }
            else if constexpr(std::ratio_equal<Ratio, std::femto::type>::value)
            {
                return "f";
            }
            else if constexpr(std::ratio_equal<Ratio, std::pico::type>::value)
            {
                return "p";
            }
            else if constexpr(std::ratio_equal<Ratio, std::nano::type>::value)
            {
                return "n";
            }
            else if constexpr(std::ratio_equal<Ratio, std::micro::type>::value)
            {
                return "µ";
            }
            else if constexpr(std::ratio_equal<Ratio, std::milli::type>::value)
            {
                return "m";
            }
            else if constexpr(std::ratio_equal<Ratio, std::centi::type>::value)
            {
                return "c";
            }
            else if constexpr(std::ratio_equal<Ratio, std::deci::type>::value)
            {
                return "d";
            }
            else if constexpr(std::ratio_equal<Ratio, std::ratio<1>::type>::value)
            {
                return "";
            }
            else if constexpr(std::ratio_equal<Ratio, std::deca::type>::value)
            {
                return "da";
            }
            else if constexpr(std::ratio_equal<Ratio, std::hecto::type>::value)
            {
                return "h";
            }
            else if constexpr(std::ratio_equal<Ratio, std::kilo::type>::value)
            {
                return "k";
            }
            else if constexpr(std::ratio_equal<Ratio, std::mega::type>::value)
            {
                return "M";
            }
            else if constexpr(std::ratio_equal<Ratio, std::giga::type>::value)
            {
                return "G";
            }
            else if constexpr(std::ratio_equal<Ratio, std::tera::type>::value)
            {
                return "T";
            }
            else if constexpr(std::ratio_equal<Ratio, std::peta::type>::value)
            {
                return "P";
            }
            else if constexpr(std::ratio_equal<Ratio, std::exa::type>::value)
            {
                return "E";
            }
            else
            {
                return "";
            }
        }
    }
}