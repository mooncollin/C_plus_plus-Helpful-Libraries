#pragma once

#include <ratio>
#include <limits>
#include <type_traits>
#include <iostream>
#include <sstream>
#include "measure.hpp"
#include "..\math.hpp"

namespace collin
{
    namespace measures
    {
        template<class Rep, class Interval = std::ratio<1>>
        class distance;
    }
}

namespace std
{
    template<class Rep1, class Interval1, class Rep2, class Interval2>
    struct common_type<collin::measures::distance<Rep1, Interval1>, collin::measures::distance<Rep2, Interval2>>
    {
        private:
            using num_ = collin::measures::static_gcd<Interval1::num, Interval2::num>;
            using den_ = collin::measures::static_gcd<Interval1::den, Interval2::den>;
            using cr_ = std::common_type_t<Rep1, Rep2>;
            using r_ = std::ratio<num_::value, (Interval1::den / den_::value) * Interval2::den>;
        public:
            using type = collin::measures::distance<cr_, r_>;
    };
}

namespace collin
{
    namespace measures
    {
        template<class T>
        struct is_distance : std::false_type {};

        template<class Rep, class Interval>
        struct is_distance<distance<Rep, Interval>> : std::true_type {};

        template<class Rep, class Interval>
        struct is_distance<const distance<Rep, Interval>> : std::true_type {};

        template<class Rep, class Interval>
        struct is_distance<distance<Rep, Interval>&> : std::true_type {};

        template<class Rep, class Interval>
        struct is_distance<const distance<Rep, Interval>&> : std::true_type {};

        template<class T>
        constexpr bool is_distance_v = is_distance<T>::value;

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
        };

        template<class ToDistance, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_distance_v<ToDistance>
        >>
        constexpr ToDistance distance_cast(const distance<Rep, Interval>& d)
        {
            using ratio = typename std::ratio_divide<Interval, typename ToDistance::interval>::type;
            using common_type = typename std::common_type_t<typename ToDistance::rep, Rep, intmax_t>;

            return ToDistance(
                static_cast<typename ToDistance::rep>(
                    static_cast<common_type>(d.count()) * static_cast<common_type>(ratio::num) / static_cast<common_type>(ratio::den)
                )
            );
        }

        template<class Rep, class Interval>
        class distance
        {
            public:
                using rep = Rep;
                using interval = Interval;

                static constexpr distance zero() noexcept
                {
                    return distance(distance_values<rep>::zero());
                }

                static constexpr distance min() noexcept
                {
                    return distance(distance_values<rep>::min());
                }

                static constexpr distance max() noexcept
                {
                    return distance(distance_values<rep>::max());
                }

                distance() = default;
                distance(const distance& dsn) = default;

                template<class Rep2, class Interval2>
                constexpr distance(const distance<Rep2, Interval2>& dsn)
                    : amount_(distance_cast<distance>(dsn).count())
                {
                }

                template<class Rep2>
                constexpr explicit distance(const Rep2& n)
                    : amount_(static_cast<rep>(n))
                {
                }

                distance& operator=(const distance& other) = default;

                ~distance() = default;

                constexpr rep count() const
                {
                    return amount_;
                }

                constexpr distance operator+() const
                {
                    return distance(*this);
                }

                constexpr distance operator-() const
                {
                    return distance(-amount_);
                }

                constexpr distance& operator++()
                {
                    ++amount_;
                    return *this;
                }

                constexpr distance operator++(int)
                {
                    return distance(amount_++);
                }

                constexpr distance& operator--()
                {
                    --amount_;
                    return *this;
                }

                constexpr distance operator--(int)
                {
                    return distance(amount_--);
                }

                constexpr distance& operator+=(const distance& d)
                {
                    amount_ += d.count();
                    return *this;
                }

                constexpr distance& operator-=(const distance& d)
                {
                    amount_ -= d.count();
                    return *this;
                }

                constexpr distance& operator*=(const rep& rhs)
                {
                    amount_ *= rhs;
                    return *this;
                }

                constexpr distance& operator/=(const rep& rhs)
                {
                    amount_ /= rhs;
                    return *this;
                }

                constexpr distance& operator%=(const rep& rhs)
                {
                    amount_ %= rhs;
                    return *this;
                }

                constexpr distance& operator%=(const distance& d)
                {
                    amount_ %= d.count();
                    return *this;
                }

            private:
                rep amount_;
        };

        using attometers = distance<intmax_t, std::atto>;
        using femtometers = distance<intmax_t, std::femto>;
        using picometers = distance<intmax_t, std::pico>;
        using nanometers = distance<intmax_t, std::nano>;
        using micrometers = distance<intmax_t, std::micro>;
        using millimeters = distance<intmax_t, std::milli>;
        using centimeters = distance<intmax_t, std::centi>;
        using decimeters = distance<intmax_t, std::deci>;
        using meters = distance<intmax_t>;
        using decameters = distance<intmax_t, std::deca>;
        using hectometers = distance<intmax_t, std::hecto>;
        using kilometers = distance<intmax_t, std::kilo>;
        using megameters = distance<intmax_t, std::mega>;
        using gigameters = distance<intmax_t, std::giga>;
        using terameters = distance<intmax_t, std::tera>;
        using petameters = distance<intmax_t, std::peta>;
        using exameters = distance<intmax_t, std::exa>;

