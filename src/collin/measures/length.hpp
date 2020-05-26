#pragma once

#include <ratio>
#include <limits>
#include <type_traits>
#include <iostream>
#include <sstream>
#include <cstdint>
#include "measure.hpp"
#include "..\math.hpp"

namespace collin
{
    namespace measures
    {
        template<class Rep, class Interval = std::ratio<1>>
        class length;
    }
}

namespace std
{
    template<class Rep1, class Interval1, class Rep2, class Interval2>
    struct common_type<collin::measures::length<Rep1, Interval1>, collin::measures::length<Rep2, Interval2>>
    {
        private:
            using num_ = collin::measures::static_gcd<Interval1::num, Interval2::num>;
            using den_ = collin::measures::static_gcd<Interval1::den, Interval2::den>;
            using cr_ = std::common_type_t<Rep1, Rep2>;
            using r_ = std::ratio<num_::value, (Interval1::den / den_::value) * Interval2::den>;
        public:
            using type = collin::measures::length<cr_, r_>;
    };
}

namespace collin
{
    namespace measures
    {
        template<class T>
        struct is_length : std::false_type {};

        template<class Rep, class Interval>
        struct is_length<length<Rep, Interval>> : std::true_type {};


        template<class T>
        constexpr bool is_length_v = is_length<T>::value;

        template<class Rep>
        struct length_values
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
        };

        template<class ToLength, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_length_v<ToLength>
        >>
        constexpr ToLength length_cast(const length<Rep, Interval>& d)
        {
            using ratio = typename std::ratio_divide<Interval, typename ToLength::interval>::type;
            using common_type = typename std::common_type_t<typename ToLength::rep, Rep, std::intmax_t>;

            return ToLength(
                static_cast<typename ToLength::rep>(
                    static_cast<common_type>(d.count()) * static_cast<common_type>(ratio::num) / static_cast<common_type>(ratio::den)
                )
            );
        }

        template<class Rep, class Interval>
        class length
        {
            public:
                using rep = Rep;
                using interval = Interval;

                static constexpr length zero() noexcept
                {
                    return length(length_values<rep>::zero());
                }

                static constexpr length min() noexcept
                {
                    return length(length_values<rep>::min());
                }

                static constexpr length max() noexcept
                {
                    return length(length_values<rep>::max());
                }

                length() = default;
                length(const length& dsn) = default;

                template<class Rep2, class Interval2>
                constexpr length(const length<Rep2, Interval2>& dsn)
                    : amount_(length_cast<length>(dsn).count())
                {
                }

                template<class Rep2>
                constexpr explicit length(const Rep2& n)
                    : amount_(static_cast<rep>(n))
                {
                }

                length& operator=(const length& other) = default;

                ~length() = default;

                constexpr rep count() const
                {
                    return amount_;
                }

                constexpr length operator+() const
                {
                    return length(*this);
                }

                constexpr length operator-() const
                {
                    return length(-amount_);
                }

                constexpr length& operator++()
                {
                    ++amount_;
                    return *this;
                }

                constexpr length operator++(int)
                {
                    return length(amount_++);
                }

                constexpr length& operator--()
                {
                    --amount_;
                    return *this;
                }

                constexpr length operator--(int)
                {
                    return length(amount_--);
                }

                constexpr length& operator+=(const length& d)
                {
                    amount_ += d.count();
                    return *this;
                }

                constexpr length& operator-=(const length& d)
                {
                    amount_ -= d.count();
                    return *this;
                }

                constexpr length& operator*=(const rep& rhs)
                {
                    amount_ *= rhs;
                    return *this;
                }

                constexpr length& operator/=(const rep& rhs)
                {
                    amount_ /= rhs;
                    return *this;
                }

                constexpr length& operator%=(const rep& rhs)
                {
                    amount_ %= rhs;
                    return *this;
                }

                constexpr length& operator%=(const length& d)
                {
                    amount_ %= d.count();
                    return *this;
                }

            private:
                rep amount_;
        };

        using attometers = length<std::intmax_t, std::atto>;
        using femtometers = length<std::intmax_t, std::femto>;
        using picometers = length<std::intmax_t, std::pico>;
        using nanometers = length<std::intmax_t, std::nano>;
        using micrometers = length<std::intmax_t, std::micro>;
        using millimeters = length<std::intmax_t, std::milli>;
        using centimeters = length<std::intmax_t, std::centi>;
        using decimeters = length<std::intmax_t, std::deci>;
        using meters = length<std::intmax_t>;
        using decameters = length<std::intmax_t, std::deca>;
        using hectometers = length<std::intmax_t, std::hecto>;
        using kilometers = length<std::intmax_t, std::kilo>;
        using megameters = length<std::intmax_t, std::mega>;
        using gigameters = length<std::intmax_t, std::giga>;
        using terameters = length<std::intmax_t, std::tera>;
        using petameters = length<std::intmax_t, std::peta>;
        using exameters = length<std::intmax_t, std::exa>;

