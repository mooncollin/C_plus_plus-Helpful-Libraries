#pragma once

#include <ratio>
#include <string_view>
#include <type_traits>
#include <stdexcept>
#include <iostream>

namespace collin
{
    namespace measures
    {
        template<intmax_t Pn>
        struct static_sign : std::integral_constant<intmax_t, (Pn < 0) ? -1 : 1> {};

        template<intmax_t Pn>
        struct static_abs : std::integral_constant<intmax_t, Pn* static_sign<Pn>::value> {};

        template<intmax_t Pn, intmax_t Qn>
        struct static_gcd : static_gcd<Qn, (Pn% Qn)> {};

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

        template<class Rep, class Ratio, class UnitValues, class System>
        class basic_unit;

        template<class T>
        struct is_basic_unit : std::false_type {};

        template<class Rep, class Ratio, class UnitValues, class System>
        struct is_basic_unit<basic_unit<Rep, Ratio, UnitValues, System>> : std::true_type {};

        template<class T>
        constexpr bool is_basic_unit_v = is_basic_unit<T>::value;
    }
}

namespace std
{
    template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
    struct common_type <collin::measures::basic_unit<Rep1, Ratio1, UnitValues, System>, collin::measures::basic_unit<Rep2, Ratio2, UnitValues, System>>
    {
        private:
            using num_ = collin::measures::static_gcd<Ratio1::num, Ratio2::num>;
            using den_ = collin::measures::static_gcd<Ratio1::den, Ratio2::den>;
            using cr_  = std::common_type_t<Rep1, Rep2>;
            using r_   = std::ratio<num_::value, (Ratio1::den / den_::value) * Ratio2::den>;
        public:
            using type = collin::measures::basic_unit<cr_, r_, UnitValues, System>;
    };
}

