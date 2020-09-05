#pragma once

#include <ratio>
#include <string_view>
#include <type_traits>
#include <stdexcept>
#include <iostream>

#include "collin/math.hpp"
#include "collin/type_traits.hpp"
#include "collin/concepts.hpp"
#include "collin/tuple.hpp"

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

        template<class T>
        concept ratio_type = is_ratio_v<T>;

        template<ratio_type...>
        struct ratio_add_many;

        template<ratio_type R1>
        struct ratio_add_many<R1>
        {
            using type = R1;
        };

        template<ratio_type R1, ratio_type R2>
        struct ratio_add_many<R1, R2>
        {
            using type = std::ratio_add<R1, R2>;
        };

        template<ratio_type R1, ratio_type R2, ratio_type... RR>
        struct ratio_add_many<R1, R2, RR...>
        {
            using type = std::ratio_add<R1, typename ratio_add_many<R2, RR...>::type>;
        };

        template<ratio_type... RR>
        using ratio_add_many_t = typename ratio_add_many<RR...>::type;

        template<ratio_type...>
        struct ratio_subtract_many;

        template<ratio_type R1>
        struct ratio_subtract_many<R1>
        {
            using type = R1;
        };

        template<ratio_type R1, ratio_type R2>
        struct ratio_subtract_many<R1, R2>
        {
            using type = std::ratio_subtract<R1, R2>;
        };

        template<ratio_type R1, ratio_type R2, ratio_type... RR>
        struct ratio_subtract_many<R1, R2, RR...>
        {
            using type = std::ratio_subtract<R1, typename ratio_subtract_many<R2, RR...>::type>;
        };

        template<ratio_type... RR>
        using ratio_subtract_many_t = typename ratio_subtract_many<RR...>::type;

        template<ratio_type...>
        struct ratio_multiply_many;

        template<ratio_type R1>
        struct ratio_multiply_many<R1>
        {
            using type = R1;
        };

        template<ratio_type R1, ratio_type R2>
        struct ratio_multiply_many<R1, R2>
        {
            using type = std::ratio_multiply<R1, R2>;
        };

        template<ratio_type R1, ratio_type R2, ratio_type... RR>
        struct ratio_multiply_many<R1, R2, RR...>
        {
            using type = std::ratio_multiply<R1, typename ratio_multiply_many<R2, RR...>::type>;
        };

        template<ratio_type... RR>
        using ratio_multiply_many_t = typename ratio_multiply_many<RR...>::type;

        template<ratio_type...>
        struct ratio_divide_many;

        template<ratio_type R1>
        struct ratio_divide_many<R1>
        {
            using type = R1;
        };

        template<ratio_type R1, ratio_type R2>
        struct ratio_divide_many<R1, R2>
        {
            using type = std::ratio_divide<R1, R2>;
        };

        template<ratio_type R1, ratio_type R2, ratio_type... RR>
        struct ratio_divide_many<R1, R2, RR...>
        {
            using type = std::ratio_divide<R1, typename ratio_divide_many<R2, RR...>::type>;
        };

        template<ratio_type... RR>
        using ratio_divide_many_t = typename ratio_divide_many<RR...>::type;

        template<collin::concepts::arithmetic Rep, ratio_type Ratio, class UnitValues, class System>
        class basic_unit;

        template<class T>
        struct is_basic_unit : std::false_type {};

        template<collin::concepts::arithmetic Rep, ratio_type Ratio, class UnitValues, class System>
        struct is_basic_unit<basic_unit<Rep, Ratio, UnitValues, System>> : std::true_type {};

        template<class T>
        constexpr bool is_basic_unit_v = is_basic_unit<T>::value;

        template<class T>
        concept basic_unit_type = is_basic_unit_v<T>;

        constexpr std::intmax_t dynamic_dimension = std::numeric_limits<std::intmax_t>::max();

        template<std::intmax_t Ext>
        class dimension_type
        {
            public:
                using size_type = std::intmax_t;
                static constexpr size_type value = Ext;

                constexpr dimension_type() noexcept = default;

                template<size_type Other>
                constexpr dimension_type(dimension_type<Other> ext) noexcept
                {
                    static_assert(Other == Ext);
                }

                constexpr dimension_type(size_type) noexcept {}

                constexpr size_type size() const noexcept
                {
                    return Ext;
                }
        };

        template<>
        class dimension_type<dynamic_dimension>
        {
            public:
                using size_type = std::intmax_t;
                static constexpr size_type value = dynamic_dimension;

                template<size_type Other>
                explicit constexpr dimension_type(dimension_type<Other> ext) noexcept
                    : size_(ext.size()) {}

                explicit constexpr dimension_type(size_type size) noexcept
                    : size_(size) {}

                constexpr size_type size() const noexcept
                {
                    return size_;
                }

            private:
                size_type size_;
        };

        template<collin::concepts::arithmetic Rep, ratio_type Ratio, class UnitValues, class System, std::intmax_t Dimension = dynamic_dimension>
            requires (Dimension != 0)
        class basic_dimension_unit;

        template<class T>
        struct is_basic_dimension_unit : std::false_type {};

        template<collin::concepts::arithmetic Rep, ratio_type Ratio, class UnitValues, class System, std::intmax_t Dimension>
        struct is_basic_dimension_unit<basic_dimension_unit<Rep, Ratio, UnitValues, System, Dimension>> : std::true_type {};

        template<class T>
        constexpr bool is_basic_dimension_unit_v = is_basic_dimension_unit<T>::value;

        template<class T>
        concept basic_dimension_unit_type = is_basic_dimension_unit_v<T>;

        template<collin::concepts::arithmetic Rep, basic_dimension_unit_type... DimensionUnits>
            requires (collin::type_traits::is_unique_v<typename DimensionUnits::unit_values...> &&
                        (... && !collin::concepts::same<typename DimensionUnits::dimension_t, dimension_type<dynamic_dimension>>) &&
                        sizeof...(DimensionUnits) >= 2
                     )
        class basic_derived_unit;

        template<class T>
        struct is_basic_derived_unit : std::false_type {};

        template<class... DimensionUnits>
        struct is_basic_derived_unit<basic_derived_unit<DimensionUnits...>> : std::true_type {};

        template<class T>
        constexpr bool is_basic_derived_unit_v = is_basic_derived_unit<T>::value;

        template<class T>
        concept basic_derived_unit_type = is_basic_derived_unit_v<T>;
    }
}

