module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.time;

import <chrono>;
import <ratio>;
import <cstdint>;

import cmoon.ratio;
import cmoon.string;

import cmoon.measures;

namespace cmoon::measures
{
    export
	template<class Rep>
	struct time_values : public std::chrono::duration_values<Rep> {};

    export
	template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
	using time = basic_unit<Rep, Ratio, time_values<Rep>, System, Dimension>;

    template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
    std::true_type is_time_base_impl(const time<Rep, System, Ratio, Dimension>&);

    std::false_type is_time_base_impl(...);

    template<class U>
    constexpr auto is_based_in_time = decltype(is_time_base_impl(std::declval<U>()))::value;

    export
    template<class T>
    struct is_time : std::bool_constant<is_based_in_time<T>> {};

    export
    template<class T>
    constexpr bool is_time_v = is_time<T>::value;

    export
    template<class T>
    concept time_type = is_time_v<T>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_attoseconds = time<Rep, metric_system, std::atto, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_femtoseconds = time<Rep, metric_system, std::femto, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_picoseconds = time<Rep, metric_system, std::pico, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_nanoseconds = time<Rep, metric_system, std::nano, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_microseconds = time<Rep, metric_system, std::micro, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_milliseconds = time<Rep, metric_system, std::milli, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_centiseconds = time<Rep, metric_system, std::centi, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_deciseconds = time<Rep, metric_system, std::deci, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_seconds = time<Rep, metric_system, std::ratio<1>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_decaseconds = time<Rep, metric_system, std::deca, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_hectoseconds = time<Rep, metric_system, std::hecto, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_kiloseconds = time<Rep, metric_system, std::kilo, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_megaseconds = time<Rep, metric_system, std::mega, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_gigaseconds = time<Rep, metric_system, std::giga, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_teraseconds = time<Rep, metric_system, std::tera, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_petaseconds = time<Rep, metric_system, std::peta, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_exaseconds = time<Rep, metric_system, std::exa, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_minutes = time<Rep, metric_system, std::ratio<60>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_hours = time<Rep, metric_system, std::ratio<3600>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_days = time<Rep, metric_system, std::ratio<86400>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_weeks = time<Rep, metric_system, std::ratio<604800>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_months = time<Rep, metric_system, std::ratio<2629746>, Dimension>;

    export
    template<class Rep, dimension_type Dimension = 1>
    using basic_years = time<Rep, metric_system, std::ratio<31556952>, Dimension>;

    export
    using attoseconds = basic_attoseconds<std::intmax_t>;
    
    export
    using femtoseconds = basic_femtoseconds<std::intmax_t>;
    
    export
    using picoseconds = basic_picoseconds<std::intmax_t>;
    
    export
    using nanoseconds = basic_nanoseconds<std::intmax_t>;
    
    export
    using microseconds = basic_microseconds<std::intmax_t>;
    
    export
    using milliseconds = basic_milliseconds<std::intmax_t>;
    
    export
    using centiseconds = basic_centiseconds<std::intmax_t>;
    
    export
    using deciseconds = basic_deciseconds<std::intmax_t>;
    
    export
    using seconds = basic_seconds<std::intmax_t>;
    
    export
    using decaseconds = basic_decaseconds<std::intmax_t>;
    
    export
    using hectoseconds = basic_hectoseconds<std::intmax_t>;
    
    export
    using kiloseconds = basic_kiloseconds<std::intmax_t>;
    
    export
    using megaseconds = basic_megaseconds<std::intmax_t>;
    
    export
    using gigaseconds = basic_gigaseconds<std::intmax_t>;
    
    export
    using teraseconds = basic_teraseconds<std::intmax_t>;
    
    export
    using petaseconds = basic_petaseconds<std::intmax_t>;
    
    export
    using exaseconds = basic_exaseconds<std::intmax_t>;
    
    export
    using minutes = basic_minutes<std::intmax_t>;
    
    export
    using hours = basic_hours<std::intmax_t>;
    
    export
    using days = basic_days<std::intmax_t>;
    
    export
    using weeks = basic_weeks<std::intmax_t>;
    
    export
    using months = basic_months<std::intmax_t>;
    
    export
    using years = basic_years<std::intmax_t>;

    export
    template<class Rep, cmoon::ratio_type Ratio, dimension_type Dimension, class CharT>
    struct suffix<time<Rep, metric_system, Ratio, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("s"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_minutes<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("min"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_hours<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("h"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_days<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("d"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_weeks<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("w"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_months<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("mo"))};
    };

    export
    template<class Rep, dimension_type Dimension, class CharT>
    struct suffix<basic_years<Rep, Dimension>, CharT>
    {
        static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("y"))};
    };

    export
    template<time_type Time>
    using chrono_t = std::chrono::duration<typename Time::rep, typename Time::ratio>;

    export
    template<class Rep, cmoon::ratio_type Ratio>
    constexpr std::chrono::duration<Rep, Ratio> to_chrono(const basic_unit<Rep, Ratio, time_values<Rep>, metric_system, 1>& t)
    {
        return std::chrono::duration<Rep, Ratio>{t.count()};
    }

    export
    template<class Rep, class Period>
    constexpr basic_unit<Rep, Period, time_values<Rep>, metric_system, 1> from_chrono(const std::chrono::duration<Rep, Period>& d)
    {
        return basic_unit<Rep, Period, time_values<Rep>, metric_system, 1>{d.count()};
    }
}