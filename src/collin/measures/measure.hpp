#pragma once

#include <ratio>
#include <string_view>
#include <type_traits>
#include <stdexcept>
#include <iostream>

#include "collin/math.hpp"
#include "collin/type_traits.hpp"

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

        constexpr std::intmax_t dynamic_dimension = std::numeric_limits<std::intmax_t>::max();

        template<std::intmax_t Ext>
        class dimension_type
        {
            public:
                using size_type = std::intmax_t;

                constexpr dimension_type() noexcept = default;

                template<size_type Other>
                constexpr dimension_type(dimension_type<Other> ext)
                {
                    static_assert(Other == Ext);
                }

                constexpr dimension_type(size_type size) {}

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

                template<size_type Other>
                explicit constexpr dimension_type(dimension_type<Other> ext)
                    : size_(ext.size()) {}

                explicit constexpr dimension_type(size_type size)
                    : size_(size) {}

                constexpr size_type size() const noexcept
                {
                    return size_;
                }

            private:
                size_type size_;
        };

        template<class Rep, class Ratio, class UnitValues, class System, std::intmax_t Dimension = dynamic_dimension, typename = std::enable_if_t<
/* requires */  Dimension != 0
        >>
        class basic_dimension_unit;

        template<class T>
        struct is_basic_dimension_unit : std::false_type {};

        template<class Rep, class Ratio, class UnitValues, class System, std::intmax_t Dimension>
        struct is_basic_dimension_unit<basic_dimension_unit<Rep, Ratio, UnitValues, System, Dimension>> : std::true_type {};

        template<class T>
        constexpr bool is_basic_dimension_unit_v = is_basic_dimension_unit<T>::value;

        template<class... DimensionUnits>
        class basic_derived_unit;
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
                return unit_cast<ToBasicUnit>(unit);
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

                constexpr basic_unit& operator=(const basic_unit&) = default;
                constexpr basic_unit& operator=(basic_unit&&) noexcept = default;

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

        template<class Rep, class Ratio, class UnitValues, class System, std::intmax_t Dimension, typename>
        class basic_dimension_unit
        {

            public:
                using rep = Rep;
                using ratio = Ratio;
                using unit_values = UnitValues;
                using system = System;
                using dimension_t = dimension_type<Dimension>;
                using unit_t = basic_unit<rep, ratio, unit_values, system>;

                static constexpr auto dimension_v = Dimension;

                constexpr basic_dimension_unit() = default;
                constexpr basic_dimension_unit(const basic_dimension_unit&) = default;
                constexpr basic_dimension_unit(basic_dimension_unit&&) noexcept = default;

                constexpr basic_dimension_unit& operator=(const basic_dimension_unit&) = default;
                constexpr basic_dimension_unit& operator=(basic_dimension_unit&&) = default;

                ~basic_dimension_unit() noexcept = default;

                // TODO: Convert if dimension is dynamic on either side
                template<class Rep2, class Ratio2>
                constexpr basic_dimension_unit(const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension>& other)
                    : unit_{other.unit()} {}

                template<class Rep2>
                constexpr explicit basic_dimension_unit(const Rep2& n, std::intmax_t dim = {1})
                    : unit_{n}, dimension_{Dimension == dynamic_dimension ? dim : Dimension} {}

                constexpr typename dimension_t::size_type dimension() const noexcept
                {
                    return dimension_.size();
                }

                constexpr unit_t& unit() noexcept
                {
                    return unit_;
                }

                constexpr const unit_t& unit() const noexcept
                {
                    return unit_;
                }

            private:
                unit_t unit_ {};
                dimension_t dimension_ {1};
        };

        template<class Rep, class Ratio, class UnitValues, class System>
        class basic_dimension_unit<Rep, Ratio, UnitValues, System, 1> : public basic_unit<Rep, Ratio, UnitValues, System>
        {

            public:
                using dimension_t = dimension_type<1>;
                using unit_t = basic_unit<Rep, Ratio, UnitValues, System>;

                static constexpr auto dimension_v = 1;

                constexpr basic_dimension_unit() = default;
                constexpr basic_dimension_unit(const basic_dimension_unit&) = default;
                constexpr basic_dimension_unit(basic_dimension_unit&&) noexcept = default;

                constexpr basic_dimension_unit& operator=(const basic_dimension_unit&) = default;
                constexpr basic_dimension_unit& operator=(basic_dimension_unit&&) noexcept = default;

                ~basic_dimension_unit() noexcept = default;

                template<class Rep2>
                constexpr explicit basic_dimension_unit(const Rep2& n)
                    : unit_t{n} {}

                template<class Rep2, class Ratio2>
                constexpr basic_dimension_unit(const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>& other)
                    : unit_t{other.unit_} {}

                constexpr typename dimension_t::size_type dimension() const noexcept
                {
                    return 1;
                }

                constexpr unit_t& unit() noexcept
                {
                    return *this;
                }

                constexpr const unit_t& unit() const noexcept
                {
                    return *this;
                }
        };

        template<class T, std::intmax_t Dimension, typename = std::enable_if_t<
