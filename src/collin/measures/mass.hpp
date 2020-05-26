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
        template<class Rep, class Ratio = std::ratio<1>>
        class mass;
    }
}

namespace std
{
    template<class Rep1, class Interval1, class Rep2, class Interval2>
    struct common_type<collin::measures::mass<Rep1, Interval1>, collin::measures::mass<Rep2, Interval2>>
    {
        private:
            using num_ = collin::measures::static_gcd<Interval1::num, Interval2::num>;
            using den_ = collin::measures::static_gcd<Interval1::den, Interval2::den>;
            using cr_ = std::common_type_t<Rep1, Rep2>;
            using r_ = std::ratio<num_::value, (Interval1::den / den_::value) * Interval2::den>;
        public:
            using type = collin::measures::mass<cr_, r_>;
    };
}

namespace collin
{
    namespace measures
    {
        template<class T>
        struct is_mass : std::false_type {};

        template<class Rep, class Interval>
        struct is_mass<mass<Rep, Interval>> : std::true_type {};


        template<class T>
        constexpr bool is_mass_v = is_mass<T>::value;

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
        };

        template<class ToMass, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_mass_v<ToMass>
        >>
        constexpr ToMass mass_cast(const mass<Rep, Interval>& d)
        {
            using ratio = typename std::ratio_divide<Interval, typename ToMass::interval>::type;
            using common_type = typename std::common_type_t<typename ToMass::rep, Rep, std::intmax_t>;

            return ToMass(
                static_cast<typename ToMass::rep>(
                    static_cast<common_type>(d.count()) * static_cast<common_type>(ratio::num) / static_cast<common_type>(ratio::den)
                )
            );
        }

        template<class Rep, class Interval>
        class mass
        {
            public:
                using rep = Rep;
                using interval = Interval;

                static constexpr mass zero() noexcept
                {
                    return mass(mass_values<rep>::zero());
                }

                static constexpr mass min() noexcept
                {
                    return mass(mass_values<rep>::min());
                }

                static constexpr mass max() noexcept
                {
                    return mass(mass_values<rep>::max());
                }

                mass() = default;
                mass(const mass& dsn) = default;

                template<class Rep2, class Interval2>
                constexpr mass(const mass<Rep2, Interval2>& dsn)
                    : amount_(mass_cast<mass>(dsn).count())
                {
                }

                template<class Rep2>
                constexpr explicit mass(const Rep2& n)
                    : amount_(static_cast<rep>(n))
                {
                }

                mass& operator=(const mass& other) = default;

                ~mass() = default;

                constexpr rep count() const
                {
                    return amount_;
                }

                constexpr mass operator+() const
                {
                    return mass(*this);
                }

                constexpr mass operator-() const
                {
                    return mass(-amount_);
                }

                constexpr mass& operator++()
                {
                    ++amount_;
                    return *this;
                }

                constexpr mass operator++(int)
                {
                    return mass(amount_++);
                }

                constexpr mass& operator--()
                {
                    --amount_;
                    return *this;
                }

                constexpr mass operator--(int)
                {
                    return mass(amount_--);
                }

                constexpr mass& operator+=(const mass& d)
                {
                    amount_ += d.count();
                    return *this;
                }

                constexpr mass& operator-=(const mass& d)
                {
                    amount_ -= d.count();
                    return *this;
                }

                constexpr mass& operator*=(const rep& rhs)
                {
                    amount_ *= rhs;
                    return *this;
                }

                constexpr mass& operator/=(const rep& rhs)
                {
                    amount_ /= rhs;
                    return *this;
                }

                constexpr mass& operator%=(const rep& rhs)
                {
                    amount_ %= rhs;
                    return *this;
                }

                constexpr mass& operator%=(const mass& d)
                {
                    amount_ %= d.count();
                    return *this;
                }

            private:
                rep amount_;
        };

        using attograms = mass<std::intmax_t, std::atto>;
        using femtograms = mass<std::intmax_t, std::femto>;
        using picograms = mass<std::intmax_t, std::pico>;
        using nanograms = mass<std::intmax_t, std::nano>;
        using micrograms = mass<std::intmax_t, std::micro>;
        using milligrams = mass<std::intmax_t, std::milli>;
        using centigrams = mass<std::intmax_t, std::centi>;
        using decigrams = mass<std::intmax_t, std::deci>;
        using grams = mass<std::intmax_t>;
        using decagrams = mass<std::intmax_t, std::deca>;
        using hectograms = mass<std::intmax_t, std::hecto>;
        using kilograms = mass<std::intmax_t, std::kilo>;
        using megagrams = mass<std::intmax_t, std::mega>;
        using gigagrams = mass<std::intmax_t, std::giga>;
        using teragrams = mass<std::intmax_t, std::tera>;
        using petagrams = mass<std::intmax_t, std::peta>;
        using exagrams = mass<std::intmax_t, std::exa>;