namespace collin
{
    namespace measures
    {
        template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, typename = std::enable_if_t<
/* requires */  is_basic_unit_v<ToBasicUnit> && std::is_same_v<typename ToBasicUnit::system, System>
        >>
        constexpr ToBasicUnit unit_cast(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
        {
            using ratio = typename std::ratio_divide<Ratio, typename ToBasicUnit::ratio>::type;
            using common_type = typename std::common_type_t<typename ToBasicUnit::rep, Rep>;

            return ToBasicUnit(
                static_cast<typename ToBasicUnit::rep>(
                    static_cast<common_type>(unit.count()) * static_cast<common_type>(ratio::num) / static_cast<common_type>(ratio::den)
                )
            );
        }

        template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, typename = std::enable_if_t<
/* requires */  is_basic_unit_v<ToBasicUnit>
        >>
        constexpr ToBasicUnit system_cast(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
        {
            if constexpr (std::is_same_v<typename ToBasicUnit::system, System>)
            {
                return ToBasicUnit(unit);
            }
            else
            {
                return UnitValues::system_cast<ToBasicUnit>(unit);
            }
        }

        template<class Rep, class Ratio, class UnitValues, class System>
        /*
         * Rep is the representation. Usually some large integer type.
         * Ratio is the ratio of the unit. This is generally compared to whatever the base unit is.
         * UnitValues is what distinguishes different base units from each other. Units such as distance,
         * and mass are not the same unit, so it shall not share the same UnitValue type, although it may
         * have similar zero(), min(), and max()
         * System is the measurement system that the unit belongs to. This allows units of different systems
         * to have to go through extra steps for narrowing.
         */
        class basic_unit
        {
            public:
                using rep = Rep;
                using ratio = Ratio;
                using unit_values = UnitValues;
                using system = System;

                static constexpr basic_unit zero() noexcept
                {
                    return basic_unit(UnitValues<Rep>::zero());
                }

                static constexpr basic_unit min() noexcept
                {
                    return basic_unit(UnitValues<Rep>::min());
                }

                static constexpr basic_unit max() noexcept
                {
                    return basic_unit(UnitValues<Rep>::max());
                }

                constexpr basic_unit() = default;
                constexpr basic_unit(const basic_unit&) = default;
                constexpr basic_unit(basic_unit&&) noexcept = default;

                basic_unit& operator=(const basic_unit&) = default;
                basic_unit& operator=(basic_unit&&) noexcept = default;

                ~basic_unit() noexcept = default;

                template<class Rep2, class Ratio2>
                constexpr basic_unit(const basic_unit<Rep2, Ratio2, UnitValues, System>& other)
                    : amount_(unit_cast<basic_unit>(other).count()) {}

                template<class Rep2>
                constexpr explicit basic_unit(const Rep2& n)
                    : amount_(static_cast<rep>(n)) {}

                [[nodiscard]] constexpr rep count() const noexcept
                {
                    return amount_;
                }

                [[nodiscard]] constexpr basic_unit operator+() const
                {
                    return basic_unit(*this);
                }

                [[nodiscard]] constexpr basic_unit operator-() const
                {
                    return basic_unit(-amount_);
                }

                constexpr basic_unit& operator++()
                {
                    ++amount_;
                    return *this;
                }

                constexpr basic_unit operator++(int)
                {
                    return basic_unit(amount_++);
                }

                constexpr basic_unit& operator--()
                {
                    --amount_;
                    return *this;
                }

                constexpr basic_unit& operator+=(const basic_unit& other)
                {
                    amount_ += other.count();
                    return *this;
                }

                constexpr basic_unit& operator-=(const basic_unit& other)
                {
                    amount_ -= other.count();
                    return *this;
                }

                constexpr basic_unit& operator*=(const rep& other)
                {
                    amount_ *= other;
                    return *this;
                }

                constexpr basic_unit& operator/=(const rep& other)
                {
                    amount_ /= other;
                    return *this;
                }

                constexpr basic_unit& operator %=(const rep& other)
                {
                    amount_ %= other;
                    return *this;
                }

                constexpr basic_unit& operator %=(const basic_unit& other)
                {
                    amount_ %= other.count();
                    return *this;
                }

            private:
                rep amount_ {0};
        };

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr
        typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                    basic_unit<Rep2, Ratio2, UnitValues, System>>
        operator+(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                            basic_unit<Rep2, Ratio2, UnitValues, System>>;
            
            return common_type(common_type(lhs).count() + common_type(rhs).count());
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr
        typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
        basic_unit<Rep2, Ratio2, UnitValues, System>>
        operator-(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                            basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type(common_type(lhs).count() - common_type(rhs).count());
        }

        template<class Rep1, class Ratio, class UnitValues, class System, class Rep2>
        constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator*(const basic_unit<Rep1, Ratio, UnitValues, System>& lhs, const Rep2& rhs)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return basic_unit<common_type, Ratio, UnitValues, System>(static_cast<common_type>(lhs.count()) * static_cast<common_type>(rhs));
        }

        template<class Rep1, class Ratio, class UnitValues, class System, class Rep2>
        constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator*(const Rep2& rhs, const basic_unit<Rep1, Ratio, UnitValues, System>& lhs)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return basic_unit<common_type, Ratio, UnitValues, System>(static_cast<common_type>(lhs.count()) * static_cast<common_type>(rhs));
        }

        template<class Rep1, class Ratio, class UnitValues, class System, class Rep2>
        constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator/(const basic_unit<Rep1, Ratio, UnitValues, System>& lhs, const Rep2& rhs)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return basic_unit<common_type, Ratio, UnitValues, System>(static_cast<common_type>(lhs.count()) / static_cast<common_type>(rhs));
        }

        template<class Rep1, class Ratio, class UnitValues, class System, class Rep2>
        constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator/(const Rep2& rhs, const basic_unit<Rep1, Ratio, UnitValues, System>& lhs)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return basic_unit<common_type, Ratio, UnitValues, System>(static_cast<common_type>(lhs.count()) / static_cast<common_type>(rhs));
        }