namespace std
{
    template<class Rep1, collin::measures::ratio_type Ratio1, class Rep2, collin::measures::ratio_type Ratio2, class UnitValues, class System>
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
        template<basic_unit_type ToBasicUnit, class Rep, ratio_type Ratio, class UnitValues, class System>
            requires (collin::concepts::same<typename ToBasicUnit::system, System>)
        [[nodiscard]] constexpr ToBasicUnit unit_cast(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
        {
            using ratio = typename std::ratio_divide<Ratio, typename ToBasicUnit::ratio>::type;
            using common_type = typename std::common_type_t<typename ToBasicUnit::rep, Rep>;

            return ToBasicUnit(
                static_cast<typename ToBasicUnit::rep>(
                    static_cast<common_type>(unit.count()) * collin::math::rational_ratio<ratio, common_type>
                )
            );
        }

        template<basic_unit_type ToBasicUnit, class Rep, ratio_type Ratio, class UnitValues, class System>
            requires(collin::concepts::same<typename ToBasicUnit::system, System> ||
                    requires(const basic_unit<Rep, Ratio, UnitValues, System>& unit) { UnitValues::system_cast<ToBasicUnit>(unit); })
        [[nodiscard]] constexpr ToBasicUnit system_cast(const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            if constexpr (std::is_same_v<typename ToBasicUnit::system, System>)
            {
                return unit_cast<ToBasicUnit>(unit);
            }
            else
            {
                return UnitValues::system_cast<ToBasicUnit>(unit);
            }
        }

        template<collin::concepts::arithmetic Rep, ratio_type Ratio, class UnitValues, class System>
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
                    return basic_unit{UnitValues<Rep>::zero()};
                }

                static constexpr basic_unit min() noexcept
                {
                    return basic_unit{UnitValues<Rep>::min()};
                }

                static constexpr basic_unit max() noexcept
                {
                    return basic_unit{UnitValues<Rep>::max()};
                }

                constexpr basic_unit() = default;
                constexpr basic_unit(const basic_unit&) = default;
                constexpr basic_unit(basic_unit&&) noexcept = default;

                constexpr basic_unit& operator=(const basic_unit&) = default;
                constexpr basic_unit& operator=(basic_unit&&) noexcept = default;

                template<class Rep2, ratio_type Ratio2>
                    requires(collin::concepts::constructible<rep, Rep2>)
                constexpr basic_unit(const basic_unit<Rep2, Ratio2, UnitValues, System>& other)
                    : amount_{unit_cast<basic_unit>(other).count()} {}

                template<class Rep2>
                constexpr explicit basic_unit(const Rep2& n)
                    : amount_{n} {}

                template<class Rep2, ratio_type Ratio2>
                constexpr basic_unit& operator=(const basic_unit<Rep2, Ratio2, UnitValues, System>& other)
                {
                    amount_ = static_cast<rep>(unit_cast<basic_unit>(other).count());
                    return *this;
                }

                [[nodiscard]] constexpr rep count() const noexcept
                {
                    return amount_;
                }

                [[nodiscard]] constexpr basic_unit operator+() const noexcept
                {
                    return basic_unit{*this};
                }

                [[nodiscard]] constexpr basic_unit operator-() const noexcept
                {
                    return basic_unit{-amount_};
                }