/* requires */  is_basic_unit_v<T>
        >>
        using basic_dimension_unit_t = basic_dimension_unit<typename T::rep, typename T::ratio, typename T::unit_values, typename T::system, Dimension>;

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        constexpr bool operator==(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs)
        {
            using common_type = std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                   basic_unit<Rep2, Ratio2, UnitValues, System>>;

            auto lhs_normalized_unit = common_type{lhs.unit()};
            auto rhs_normalized_unit = common_type{rhs.unit()};
            if (lhs.dimension() > rhs.dimension())
            {
                const auto difference = collin::math::abs(lhs.dimension() - rhs.dimension());
                lhs_normalized_unit = common_type{collin::math::pow(lhs_normalized_unit.count(), difference + 1)};
            }
            else if (rhs.dimension() > lhs.dimension())
            {
                const auto difference = collin::math::abs(lhs.dimension() - rhs.dimension());
                rhs_normalized_unit = common_type{collin::math::pow(rhs_normalized_unit.count(), difference + 1)};
            }

            return lhs_normalized_unit == rhs_normalized_unit;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        constexpr bool operator!=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs)
        {
            return !(lhs == rhs);
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        constexpr bool operator<(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs)
        {
            using common_type = std::common_type_t<basic_unit<Rep1, Ratio1, UnitValues, System>,
                                                   basic_unit<Rep2, Ratio2, UnitValues, System>>;

            auto lhs_normalized_unit = common_type{lhs.unit()};
            auto rhs_normalized_unit = common_type{rhs.unit()};
            if (lhs.dimension() > rhs.dimension())
            {
                const auto difference = collin::math::abs(lhs.dimension() - rhs.dimension());
                lhs_normalized_unit = common_type{collin::math::pow(lhs_normalized_unit.count(), difference + 1)};
            }
            else if (rhs.dimension() > lhs.dimension())
            {
                const auto difference = collin::math::abs(lhs.dimension() - rhs.dimension());
                rhs_normalized_unit = common_type{collin::math::pow(rhs_normalized_unit.count(), difference + 1)};
            }
            
            return lhs_normalized_unit < rhs_normalized_unit;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        constexpr bool operator>(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs)
        {
            return rhs < lhs;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        constexpr bool operator<=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs)
        {
            return !(rhs < lhs);
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension1, std::intmax_t Dimension2>
        constexpr bool operator>=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension1>& lhs, const basic_dimension_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs)
        {
            return !(lhs < rhs);
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator==(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs == basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator==(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} == rhs;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator!=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs != basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator!=(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} != rhs;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator<(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs < basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator<(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} < rhs;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator>(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs > basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator>(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} > rhs;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator<=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs <= basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator<=(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} <= rhs;
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator>=(const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System>& rhs)
        {
            return lhs >= basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{rhs};
        }

        template<class Rep1, class Ratio1, class Rep2, class Ratio2, class UnitValues, class System, std::intmax_t Dimension>
        constexpr bool operator>=(const basic_unit<Rep2, Ratio2, UnitValues, System>& lhs, const basic_dimension_unit<Rep1, Ratio1, UnitValues, System, Dimension>& rhs)
        {
            return basic_dimension_unit<Rep2, Ratio2, UnitValues, System, 1>{lhs} >= rhs;
        }

        template<class... DimensionUnits>
        class basic_derived_unit : public std::tuple<DimensionUnits...>
        {
        public:
            using units = std::tuple<DimensionUnits...>;

            static constexpr auto num_units = sizeof...(DimensionUnits);

            constexpr basic_derived_unit()
                : units{{DimensionUnits{}}...} {}

            template<class... Reps, typename = std::enable_if_t<
    /* requires */  num_units == sizeof...(Reps)
            >>
            constexpr explicit basic_derived_unit(Reps&&... reps)
                : units{{DimensionUnits{std::forward<Reps>(reps)}}...} {}

            constexpr basic_derived_unit(const basic_derived_unit&) = default;
            constexpr basic_derived_unit(basic_derived_unit&&) noexcept = default;

            constexpr basic_derived_unit& operator=(const basic_derived_unit&) = default;
            constexpr basic_derived_unit& operator=(basic_derived_unit&&) noexcept = default;

            ~basic_derived_unit() noexcept = default;

        private:
            using enable_check = std::enable_if_t<
                (num_units >= 2) && // Must have two or more units to be considered a derived unit
                (... && is_basic_dimension_unit_v<DimensionUnits>) && // Unit types must be dimensional units
                type_traits::is_unique_v<typename DimensionUnits::unit_values...> // Unit values cannot be repeated (i.e. two distance units)
            >;
        };

        namespace details
        {
            template<class T, class... Units, std::size_t I>
            constexpr auto& get_helper(const measures::basic_derived_unit<Units...>& units, std::index_sequence<I>) noexcept
            {
                using this_type = std::tuple_element_t<I, std::tuple<Units...>>;
                if constexpr (std::is_convertible_v<T, typename this_type::unit_t>)
                {
                    return std::get<I>(units);
                }
            }

            template<class T, class... Units, std::size_t F, std::size_t... I>
            constexpr auto& get_helper(const measures::basic_derived_unit<Units...>& units, std::index_sequence<F, I...>) noexcept
            {
                using this_type = std::tuple_element_t<F, std::tuple<Units...>>;

                if constexpr(std::is_convertible_v<T, typename this_type::unit_t>)
                {
                    return std::get<F>(units);
                }
                else
                {
                    return get_helper<T>(units, std::index_sequence<I...>{});
                }
            }
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

        template<class CharT, class Traits, class Rep, class Ratio, class UnitValues, class System>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_unit<Rep, Ratio, UnitValues, System>& unit)
        {
            return os << unit.count() << System::suffix_v<basic_unit<Rep, Ratio, UnitValues, System>>;
        }
    }
}

namespace std
{
    template<class T, std::intmax_t Dimension, class... Units>
    constexpr T& get(collin::measures::basic_derived_unit<Units...>& units) noexcept
    {
        return std::get<collin::measures::basic_dimension_unit_t<T, Dimension>>(units).unit();
    }

    template<class T, std::intmax_t Dimension, class... Units>
    constexpr T&& get(collin::measures::basic_derived_unit<Units...>&& units) noexcept
    {
        return std::get<collin::measures::basic_dimension_unit_t<T, Dimension>>(units).unit();
    }

    template<class T, std::intmax_t Dimension, class... Units>
    constexpr const T& get(const collin::measures::basic_derived_unit<Units...>& units) noexcept
    {
        return std::get<collin::measures::basic_dimension_unit_t<T, Dimension>>(units).unit();
    }

    template<class T, std::intmax_t Dimension, class... Units>
    constexpr const T&& get(const collin::measures::basic_derived_unit<Units...>&& units) noexcept
    {
        return std::get<collin::measures::basic_dimension_unit_t<T, Dimension>>(units).unit();
    }

    template<class T, class... Units>
    constexpr auto& get(const collin::measures::basic_derived_unit<Units...>& units) noexcept
    {
        return collin::measures::details::get_helper<T>(units, std::make_index_sequence<sizeof...(Units)>{});
    }
}