        template<class Rep1, class Ratio, class UnitValues, class System, class Rep2>
        constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator%(const basic_unit<Rep1, Ratio, UnitValues, System>& lhs, const Rep2& rhs)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return basic_unit<common_type, Ratio, UnitValues, System>(static_cast<common_type>(lhs.count()) % static_cast<common_type>(rhs));
        }

        template<class Rep1, class Ratio, class UnitValues, class System, class Rep2>
        constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator%(const Rep2& rhs, const basic_unit<Rep1, Ratio, UnitValues, System>& lhs)
        {
            using common_type = std::common_type_t<Rep1, Rep2>;

            return basic_unit<common_type, Ratio, UnitValues, System>(static_cast<common_type>(lhs.count()) % static_cast<common_type>(rhs));
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr
        typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
        basic_unit<Rep2, Ratio2, UnitValues, System>>
        operator%(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type(common_type(lhs).count() % common_type(rhs).count());
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr bool operator==(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            using common_type = std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>, basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type(lhs).count() == common_type(rhs).count();
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr bool operator!=(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return !(lhs == rhs);
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr bool operator<(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            using common_type = std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>, basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type(lhs).count() < common_type(rhs).count();
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr bool operator>(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return rhs < lsh;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr bool operator<=(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return !(rhs < lsh);
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        constexpr bool operator>=(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return !(lhs < rhs);
        }

        template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, typename= std::enable_if_t<
/* requires */  is_basic_unit_v<ToBasicUnit>
        >>
        constexpr ToBasicUnit floor(const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            const auto unit2 = unit_cast<ToBasicUnit>(unit);
            if (unit2 > unit)
            {
                return unit2 - ToBasicUnit{1};
            }

            return unit2;
        }

        template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, typename= std::enable_if_t<
/* requires */  is_basic_unit_v<ToBasicUnit>
        >>
        constexpr ToBasicUnit ceil(const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            const auto unit2 = unit_cast<ToBasicUnit>(unit);
            if (unit2 < unit)
            {
                return unit2 + ToBasicUnit{1};
            }

            return unit2;
        }

        template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, typename= std::enable_if_t<
/* requires */  is_basic_unit_v<ToBasicUnit>
        >>
        constexpr ToBasicUnit round(const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            const auto unit0 = floor<ToBasicUnit>(unit);
            const auto unit1 = unit0 + ToBasicUnit{1};
            const auto diff0 = unit - unit0;
            const auto diff1 = unit1 - unit;
            if (diff0 == diff1)
            {
                if (math::is_odd(unit0.count()))
                {
                    return unit1;
                }

                return unit0;
            }
            else if (diff0 < diff1)
            {
                return unit0;
            }

            return unit1;
        }

        template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, typename= std::enable_if_t<
/* requires */  is_basic_unit_v<ToBasicUnit>
        >>
        constexpr ToBasicUnit abs(const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            return unit >= unit.zero() ? unit : -unit;
        }

        struct metric_system
        {
            template<class T>
            struct suffix
            {
                constexpr static std::string_view value {""};
            };

            template<>
            struct suffix<std::atto>
            {
                constexpr static std::string_view value {"a"};
            };

            template<>
            struct suffix<std::femto>
            {
                constexpr static std::string_view value {"f"};
            };

            template<>
            struct suffix<std::pico>
            {
                constexpr static std::string_view value {"p"};
            };

            template<>
            struct suffix<std::nano>
            {
                constexpr static std::string_view value {"n"};
            };

            template<>
            struct suffix<std::micro>
            {
                constexpr static std::string_view value {"µ"};
            };

            template<>
            struct suffix<std::milli>
            {
                constexpr static std::string_view value {"m"};
            };

            template<>
            struct suffix<std::centi>
            {
                constexpr static std::string_view value {"c"};
            };

            template<>
            struct suffix<std::deci>
            {
                constexpr static std::string_view value {"d"};
            };

            template<>
            struct suffix<std::deca>
            {
                constexpr static std::string_view value {"da"};
            };

            template<>
            struct suffix<std::hecto>
            {
                constexpr static std::string_view value {"h"};
            };

            template<>
            struct suffix<std::kilo>
            {
                constexpr static std::string_view value {"k"};
            };

            template<>
            struct suffix<std::mega>
            {
                constexpr static std::string_view value {"M"};
            };

            template<>
            struct suffix<std::giga>
            {
                constexpr static std::string_view value {"G"};
            };

            template<>
            struct suffix<std::tera>
            {
                constexpr static std::string_view value {"T"};
            };

            template<>
            struct suffix<std::peta>
            {
                constexpr static std::string_view value {"P"};
            };

            template<>
            struct suffix<std::exa>
            {
                constexpr static std::string_view value {"E"};
            };

            template<class T>
            static constexpr auto suffix_v = suffix<T>::value;
        };

        struct imperial_system
        {
            template<class T>
            struct suffix
            {
                constexpr static std::string_view value {""};
            };

            template<class T>
            static constexpr auto suffix_v = suffix<T>::value;
        };

        template<class CharT, class Traits, class Rep, class Ratio, class UnitValues, class System>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            return os << unit.count() << System::suffix_v<basic_unit<Rep, Ratio, UnitValues, System>>;
        }
    }
}