                constexpr basic_unit& operator++() noexcept
                {
                    ++amount_;
                    return *this;
                }

                constexpr basic_unit operator++(int) noexcept
                {
                    return basic_unit{amount_++};
                }

                constexpr basic_unit& operator--() noexcept
                {
                    --amount_;
                    return *this;
                }

                constexpr basic_unit& operator--(int) noexcept
                {
                    return basic_unit{amount_--};
                }

                template<class Rep2, ratio_type Ratio2>
                    requires(requires(rep r, const Rep2& r2) { r += r2; })
                constexpr basic_unit& operator+=(const basic_unit<Rep2, Ratio2, UnitValues, System>& other) noexcept
                {
                    amount_ += unit_cast<basic_unit>(other).count();
                    return *this;
                }

                template<class Rep2, ratio_type Ratio2>
                    requires(requires(rep r, const Rep2& r2) { r -= r2; })
                constexpr basic_unit& operator-=(const basic_unit<Rep2, Ratio2, UnitValues, System>& other) noexcept
                {
                    amount_ -= unit_cast<basic_unit>(other).count();
                    return *this;
                }

                template<class Rep2>
                    requires(requires(rep r, const Rep2& r2) { r *= r2; })
                constexpr basic_unit& operator*=(const Rep2& other) noexcept
                {
                    amount_ *= other;
                    return *this;
                }

                template<class Rep2>
                    requires(requires(rep r, const Rep2& r2) { r /= r2; })
                constexpr basic_unit& operator/=(const Rep2& other) noexcept
                {
                    amount_ /= other;
                    return *this;
                }

                template<class Rep2>
                    requires(requires(rep r, const Rep2& r2) { r %= r2; })
                constexpr basic_unit& operator %=(const Rep2& other) noexcept
                {
                    amount_ %= other;
                    return *this;
                }

                template<class Rep2, ratio_type Ratio2>
                    requires(requires(rep r, const Rep2& r2) { r %= r2; })
                constexpr basic_unit& operator %=(const basic_unit<Rep2, Ratio2, UnitValues, System>& other) noexcept
                {
                    amount_ %= unit_cast<basic_unit>(other).count();
                    return *this;
                }