        using ounces = mass<double, std::ratio<1000000, 35274>>;
        using pounds = mass<double, std::ratio<100000000, 220462>>;
        using stones = mass<double, std::ratio<1000000000, 157473>>;
        using imperial_tons = mass<double, std::ratio<1000000000000000, 984206536>>;
        using us_tons = mass<double, std::ratio<1000000000000000, 1102311310>>;

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>
        operator+(const mass<Rep1, Interval1> lhs, const mass<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() + common_type(rhs).count());
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>
        operator-(const mass<Rep1, Interval1> lhs, const mass<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() - common_type(rhs).count());
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        mass<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator*(const mass<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return mass<common_type, Interval>(static_cast<common_type>(d.count()) * static_cast<common_type>(r));
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        mass<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator*(const Rep2& r, const mass<Rep1, Interval>& d)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return mass<common_type, Interval>(static_cast<common_type>(d.count()) * static_cast<common_type>(r));
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        mass<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator/(const mass<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return mass<common_type, Interval>(static_cast<common_type>(d.count()) / static_cast<common_type>(r));
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>
        operator/(const mass<Rep1, Interval1> lhs, const mass<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() / common_type(rhs).count());
        }

        template<class Rep1, class Interval, class Rep2>
        constexpr
        mass<typename std::common_type_t<Rep1, Rep2>, Interval>
        operator%(const mass<Rep1, Interval>& d, const Rep2& r)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return mass<common_type, Interval>(static_cast<common_type>(d.count()) % static_cast<common_type>(r));
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr
        typename std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>
        operator%(const mass<Rep1, Interval1> lhs, const mass<Rep2, Interval2> rhs)
        {
            using common_type = std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>;
            
            return common_type(common_type(lhs).count() % common_type(rhs).count());
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator==(const mass<Rep1, Interval1>& lhs, const mass<Rep2, Interval2>& rhs)
        {
            using common_type = std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>;

            return common_type(lhs).count() == common_type(rhs).count();
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator!=(const mass<Rep1, Interval1>& lhs, const mass<Rep2, Interval2>& rhs)
        {
            return !(lhs == rhs);
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator<(const mass<Rep1, Interval1>& lhs, const mass<Rep2, Interval2>& rhs)
        {
            using common_type = std::common_type_t<mass<Rep1, Interval1>, mass<Rep2, Interval2>>;
            
            return common_type(lhs).count() < common_type(rhs).count();
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator>(const mass<Rep1, Interval1>& lhs, const mass<Rep2, Interval2>& rhs)
        {
            return rhs < lhs;
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator<=(const mass<Rep1, Interval1>& lhs, const mass<Rep2, Interval2>& rhs)
        {
            return !(rhs < lhs);
        }

        template<class Rep1, class Interval1, class Rep2, class Interval2>
        constexpr bool operator>=(const mass<Rep1, Interval1>& lhs, const mass<Rep2, Interval2>& rhs)
        {
            return !(lhs < rhs);
        }

        template<class ToMass, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_mass_v<ToMass>
        >>
        constexpr ToMass floor(const mass<Rep, Interval>& d)
        {
            const auto d2 = mass_cast<ToMass>(d);
            if(d2 > d)
            {
                return d2 - ToMass{1};
            }

            return d2;
        }

        template<class ToMass, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_mass_v<ToMass>
        >>
        constexpr ToMass ceil(const mass<Rep, Interval>& d)
        {
            const auto d2 = mass_cast<ToMass>(d);
            if(d2 < d)
            {
                return d2 + ToMass{1};
            }

            return d2;
        }

        template<class ToMass, class Rep, class Interval, typename = std::enable_if_t<
/* requires */  is_mass_v<ToMass>
        >>
        constexpr ToMass round(const mass<Rep, Interval>& d)
        {
            const auto d0 = floor<ToMass>(d);
            const auto d1 = d0 + ToMass{1};
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
/* requires */  mass<Rep, Interval>::min() < mass<Rep, Interval>::zero()
        >>
        constexpr mass<Rep, Interval> abs(const mass<Rep, Interval>& d)
        {
            return d >= d.zero() ? d : -d;
        }

        template<class mass, typename = std::enable_if_t<
/* requires */ is_mass_v<mass>
        >>
        struct mass_suffix_impl
        {
            constexpr static std::string_view value {"g"};
        };

        template<class mass, typename = std::enable_if_t<
/* requires */ is_mass_v<mass>
        >>
        struct mass_suffix
        {
            using type = typename mass::interval::type;
            constexpr static auto value = mass_suffix_impl<mass>::value;
        };

        template<class T>
        constexpr auto mass_suffix_v = mass_suffix<T>::value;

        template<class CharT, class Traits, class Rep, class Interval>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const mass<Rep, Interval>& d)
        {
            std::basic_ostringstream<CharT, Traits> s;
            s.flags(os.flags());
            s.imbue(os.getloc());
            s.precision(os.precision());
            s << d.count() << measures::suffix_v<Interval> << measures::mass_suffix_v<mass<Rep, Interval>>;
            return os << s.str();
        }
    }
}