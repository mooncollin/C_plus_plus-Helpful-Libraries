#pragma once

#include <chrono>
#include <ratio>
#include <cstdint>

#include "measure.hpp"

namespace collin
{
	namespace measures
	{
		template<class Rep>
		struct time_values : public std::chrono::duration_values<Rep> {};

		template<class Rep, class System, class Ratio = std::ratio<1>>
		using time = basic_unit<Rep, Ratio, time_values<Rep>, System>;

        template<class Rep>
        using basic_attoseconds = time<Rep, metric_system, std::atto>;

        template<class Rep>
        using basic_femtoseconds = time<Rep, metric_system, std::femto>;

        template<class Rep>
        using basic_picoseconds = time<Rep, metric_system, std::pico>;

        template<class Rep>
        using basic_nanoseconds = time<Rep, metric_system, std::nano>;

        template<class Rep>
        using basic_microseconds = time<Rep, metric_system, std::micro>;

        template<class Rep>
        using basic_milliseconds = time<Rep, metric_system, std::milli>;

        template<class Rep>
        using basic_centiseconds = time<Rep, metric_system, std::centi>;

        template<class Rep>
        using basic_deciseconds = time<Rep, metric_system, std::deci>;

        template<class Rep>
        using basic_seconds = time<Rep, metric_system>;

        template<class Rep>
        using basic_decaseconds = time<Rep, metric_system, std::deca>;

        template<class Rep>
        using basic_hectoseconds = time<Rep, metric_system, std::hecto>;

        template<class Rep>
        using basic_kiloseconds = time<Rep, metric_system, std::kilo>;

        template<class Rep>
        using basic_megaseconds = time<Rep, metric_system, std::mega>;

        template<class Rep>
        using basic_gigaseconds = time<Rep, metric_system, std::giga>;

        template<class Rep>
        using basic_teraseconds = time<Rep, metric_system, std::tera>;

        template<class Rep>
        using basic_petaseconds = time<Rep, metric_system, std::peta>;

        template<class Rep>
        using basic_exaseconds = time<Rep, metric_system, std::exa>;

        template<class Rep>
        using basic_minutes = time<Rep, metric_system, std::ratio<60>>;

        template<class Rep>
        using basic_hours = time<Rep, metric_system, std::ratio<3600>>;

        template<class Rep>
        using basic_days = time<Rep, metric_system, std::ratio<86400>>;

        template<class Rep>
        using basic_weeks = time<Rep, metric_system, std::ratio<604800>>;

        template<class Rep>
        using basic_months = time<Rep, metric_system, std::ratio<2629746>>;

        template<class Rep>
        using basic_years = time<Rep, metric_system, std::ratio<31556952>>;

        using attoseconds = basic_attoseconds<std::intmax_t>;
        using femtoseconds = basic_femtoseconds<std::intmax_t>;
        using picoseconds = basic_picoseconds<std::intmax_t>;
        using nanoseconds = basic_nanoseconds<std::intmax_t>;
        using microseconds = basic_microseconds<std::intmax_t>;
        using milliseconds = basic_milliseconds<std::intmax_t>;
        using centiseconds = basic_centiseconds<std::intmax_t>;
        using deciseconds = basic_deciseconds<std::intmax_t>;
        using seconds = basic_seconds<std::intmax_t>;
        using decaseconds = basic_decaseconds<std::intmax_t>;
        using hectoseconds = basic_hectoseconds<std::intmax_t>;
        using kiloseconds = basic_kiloseconds<std::intmax_t>;
        using megaseconds = basic_megaseconds<std::intmax_t>;
        using gigaseconds = basic_gigaseconds<std::intmax_t>;
        using teraseconds = basic_teraseconds<std::intmax_t>;
        using petaseconds = basic_petaseconds<std::intmax_t>;
        using exaseconds = basic_exaseconds<std::intmax_t>;
        using minutes = basic_minutes<std::intmax_t>;
        using hours = basic_hours<std::intmax_t>;
        using days = basic_days<std::intmax_t>;
        using weeks = basic_weeks<std::intmax_t>;
        using months = basic_months<std::intmax_t>;
        using years = basic_years<std::intmax_t>;

        template<class Rep>
        struct metric_system::suffix<basic_attoseconds<Rep>>
        {
            constexpr static std::string_view value {"as"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_femtoseconds<Rep>>
        {
            constexpr static std::string_view value {"fs"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_picoseconds<Rep>>
        {
            constexpr static std::string_view value {"ps"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_nanoseconds<Rep>>
        {
            constexpr static std::string_view value {"ns"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_microseconds<Rep>>
        {
            constexpr static std::string_view value {"us"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_milliseconds<Rep>>
        {
            constexpr static std::string_view value {"ms"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_centiseconds<Rep>>
        {
            constexpr static std::string_view value {"cs"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_deciseconds<Rep>>
        {
            constexpr static std::string_view value {"ds"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_seconds<Rep>>
        {
            constexpr static std::string_view value {"s"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_decaseconds<Rep>>
        {
            constexpr static std::string_view value {"das"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_hectoseconds<Rep>>
        {
            constexpr static std::string_view value {"hs"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_kiloseconds<Rep>>
        {
            constexpr static std::string_view value {"ks"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_megaseconds<Rep>>
        {
            constexpr static std::string_view value {"Ms"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_gigaseconds<Rep>>
        {
            constexpr static std::string_view value {"Gs"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_teraseconds<Rep>>
        {
            constexpr static std::string_view value {"Ts"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_petaseconds<Rep>>
        {
            constexpr static std::string_view value {"Ps"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_exaseconds<Rep>>
        {
            constexpr static std::string_view value {"Es"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_minutes<Rep>>
        {
            constexpr static std::string_view value {"min"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_hours<Rep>>
        {
            constexpr static std::string_view value {"h"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_days<Rep>>
        {
            constexpr static std::string_view value {"d"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_weeks<Rep>>
        {
            constexpr static std::string_view value {"w"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_months<Rep>>
        {
            constexpr static std::string_view value {"mo"};
        };

        template<class Rep>
        struct metric_system::suffix<basic_years<Rep>>
        {
            constexpr static std::string_view value {"y"};
        };

        template<class Rep, class Ratio>
        constexpr std::chrono::duration<Rep, Ratio> to_chrono(const basic_unit<Rep, Ratio, time_values<Rep>, metric_system>& t)
        {
            return std::chrono::duration<Rep, Ratio>(t.count());
        }
	}
}