            private:
                rep amount_ {};
        };

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr
        typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                    basic_unit<Rep2, Ratio2, UnitValues, System>>
        operator+(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                            basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type{common_type{lhs}.count() + common_type{rhs}.count()};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr
        typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                    basic_unit<Rep2, Ratio2, UnitValues, System>>
        operator-(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                            basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type{common_type{lhs}.count() - common_type{rhs}.count()};
        }

        template<class Rep1, ratio_type Ratio, class UnitValues, class System, class Rep2>
        [[nodiscard]] constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator*(const basic_unit<Rep1, Ratio, UnitValues, System>& lhs, const Rep2& rhs) noexcept
        {
            using common_type = typename std::common_type_t<Rep1, Rep2>;
            using basic_unit_common_type = basic_unit<common_type, Ratio, UnitValues, System>;

            return basic_unit_common_type{basic_unit_common_type{lhs}.count() * rhs};
        }

        template<class Rep1, ratio_type Ratio, class UnitValues, class System, class Rep2>
        [[nodiscard]] constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator*(const Rep2& lhs, const basic_unit<Rep1, Ratio, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<Rep1, Rep2>;
            using basic_unit_common_type = basic_unit<common_type, Ratio, UnitValues, System>;

            return basic_unit_common_type{lhs * basic_unit_common_type{rhs}.count()};
        }

        template<class Rep1, ratio_type Ratio, class UnitValues, class System, class Rep2>
        [[nodiscard]] constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator/(const basic_unit<Rep1, Ratio, UnitValues, System>& lhs, const Rep2& rhs) noexcept
        {
            using common_type = typename std::common_type_t<Rep1, Rep2>;
            using basic_unit_common_type = basic_unit<common_type, Ratio, UnitValues, System>;

            return basic_unit_common_type{basic_unit_common_type{lhs}.count() / rhs};
        }

        template<class Rep1, ratio_type Ratio, class UnitValues, class System, class Rep2>
        [[nodiscard]] constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator%(const basic_unit<Rep1, Ratio, UnitValues, System>& lhs, const Rep2& rhs) noexcept
        {
            using common_type = typename std::common_type_t<Rep1, Rep2>;
            using basic_unit_common_type = basic_unit<common_type, Ratio, UnitValues, System>;

            return basic_unit_common_type{basic_unit_common_type{lhs}.count() % rhs};
        }

        template<class Rep1, ratio_type Ratio, class UnitValues, class System, class Rep2>
        [[nodiscard]] constexpr
        basic_unit<typename std::common_type_t<Rep1, Rep2>, Ratio, UnitValues, System>
        operator%(const Rep2& lhs, const basic_unit<Rep1, Ratio, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<Rep1, Rep2>;
            using basic_unit_common_type = basic_unit<common_type, Ratio, UnitValues, System>;

            return basic_unit_common_type{lhs % basic_unit_common_type{rhs}.count()};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, class Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr
        typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                    basic_unit<Rep2, Ratio2, UnitValues, System>>
        operator%(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                            basic_unit<Rep2, Ratio2, UnitValues, System>>;

            return common_type{common_type{lhs}.count() % common_type{rhs}.count()};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr bool operator==(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>, basic_unit<Rep2, Ratio2, UnitValues, System>>;
            return common_type{lhs}.count() == common_type{rhs}.count();
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr bool operator!=(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr bool operator<(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            using common_type = typename std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>, basic_unit<Rep2, Ratio2, UnitValues, System>>;
            return common_type{lhs}.count() < common_type{rhs}.count();
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr bool operator>(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            return rhs < lhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr bool operator<=(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            return !(rhs < lhs);
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System>
        [[nodiscard]] constexpr bool operator>=(const basic_unit<Rep1, Ratio1, UnitValues, System>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            return !(lhs < rhs);
        }

        template<basic_unit_type ToBasicUnit, class Rep, ratio_type Ratio, class UnitValues, class System>
        [[nodiscard]] constexpr ToBasicUnit floor(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
        {
            const auto unit2 = unit_cast<ToBasicUnit>(unit);
            if (unit2 > unit)
            {
                return unit2 - ToBasicUnit{1};
            }

            return unit2;
        }

        template<basic_unit_type ToBasicUnit, class Rep, ratio_type Ratio, class UnitValues, class System>
        [[nodiscard]] constexpr ToBasicUnit ceil(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
        {
            const auto unit2 = unit_cast<ToBasicUnit>(unit);
            if (unit2 < unit)
            {
                return unit2 + ToBasicUnit{1};
            }

            return unit2;
        }

        template<basic_unit_type ToBasicUnit, class Rep, ratio_type Ratio, class UnitValues, class System>
        [[nodiscard]] constexpr ToBasicUnit round(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
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

        template<basic_unit_type ToBasicUnit, class Rep, ratio_type Ratio, class UnitValues, class System>
        [[nodiscard]] constexpr ToBasicUnit abs(const basic_unit<Rep, Ratio, UnitValues, System>& unit) noexcept
        {
            return unit >= basic_unit<Rep, Ratio, UnitValues, System>::zero() ? unit : -unit;
        }

        template<collin::concepts::arithmetic Rep, ratio_type Ratio, class UnitValues, class System, std::intmax_t Dimension>
            requires (Dimension != 0)
        class basic_dimension_unit
        {
            public:
                using rep = Rep;
                using ratio = Ratio;
                using unit_values = UnitValues;
                using system = System;
                using dimension_t = dimension_type<Dimension>;
                using unit_t = basic_unit<rep, ratio, unit_values, system>;

                static constexpr basic_dimension_unit zero() noexcept
                {
                    return basic_dimension_unit{UnitValues<Rep>::zero()};
                }

                static constexpr basic_dimension_unit min() noexcept
                {
                    return basic_dimension_unit{UnitValues<Rep>::min()};
                }

                static constexpr basic_dimension_unit max() noexcept
                {
                    return basic_dimension_unit{UnitValues<Rep>::max()};
                }

                constexpr basic_dimension_unit()
                    : storage_{0, 1} {}

                constexpr basic_dimension_unit(const basic_dimension_unit&) = default;
                constexpr basic_dimension_unit(basic_dimension_unit&&) noexcept = default;

                constexpr basic_dimension_unit& operator=(const basic_dimension_unit&) = default;
                constexpr basic_dimension_unit& operator=(basic_dimension_unit&&) noexcept = default;

                template<class Rep2, class Ratio2>
                constexpr basic_dimension_unit(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension>& other)
                    : storage_{other.unit(), other.dimension()} {}

                template<class Rep2, class Ratio2, std::size_t Dimension2>
                constexpr basic_dimension_unit(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension2>& other, typename dimension_t::size_type d = 1)
                    : storage_{0, d}
                {
                    storage_.unit() = convert_dimension(other);
                }

                template<class Rep2, class Ratio2>
                constexpr basic_dimension_unit& operator=(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension>& other)
                {
                    storage_.unit() = other.unit();
                    return *this;
                }

                template<class Rep2, class Ratio2, std::size_t Dimension2>
                constexpr basic_dimension_unit& operator=(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension2>& other)
                {
                    storage_.unit() = convert_dimension(other);
                    return *this;
                }

                template<class Rep2>
                constexpr explicit basic_dimension_unit(const Rep2& n, typename dimension_t::size_type d = 1)
                    : storage_{n, d} {}

                [[nodiscard]] constexpr typename dimension_t::size_type dimension() const noexcept
                {
                    return storage_.size();
                }

                constexpr unit_t& unit() noexcept
                {
                    return storage_.unit();
                }

                constexpr const unit_t& unit() const noexcept
                {
                    return storage_.unit();
                }

                [[nodiscard]] constexpr rep count() const noexcept
                {
                    return unit().count();
                }

                [[nodiscard]] constexpr basic_dimension_unit operator+() const noexcept
                {
                    return basic_dimension_unit{unit()};
                }

                [[nodiscard]] constexpr basic_dimension_unit operator-() const noexcept
                {
                    return basic_dimension_unit{-unit()};
                }

                constexpr basic_dimension_unit& operator++() noexcept
                {
                    ++unit();
                    return *this;
                }

                constexpr basic_dimension_unit operator++(int) noexcept
                {
                    return basic_dimension_unit{unit()++};
                }

                constexpr basic_dimension_unit& operator--() noexcept
                {
                    --unit();
                    return *this;
                }

                constexpr basic_dimension_unit operator--(int) noexcept
                {
                    return basic_dimension_unit{unit()--};
                }

                template<class Rep2, ratio_type Ratio2, std::size_t Dimension2>
                    requires(Dimension2 == Dimension && requires(unit_t t, const basic_unit<Rep2, Ratio2, unit_values, system>& t2){ t += t2; })
                constexpr basic_dimension_unit& operator+=(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension2>& other) noexcept
                {
                    unit() += basic_dimension_unit{correct_type}.unit();
                    return *this;
                }

                template<class Rep2, ratio_type Ratio2, std::size_t Dimension2>
                    requires(requires(unit_t t, const basic_unit<Rep2, Ratio2, unit_values, system>& t2){ t -= t2; })
                constexpr basic_dimension_unit& operator-=(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension2>& other) noexcept
                {
                    unit() -= basic_dimension_unit{other}.unit();
                    return *this;
                }

                template<class Rep2>
                    requires(requires(unit_t t, const Rep2& r) { t *= r; })
                constexpr basic_dimension_unit& operator*=(const Rep2& other) noexcept
                {
                    unit() *= other;
                    return *this;
                }

                template<class Rep2>
                    requires(requires(unit_t t, const Rep2& r) { t /= r; })
                constexpr basic_dimension_unit& operator/=(const Rep2& other) noexcept
                {
                    unit() /= other;
                    return *this;
                }

                template<class Rep2>
                    requires(requires(unit_t t, const Rep2& r) { t %= r; })
                constexpr basic_dimension_unit& operator%=(const Rep2& other) noexcept
                {
                    unit() %= other;
                    return *this;
                }

                template<class Rep2, ratio_type Ratio2, std::size_t Dimension2>
                    requires(requires(unit_t t, const basic_unit<Rep2, Ratio2, unit_values, system>& t2){ t %= t2; })
                constexpr basic_dimension_unit& operator%=(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension2>& other) noexcept
                {
                    unit() %= basic_dimension_unit{other}.unit();
                    return *this;
                }
            private:
                template<class ExtentType>
                class storage_type : public ExtentType
                {
                    public:
                        template<class Data, class OtherExtentType>
                            requires(collin::concepts::constructible<unit_t, Data> && collin::concepts::constructible<ExtentType, OtherExtentType>)
                        constexpr storage_type(Data data, OtherExtentType ext) noexcept(std::is_nothrow_constructible_v<unit_t, Data>)
                            : ExtentType{ext}, unit_{data} {}

                        constexpr unit_t& unit() noexcept
                        {
                            return unit_;
                        }

                        constexpr const unit_t& unit() const noexcept
                        {
                            return unit_;
                        }
                    private:
                        unit_t unit_;
                };

                storage_type<dimension_t> storage_;

                template<class Rep2, class Ratio2, std::size_t Dimension2>
                [[nodiscard]] constexpr auto convert_dimension(const basic_dimension_unit<Rep2, Ratio2, unit_values, system, Dimension2>& other) noexcept(std::is_nothrow_copy_constructible_v<unit_t>)
                {
                    using common_type = typename std::common_type_t<basic_unit<rep, ratio, unit_values, system>,
                                                                    basic_unit<Rep2, Ratio2, unit_values, system>>;

                    const auto correct_type = common_type{other.unit()};
                    if (correct_type.count() != 0)
                    {
                        // TODO: Make sure this logic actually makes sense
                        if (dimension() != other.dimension())
                        {
                            const auto difference = collin::math::abs(other.dimension() - dimension());
                            return common_type{static_cast<typename common_type::rep>(collin::math::pow(correct_type.count(), difference + 1))};
                        }
                    }

                    return correct_type;
                }
        };

        template<basic_unit_type T, std::intmax_t Dimension>
        using basic_dimension_unit_t = basic_dimension_unit<typename T::rep, typename T::ratio, typename T::unit_values, typename T::system, Dimension>;

        template<basic_dimension_unit_type DimensionUnit>
                requires(!collin::concepts::same<typename DimensionUnit::dimension_t, dimension_type<dynamic_dimension>>)
        struct is_numerator_unit : std::bool_constant<(DimensionUnit::dimension_t::value > 0)> {};

        template<basic_dimension_unit_type DimensionUnit>
        constexpr auto is_numerator_unit_v = is_numerator_unit<DimensionUnit>::value;

        template<basic_dimension_unit_type DimensionUnit>
                requires(!collin::concepts::same<typename DimensionUnit::dimension_t, dimension_type<dynamic_dimension>>)
        struct is_denominator_unit : std::bool_constant<(DimensionUnit::dimension_t::value < 0)> {};

        template<basic_dimension_unit_type DimensionUnit>
        constexpr auto is_denominator_unit_v = is_numerator_unit<DimensionUnit>::value;

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr bool operator==(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            if (lhs.dimension() > rhs.dimension())
            {
                return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>{lhs, rhs.dimension()}.unit() == rhs.unit();
            }
            return lhs.unit() == basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>{rhs, lhs.dimension()}.unit();
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr bool operator!=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr bool operator<(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            if (lhs.dimension() > rhs.dimension())
            {
                return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>{lhs, rhs.dimension()}.unit() < rhs.unit();
            }
            return lhs.unit() < basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>{rhs, lhs.dimension()}.unit();
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr bool operator>(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            return rhs < lhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr bool operator<=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            return !(rhs < lhs);
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr bool operator>=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            return !(lhs < rhs);
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator==(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            return lhs == basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator==(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs) noexcept
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} == rhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator!=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs) noexcept
        {
            return lhs != basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator!=(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs) noexcept
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} != rhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator<(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs < basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator<(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} < rhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator>(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs > basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator>(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} > rhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator<=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs <= basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator<=(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} <= rhs;
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator>=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs >= basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, ratio_type Ratio1, class Rep2, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr bool operator>=(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} >= rhs;
        }

        template<class Rep1, class Rep2, ratio_type Ratio1, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr auto operator+(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            auto r {lhs};
            r += rhs;
            return r;
        }

        template<class Rep1, class Rep2, ratio_type Ratio1, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr auto operator-(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            auto r {lhs};
            r -= rhs;
            return r;
        }

        template<class Rep1, class Rep2, ratio_type Ratio1, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr auto operator*(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const Rep2& rhs) noexcept
        {
            auto r {lhs};
            r *= rhs;
            return r;
        }

        template<class Rep1, class Rep2, ratio_type Ratio1, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr auto operator/(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const Rep2& rhs) noexcept
        {
            auto r {lhs};
            r /= rhs;
            return r;
        }

        template<class Rep1, class Rep2, ratio_type Ratio1, class UnitValues, class System, std::intmax_t Dimension>
        [[nodiscard]] constexpr auto operator%(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const Rep2& rhs) noexcept
        {
            auto r {lhs};
            r %= rhs;
            return r;
        }

        template<class Rep1, class Rep2, ratio_type Ratio1, ratio_type Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        [[nodiscard]] constexpr auto operator%(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
        {
            auto r {lhs};
            r %= rhs;
            return r;
        }

        template<collin::concepts::arithmetic Rep, basic_dimension_unit_type... DimensionUnits>
            requires (collin::type_traits::is_unique_v<typename DimensionUnits::unit_values...> &&
                        (... && !collin::concepts::same<typename DimensionUnits::dimension_t, dimension_type<dynamic_dimension>>) &&
                        sizeof...(DimensionUnits) >= 2
                     )
        class basic_derived_unit
        {
            using storage_t = collin::math::basic_rational<Rep>;

            template<basic_dimension_unit_type... DimensionUnits2>
                using result_ratio = ratio_multiply_many_t<
                                        std::conditional_t<is_numerator_unit_v<DimensionUnits>,
                                            std::ratio_divide<
                                                typename DimensionUnits2::ratio,
                                                typename DimensionUnits::ratio
                                                >,
                                            std::ratio_divide<
                                                std::ratio<1, 1>,
                                                std::ratio_divide<
                                                    typename DimensionUnits2::ratio,
                                                    typename DimensionUnits::ratio
                                                >
                                            >
                                        >...
                                    >;
            public:
                using rep = Rep;
                using units = std::tuple<DimensionUnits...>;
                using numerator_units = typename collin::tuples::filter_t<is_numerator_unit, units>;
                using denominator_units = typename collin::tuples::filter_t<is_denominator_unit, units>;
                static constexpr auto num_units = sizeof...(DimensionUnits);
                
                static constexpr basic_derived_unit zero() noexcept
                {
                    return basic_derived_unit{std::tuple_element<0, units>::unit_values<rep>::zero()};
                }

                static constexpr basic_derived_unit min() noexcept
                {
                    return basic_derived_unit{std::tuple_element<0, units>::unit_values<rep>::min()};
                }

                static constexpr basic_derived_unit max() noexcept
                {
                    return basic_derived_unit{std::tuple_element<0, units>::unit_values<rep>::max()};
                }

                constexpr basic_derived_unit() = default;
                constexpr basic_derived_unit(const basic_derived_unit&) = default;
                constexpr basic_derived_unit(basic_derived_unit&&) noexcept = default;

                constexpr basic_derived_unit& operator=(const basic_derived_unit&) = default;
                constexpr basic_derived_unit& operator=(basic_derived_unit&&) noexcept = default;

                template<class Rep2>
                    requires(collin::concepts::constructible<rep, Rep2>)
                constexpr explicit basic_derived_unit(const Rep2& n, const Rep2& d = 1)
                    : amount_{n, d} {}

                template<class Rep2>
                constexpr explicit basic_derived_unit(const collin::math::basic_rational<Rep2>& other)
                    : amount_{other} {}

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                constexpr basic_derived_unit(const basic_derived_unit<Rep2, DimensionUnits2...>& other)
                    : basic_derived_unit{other.rational() * collin::math::rational_ratio<result_ratio<DimensionUnits2...>, Rep2>} {}

                [[nodiscard]] constexpr storage_t rational() const noexcept
                {
                    return amount_;
                }

                [[nodiscard]] constexpr rep count() const noexcept
                {
                    return static_cast<rep>(amount_);
                }

                [[nodiscard]] constexpr basic_derived_unit operator+() const noexcept
                {
                    return *this;
                }

                [[nodiscard]] constexpr basic_derived_unit operator-() const noexcept
                {
                    return basic_derived_unit{-amount_};
                }

                constexpr basic_derived_unit& operator++() noexcept
                {
                    ++amount_;
                    return *this;
                }

                constexpr basic_derived_unit operator++(int) noexcept
                {
                    return basic_derived_unit{amount_++};
                }

                constexpr basic_derived_unit operator--() noexcept
                {
                    --amount_;
                    return *this;
                }

                constexpr basic_derived_unit operator--(int) noexcept
                {
                    return basic_derived_unit{amount_--};
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires(... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                     collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                            )
                constexpr basic_derived_unit& operator+=(const basic_derived_unit<Rep2, DimensionUnits2...>& other) noexcept
                {
                    using common_type = std::common_type_t<Rep, Rep2>;
                    using t = result_ratio<DimensionUnits2...>;

                    if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
                    {
                        amount_ += other.rational();
                    }
                    else
                    {
                        amount_ += other.rational() * collin::math::rational_ratio<t, common_type>;
                    }

                    return *this;
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires(... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                     collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                            )
                constexpr basic_derived_unit& operator-=(const basic_derived_unit<Rep2, DimensionUnits2...>& other) noexcept
                {
                    using common_type = std::common_type_t<Rep, Rep2>;
                    using t = result_ratio<DimensionUnits2...>;

                    if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
                    {
                        amount_ -= other.rational();
                    }
                    else
                    {
                        amount_ -= other.rational() * collin::math::rational_ratio<t, common_type>;
                    }

                    return *this;
                }

                template<class Rep2>
                constexpr basic_derived_unit& operator*=(const Rep2& other) noexcept
                {
                    amount_ *= other;
                    return *this;
                }

                template<class Rep2>
                constexpr basic_derived_unit& operator/=(const Rep2& other) noexcept
                {
                    amount_ /= other;
                    return *this;
                }

                //template<class Rep2>
                //constexpr basic_derived_unit& operator%=(const Rep2& other) noexcept
                //{
                //    amount_ %= other;
                //    return *this;
                //}

                //template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                //    requires(... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                //                     collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                //            )
                //constexpr basic_derived_unit& operator%=(const basic_derived_unit<Rep2, DimensionUnits2...>& other) noexcept
                //{
                //    using common_type = std::common_type_t<Rep, Rep2>;
                //    using t = result_ratio<DimensionUnits2...>;

                //    if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
                //    {
                //        amount_ %= other.rational();
                //    }
                //    else
                //    {
                //        amount_ %= other.rational() * collin::math::rational_ratio<t, common_type>;
                //    }

                //    return *this;
                //}

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires((sizeof...(DimensionUnits) == sizeof...(DimensionUnits2))
                                && (... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                             collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                                    )
                            )
                [[nodiscard]] friend constexpr bool operator==(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
                {
                    using t = result_ratio<DimensionUnits2...>;

                    if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
                    {
                        return lhs.rational() == rhs.rational();
                    }
                    else
                    {
                        return lhs.rational() == basic_derived_unit<Rep, DimensionUnits...>{rhs}.rational();
                    }
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires((sizeof...(DimensionUnits) == sizeof...(DimensionUnits2))
                                && (... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                             collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                                    )
                            )
                [[nodiscard]] friend constexpr bool operator!=(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
                {
                    return !(lhs == rhs);
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires((sizeof...(DimensionUnits) == sizeof...(DimensionUnits2))
                                && (... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                             collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                                    )
                            )
                [[nodiscard]] friend constexpr bool operator<(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
                {
                    using t = result_ratio<DimensionUnits2...>;

                    if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
                    {
                        return lhs.rational() < rhs.rational();
                    }
                    else
                    {
                        return lhs.rational() < basic_derived_unit<Rep, DimensionUnits...>{rhs}.rational();
                    }
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires((sizeof...(DimensionUnits) == sizeof...(DimensionUnits2))
                                && (... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                             collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                                    )
                            )
                [[nodiscard]] friend constexpr bool operator>(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
                {
                    return rhs < lhs;
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires((sizeof...(DimensionUnits) == sizeof...(DimensionUnits2))
                                && (... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                             collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                                    )
                            )
                [[nodiscard]] friend constexpr bool operator<=(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
                {
                    return !(rhs < lhs);
                }

                template<class Rep2, basic_dimension_unit_type... DimensionUnits2>
                    requires((sizeof...(DimensionUnits) == sizeof...(DimensionUnits2))
                                && (... && (collin::concepts::same<typename DimensionUnits::unit_values, typename DimensionUnits2::unit_values> &&
                                             collin::concepts::same<typename DimensionUnits::system, typename DimensionUnits2::system>)
                                    )
                            )
                [[nodiscard]] friend constexpr bool operator>=(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
                {
                    return !(lhs < rhs);
                }
            private:
                storage_t amount_ {0, 1};
        };

        template<class Rep1, basic_dimension_unit_type... DimensionUnits1, class Rep2, basic_dimension_unit_type... DimensionUnits2>
            requires((sizeof...(DimensionUnits1) == sizeof...(DimensionUnits2))
                        && (... && (collin::concepts::same<typename DimensionUnits1::unit_values, typename DimensionUnits2::unit_values> &&
                                     collin::concepts::same<typename DimensionUnits1::system, typename DimensionUnits2::system>)
                            )
                    )
        [[nodiscard]] constexpr auto operator+(const basic_derived_unit<Rep1, DimensionUnits1...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
        {
            auto r {lhs};
            r += rhs;
            return r;
        }

        template<class Rep1, basic_dimension_unit_type... DimensionUnits1, class Rep2, basic_dimension_unit_type... DimensionUnits2>
            requires((sizeof...(DimensionUnits1) == sizeof...(DimensionUnits2))
                        && (... && (collin::concepts::same<typename DimensionUnits1::unit_values, typename DimensionUnits2::unit_values> &&
                                     collin::concepts::same<typename DimensionUnits1::system, typename DimensionUnits2::system>)
                            )
                    )
        [[nodiscard]] constexpr auto operator-(const basic_derived_unit<Rep1, DimensionUnits1...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
        {
            auto r {lhs};
            r -= rhs;
            return r;
        }

        template<class Rep, class Rep2, basic_dimension_unit_type... DimensionUnits>
        [[nodiscard]] constexpr auto operator*(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const Rep2& rhs) noexcept
        {
            auto r {lhs};
            r *= rhs;
            return r;
        }

        template<class Rep, class Rep2, basic_dimension_unit_type... DimensionUnits>
        [[nodiscard]] constexpr auto operator/(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const Rep2& rhs) noexcept
        {
            auto r {lhs};
            r /= rhs;
            return r;
        }

        //template<class Rep, class Rep2, basic_dimension_unit_type... DimensionUnits>
        //[[nodiscard]] constexpr auto operator%(const basic_derived_unit<Rep, DimensionUnits...>& lhs, const Rep2& rhs) noexcept
        //{
        //    auto r {lhs};
        //    r %= rhs;
        //    return r;
        //}

        //template<class Rep1, basic_dimension_unit_type... DimensionUnits1, class Rep2, basic_dimension_unit_type... DimensionUnits2>
        //    requires((sizeof...(DimensionUnits1) == sizeof...(DimensionUnits2))
        //                && (... && (collin::concepts::same<typename DimensionUnits1::unit_values, typename DimensionUnits2::unit_values> &&
        //                             collin::concepts::same<typename DimensionUnits1::system, typename DimensionUnits2::system>)
        //                    )
        //            )
        //[[nodiscard]] constexpr auto operator%(const basic_derived_unit<Rep1, DimensionUnits1...>& lhs, const basic_derived_unit<Rep2, DimensionUnits2...>& rhs) noexcept
        //{
        //    auto r {lhs};
        //    r %= rhs;
        //    return r;
        //}

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
                constexpr static std::string_view value {"u"};
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

        template<class CharT, class Traits, class Rep, ratio_type Ratio, class UnitValues, class System>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            return os << unit.count() << System::suffix_v<basic_unit<Rep, Ratio, UnitValues, System>>;
        }
    }
}