        using inches = length<double, std::ratio<254, 10000>>;
        using feet = length<double, std::ratio<3048, 10000>>;
        using yards = length<double, std::ratio<9144, 10000>>;
        using miles = length<double, std::ratio<1000000000, 621371>>;

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>
        operator+(const length<Rep1, Interval1> lhs, const length<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() + common_type(rhs).count());
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>
        operator-(const length<Rep1, Interval1> lhs, const length<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() - common_type(rhs).count());
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        length<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator*(const length<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return length<common_type, Interval>(static_cast<common_type>(d.count()) * static_cast<common_type>(r));
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        length<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator*(const Rep2& r, const length<Rep1, Interval>& d)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return length<common_type, Interval>(static_cast<common_type>(d.count()) * static_cast<common_type>(r));
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        length<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator/(const length<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return length<common_type, Interval>(static_cast<common_type>(d.count()) / static_cast<common_type>(r));
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>
        operator/(const length<Rep1, Interval1> lhs, const length<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() / common_type(rhs).count());
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        length<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator%(const length<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return length<common_type, Interval>(static_cast<common_type>(d.count()) % static_cast<common_type>(r));
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>
        operator%(const length<Rep1, Interval1> lhs, const length<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() % common_type(rhs).count());
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator==(const length<Rep1, Interval1>& lhs, const length<Rep2, Interval2>& rhs)
        {
            using common_type = std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>;

            return common_type(lhs).count() == common_type(rhs).count();
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator!=(const length<Rep1, Interval1>& lhs, const length<Rep2, Interval2>& rhs)
        {
            return !(lhs == rhs);
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator<(const length<Rep1, Interval1>& lhs, const length<Rep2, Interval2>& rhs)
        {
            using common_type = std::common_type_t<length<Rep1, Interval1>, length<Rep2, Interval2>>;
            
            return common_type(lhs).count() < common_type(rhs).count();
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator>(const length<Rep1, Interval1>& lhs, const length<Rep2, Interval2>& rhs)
        {
            return rhs < lhs;
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator<=(const length<Rep1, Interval1>& lhs, const length<Rep2, Interval2>& rhs)
        {
            return !(rhs < lhs);
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator>=(const length<Rep1, Interval1>& lhs, const length<Rep2, Interval2>& rhs)
        {
            return !(lhs < rhs);
        }

        template<class ToLength, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_length_v<ToLength>
        >>
        constexpr ToLength floor(const length<Rep, Interval>& d)
        {
            const auto d2 = length_cast<ToLength>(d);
            if(d2 > d)
            {
                return d2 - ToLength{1};
            }

            return d2;
        }

        template<class ToLength, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_length_v<ToLength>
        >>
        constexpr ToLength ceil(const length<Rep, Interval>& d)
        {
            const auto d2 = length_cast<ToLength>(d);
            if(d2 < d)
            {
                return d2 + ToLength{1};
            }

            return d2;
        }

        template<class ToLength, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_length_v<ToLength>
        >>
        constexpr ToLength round(const length<Rep, Interval>& d)
        {
            const auto d0 = floor<ToLength>(d);
            const auto d1 = d0 + ToLength{1};
            const auto diff0 = d - d0;
            const auto diff1 = d1 - d;
            if(diff0 == diff1)
            {
                if(math::is_odd(d0.count()))
                {
                    return d1;
                }
                return d0;
            }
            else if(diff0 < diff1)
            {
                return d0;
            }

            return d1;
        }

        template<class Rep, class Interval, typename = std::enable_if_t<
/* requires */  length<Rep, Interval>::min() < length<Rep, Interval>::zero()
        >>
        constexpr length<Rep, Interval> abs(const length<Rep, Interval>& d)
        {
            return d >= d.zero() ? d : -d;
        }

        template<class length, typename = std::enable_if_t<
/* requires */ is_length_v<length>
        >>
        struct length_suffix_impl
        {
            constexpr static std::string_view value {"m"};
        };

        template<>
        struct length_suffix_impl<inches>
        {
            constexpr static std::string_view value {"in"};
        };

        template<>
        struct length_suffix_impl<feet>
        {
            constexpr static std::string_view value {"ft"};
        };

        template<>
        struct length_suffix_impl<yards>
        {
            constexpr static std::string_view value {"yd"};
        };

        template<>
        struct length_suffix_impl<miles>
        {
            constexpr static std::string_view value {"mi"};
        };

        template<class length, typename = std::enable_if_t<
/* requires */ is_length_v<length>
        >>
        struct length_suffix
        {
            using type = typename length::interval::type;
            constexpr static auto value = length_suffix_impl<length>::value;
        };

        template<class T>
        constexpr auto length_suffix_v = length_suffix<T>::value;

        template<class CharT, class Traits, class Rep, class Interval>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const length<Rep, Interval>& d)
        {
            std::basic_ostringstream<CharT, Traits> s;
            s.flags(os.flags());
            s.imbue(os.getloc());
            s.precision(os.precision());
            s << d.count() << measures::suffix_v<Interval> << measures::length_suffix_v<length<Rep, Interval>>;
            return os << s.str();
        }
    }
}