        using inches = distance<double, std::ratio<254, 10000>>;
        using feet = distance<double, std::ratio<3048, 10000>>;
        using yards = distance<double, std::ratio<9144, 10000>>;
        using miles = distance<double, std::ratio<1000000000, 621371>>;

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>
        operator+(const distance<Rep1, Interval1> lhs, const distance<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() + common_type(rhs).count());
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>
        operator-(const distance<Rep1, Interval1> lhs, const distance<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() - common_type(rhs).count());
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        distance<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator*(const distance<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return distance<common_type, Interval>(static_cast<common_type>(d.count()) * static_cast<common_type>(r));
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        distance<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator*(const Rep2& r, const distance<Rep1, Interval>& d)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return distance<common_type, Interval>(static_cast<common_type>(d.count()) * static_cast<common_type>(r));
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        distance<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator/(const distance<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return distance<common_type, Interval>(static_cast<common_type>(d.count()) / static_cast<common_type>(r));
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>
        operator/(const distance<Rep1, Interval1> lhs, const distance<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() / common_type(rhs).count());
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        distance<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator%(const distance<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return distance<common_type, Interval>(static_cast<common_type>(d.count()) % static_cast<common_type>(r));
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>
        operator%(const distance<Rep1, Interval1> lhs, const distance<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() % common_type(rhs).count());
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator==(const distance<Rep1, Interval1>& lhs, const distance<Rep2, Interval2>& rhs)
        {
            using common_type = std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>;

            return common_type(lhs).count() == common_type(rhs).count();
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator!=(const distance<Rep1, Interval1>& lhs, const distance<Rep2, Interval2>& rhs)
        {
            return !(lhs == rhs);
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator<(const distance<Rep1, Interval1>& lhs, const distance<Rep2, Interval2>& rhs)
        {
            using common_type = std::common_type_t<distance<Rep1, Interval1>, distance<Rep2, Interval2>>;
            
            return common_type(lhs).count() < common_type(rhs).count();
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator>(const distance<Rep1, Interval1>& lhs, const distance<Rep2, Interval2>& rhs)
        {
            return rhs < lhs;
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator<=(const distance<Rep1, Interval1>& lhs, const distance<Rep2, Interval2>& rhs)
        {
            return !(rhs < lhs);
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator>=(const distance<Rep1, Interval1>& lhs, const distance<Rep2, Interval2>& rhs)
        {
            return !(lhs < rhs);
        }

        template<class ToDistance, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_distance_v<ToDistance>
        >>
        constexpr ToDistance floor(const distance<Rep, Interval>& d)
        {
            const auto d2 = distance_cast<ToDistance>(d);
            if(d2 > d)
            {
                return d2 - ToDistance{1};
            }

            return d2;
        }

        template<class ToDistance, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_distance_v<ToDistance>
        >>
        constexpr ToDistance ceil(const distance<Rep, Interval>& d)
        {
            const auto d2 = distance_cast<ToDistance>(d);
            if(d2 < d)
            {
                return d2 + ToDistance{1};
            }

            return d2;
        }

        template<class ToDistance, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_distance_v<ToDistance>
        >>
        constexpr ToDistance round(const distance<Rep, Interval>& d)
        {
            const auto d0 = floor<ToDistance>(d);
            const auto d1 = d0 + ToDistance{1};
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
/* requires */  distance<Rep, Interval>::min() < distance<Rep, Interval>::zero()
        >>
        constexpr distance<Rep, Interval> abs(const distance<Rep, Interval>& d)
        {
            return d >= d.zero() ? d : -d;
        }

        template<class Distance, typename = std::enable_if_t<
/* requires */ is_distance_v<Distance>
        >>
        constexpr std::string_view distance_suffix()
        {
            if constexpr(std::ratio_equal<typename Distance::interval::type, inches::interval::type>::value)
            {
                return "in";
            }
            else if constexpr(std::ratio_equal<typename Distance::interval::type, feet::interval::type>::value)
            {
                return "ft";
            }
            else if constexpr(std::ratio_equal<typename Distance::interval::type, yards::interval::type>::value)
            {
                return "yd";
            }
            else if constexpr(std::ratio_equal<typename Distance::interval::type, miles::interval::type>::value)
            {
                return "mi";
            }
            else
            {
                return "m";
            }
        }

        template<class CharT, class Traits, class Rep, class Interval>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const distance<Rep, Interval>& d)
        {
            std::basic_ostringstream<CharT, Traits> s;
            s.flags(os.flags());
            s.imbue(os.getloc());
            s.precision(os.precision());
            s << d.count() << measures::suffix<Interval>() << measures::distance_suffix<distance<Rep, Interval>>();
            return os << s.str();
        }
    }
}