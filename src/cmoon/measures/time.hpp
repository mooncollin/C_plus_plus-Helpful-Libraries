#pragma once

#include <chrono>
#include <ratio>
#include <cstdint>

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep>
		struct time_values : public std::chrono::duration_values<Rep> {};

		template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
		using time = basic_unit<Rep, Ratio, time_values<Rep>, System, Dimension>;

        template<class T>
        struct is_time : std::false_type {};

        template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
        struct is_time<time<Rep, System, Ratio, Dimension>> : std::true_type {};

        template<class T>
        constexpr bool is_time_v = is_time<T>::value;

        template<class T>
        concept time_type = is_time_v<T>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_attoseconds = time<Rep, metric_system, std::atto, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_femtoseconds = time<Rep, metric_system, std::femto, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_picoseconds = time<Rep, metric_system, std::pico, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_nanoseconds = time<Rep, metric_system, std::nano, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_microseconds = time<Rep, metric_system, std::micro, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_milliseconds = time<Rep, metric_system, std::milli, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_centiseconds = time<Rep, metric_system, std::centi, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_deciseconds = time<Rep, metric_system, std::deci, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_seconds = time<Rep, metric_system, std::ratio<1>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_decaseconds = time<Rep, metric_system, std::deca, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_hectoseconds = time<Rep, metric_system, std::hecto, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_kiloseconds = time<Rep, metric_system, std::kilo, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_megaseconds = time<Rep, metric_system, std::mega, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_gigaseconds = time<Rep, metric_system, std::giga, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_teraseconds = time<Rep, metric_system, std::tera, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_petaseconds = time<Rep, metric_system, std::peta, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_exaseconds = time<Rep, metric_system, std::exa, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_minutes = time<Rep, metric_system, std::ratio<60>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_hours = time<Rep, metric_system, std::ratio<3600>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_days = time<Rep, metric_system, std::ratio<86400>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_weeks = time<Rep, metric_system, std::ratio<604800>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_months = time<Rep, metric_system, std::ratio<2629746>, Dimension>;

        template<class Rep, dimension_type Dimension = 1>
        using basic_years = time<Rep, metric_system, std::ratio<31556952>, Dimension>;

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

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_attoseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"as"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_femtoseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"fs"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_picoseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"ps"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_nanoseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"ns"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_microseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"us"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_milliseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"ms"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_centiseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"cs"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_deciseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"ds"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_seconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"s"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_decaseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"das"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_hectoseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"hs"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_kiloseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"ks"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_megaseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Ms"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_gigaseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Gs"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_teraseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Ts"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_petaseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Ps"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_exaseconds<Rep, Dimension>>
        {
            constexpr static std::string_view value {"Es"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_minutes<Rep, Dimension>>
        {
            constexpr static std::string_view value {"min"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_hours<Rep, Dimension>>
        {
            constexpr static std::string_view value {"h"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_days<Rep, Dimension>>
        {
            constexpr static std::string_view value {"d"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_weeks<Rep, Dimension>>
        {
            constexpr static std::string_view value {"w"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_months<Rep, Dimension>>
        {
            constexpr static std::string_view value {"mo"};
        };

        template<class Rep, dimension_type Dimension>
        struct metric_system::suffix<basic_years<Rep, Dimension>>
        {
            constexpr static std::string_view value {"y"};
        };

        template<time_type Time>
        using chrono_t = std::chrono::duration<typename Time::rep, typename Time::ratio>;

        template<class Rep, cmoon::ratio_type Ratio>
        constexpr std::chrono::duration<Rep, Ratio> to_chrono(const basic_unit<Rep, Ratio, time_values<Rep>, metric_system, 1>& t)
        {
            return std::chrono::duration<Rep, Ratio>{t.count()};
        }

        template<class Rep, class Period>
        constexpr basic_unit<Rep, Period, time_values<Rep>, metric_system, 1> from_chrono(const std::chrono::duration<Rep, Period>& d)
        {
            return basic_unit<Rep, Period, time_values<Rep>, metric_system, 1>{d.count()};
        }
	}
}