#pragma once

#include <type_traits>
#include <limits>
#include <ratio>
#include <concepts>
#include <iostream>
#include <sstream>

#include "cmoon/format.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/type_traits.hpp"
#include "cmoon/math.hpp"
#include "cmoon/tuple.hpp"

namespace cmoon
{
	namespace measures
	{
		using dimension_type = std::intmax_t;

		template<class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension = 1>
		class basic_unit;

		template<class T>
		struct is_basic_unit : std::false_type {};

		template<class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		struct is_basic_unit<basic_unit<Rep, Ratio, UnitValues, System, Dimension>> : std::true_type {};

		template<class T>
		constexpr bool is_basic_unit_v = is_basic_unit<T>::value;

		template<class T>
		concept basic_unit_type = is_basic_unit_v<T>;

        template<basic_unit_type Unit>
        struct is_numerator_unit : std::bool_constant<(Unit::dimension > 0)> {};

        template<basic_unit_type Unit>
        constexpr auto is_numerator_unit_v = is_numerator_unit<Unit>::value;

		template<class T>
		concept numerator_unit = basic_unit_type<T> && is_numerator_unit_v<T>;

		template<basic_unit_type Unit>
		struct is_denominator_unit : std::bool_constant<(Unit::dimension < 0)> {};

		template<basic_unit_type Unit>
		constexpr auto is_denominator_unit_v = is_denominator_unit<Unit>::value;

		template<class T>
		concept denominator_unit = basic_unit_type<T> && is_denominator_unit_v<T>;

		template<class Rep, basic_unit_type... Units>
			requires(cmoon::is_unique_v<typename Units::unit_values...> &&
						sizeof...(Units) >= 2
					)
		class basic_derived_unit;

		template<class T>
		struct is_basic_derived_unit : std::false_type {};

		template<class Rep, basic_unit_type... Units>
		struct is_basic_derived_unit<basic_derived_unit<Rep, Units...>> : std::true_type {};

		template<class T>
		constexpr bool is_basic_derived_unit_v = is_basic_derived_unit<T>::value;

		template<class T>
		concept basic_derived_unit_type = is_basic_derived_unit_v<T>;
	}
}

namespace std
{
	template<class Rep1, cmoon::ratio_type Ratio1, class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, cmoon::measures::dimension_type Dimension>
	struct common_type<cmoon::measures::basic_unit<Rep1, Ratio1, UnitValues, System, Dimension>, cmoon::measures::basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>>
	{
		private:
            using num_ = cmoon::static_gcd<Ratio1::num, Ratio2::num>;
            using den_ = cmoon::static_gcd<Ratio1::den, Ratio2::den>;
            using cr_  = std::common_type_t<Rep1, Rep2>;
            using r_   = std::ratio<num_::value, (Ratio1::den / den_::value) * Ratio2::den>;
        public:
            using type = cmoon::measures::basic_unit<cr_, r_, UnitValues, System>;
	};
}

namespace cmoon
{
	namespace measures
	{
		namespace details
		{
			// Used to bypass dimension requirements
			template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
				requires(std::same_as<typename ToBasicUnit::system, System>)
			[[nodiscard]] constexpr ToBasicUnit unit_cast(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
			{
				using ratio = typename std::ratio_divide<Ratio, typename ToBasicUnit::ratio>::type;
				using common_type = std::common_type_t<typename ToBasicUnit::rep, Rep>;

				if constexpr (std::same_as<ToBasicUnit, basic_unit<Rep, Ratio, UnitValues, System, Dimension>>)
				{
					return unit;
				}
				else if constexpr (std::ratio_equal<Ratio, typename ToBasicUnit::ratio>::value)
				{
					return ToBasicUnit(static_cast<typename ToBasicUnit::rep>(unit.count()));
				}
				else
				{
					return ToBasicUnit(
						static_cast<typename ToBasicUnit::rep>(
							static_cast<common_type>(unit.count()) * cmoon::rational_ratio<ratio, common_type>
						)
					);
				}
			}
		}

		template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
			requires(std::same_as<typename ToBasicUnit::system, System> &&
					 ToBasicUnit::dimension == Dimension)
		[[nodiscard]] constexpr ToBasicUnit unit_cast(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
		{
			return details::unit_cast<ToBasicUnit>(unit);
		}

		template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, dimension_type Dimension>
			requires(basic_unit_type<ToBasicUnit> && cmoon::ratio_type<Ratio> && (std::same_as<typename ToBasicUnit::system, System> ||
					 requires(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) { UnitValues::template system_cast<ToBasicUnit>(unit); }))
		[[nodiscard]] constexpr ToBasicUnit system_cast(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit)
		{
			if constexpr (std::is_same_v<typename ToBasicUnit::system, System>)
			{
				return unit_cast<ToBasicUnit>(unit);
			}
			else
			{
				return UnitValues::template system_cast<ToBasicUnit>(unit);
			}
		}

		template<class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		class basic_unit
		{
			public:
				using rep = Rep;
				using ratio = Ratio;
				using unit_values = UnitValues;
				using system = System;
				static constexpr auto dimension = Dimension;

				static constexpr basic_unit zero() noexcept
				{
					return basic_unit{UnitValues<rep>::zero()};
				}

				static constexpr basic_unit min() noexcept
				{
					return basic_unit{UnitValues<rep>::min()};
				}

				static constexpr basic_unit max() noexcept
				{
					return basic_unit{UnitValues<rep>::max()};
				}

				constexpr basic_unit() = default;
				constexpr basic_unit(const basic_unit&) = default;
				constexpr basic_unit(basic_unit&&) noexcept = default;

				constexpr basic_unit& operator=(const basic_unit&) = default;
				constexpr basic_unit& operator=(basic_unit&&) noexcept = default;

				 template<class Rep2, cmoon::ratio_type Ratio2>
					requires(std::constructible_from<rep, Rep2>)
				 constexpr basic_unit(const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& other)
					: amount_{unit_cast<basic_unit>(other).count()} {}

				constexpr explicit basic_unit(const rep& n)
					: amount_{n} {}

				template<class Rep2, cmoon::ratio_type Ratio2>
				constexpr basic_unit& operator=(const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& other)
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
					return *this;
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

				constexpr basic_unit operator--(int) noexcept
				{
					return basic_unit{amount_--};
				}

				constexpr basic_unit& operator+=(const basic_unit& other) noexcept
				{
					amount_ += other.amount_;
					return *this;
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
					requires(requires(rep r, const Rep2& r2) { r += r2; })
				constexpr basic_unit& operator+=(const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& other) noexcept
				{
					amount_ += unit_cast<basic_unit>(other).count();
					return *this;
				}

				constexpr basic_unit& operator-=(const basic_unit& other) noexcept
				{
					amount_ -= other.amount_;
					return *this;
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				    requires(requires(rep r, const Rep2& r2) { r -= r2; })
				constexpr basic_unit& operator-=(const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& other) noexcept
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

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr auto operator+(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					using common_type = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>>;

					common_type r {lhs};
					r += rhs;
					return r;
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr auto operator-(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					using common_type = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>>;

					common_type r {lhs};
					r -= rhs;
					return r;
				}

				template<class Rep2>
					requires(!basic_unit_type<Rep2> && !basic_derived_unit_type<Rep2>)
				[[nodiscard]] friend constexpr auto operator*(const basic_unit& lhs, const Rep2& rhs) noexcept
				{
					using common_type = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio, UnitValues, System, Dimension>>;

					common_type r {lhs};
					r *= rhs;
					return r;
				}

				template<class Rep2>
					requires(!basic_unit_type<Rep2> && !basic_derived_unit_type<Rep2>)
				[[nodiscard]] friend constexpr auto operator*(const Rep2& lhs, const basic_unit& rhs) noexcept
				{
					using common_type = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio, UnitValues, System, Dimension>>;

					common_type r {lhs};
					r *= rhs;
					return r;
				}

				template<class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, dimension_type Dimension2>
				[[nodiscard]] friend constexpr auto operator*(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
				{
					using lhs_t = basic_unit;
					using rhs_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>;
					using common_type = std::common_type_t<lhs_t, rhs_t>;

					const auto amount = std::same_as<lhs_t, rhs_t> ? lhs.count() * rhs.count()
																	 : details::unit_cast<common_type>(lhs).count() * details::unit_cast<common_type>(rhs).count();

					constexpr auto result_dimension = Dimension + Dimension2;
					if constexpr (result_dimension == 0)
					{
						return amount;
					}
					else
					{
						using dimension_common_type = basic_unit<typename common_type::rep, typename common_type::ratio, typename common_type::unit_values, typename common_type::system, result_dimension>;
						return dimension_common_type{amount};
					}
				}

				template<class Rep2>
					requires(!basic_unit_type<Rep2> && !basic_derived_unit_type<Rep2>)
				[[nodiscard]] friend constexpr auto operator/(const basic_unit& lhs, const Rep2& rhs) noexcept
				{
					using common_type = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio, UnitValues, System, Dimension>>;

					common_type r {lhs};
					r /= rhs;
					return r;
				}

				template<class Rep2>
					requires(!basic_unit_type<Rep2> && !basic_derived_unit_type<Rep2>)
				[[nodiscard]] friend constexpr auto operator/(const Rep2& lhs, const basic_unit& rhs) noexcept
				{
					using common_type = std::common_type_t<rep, Rep2>;
					basic_unit<common_type, ratio, unit_values, system, -Dimension> r {lhs};
					r /= rhs.count();
					return r;
				}

				template<class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, dimension_type Dimension2>
				[[nodiscard]] friend constexpr auto operator/(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
				{
					using lhs_t = basic_unit;
					using rhs_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>;
					using common_type = std::common_type_t<lhs_t, rhs_t>;

					const auto amount = std::is_same_v<lhs_t, rhs_t> ? lhs.count() / rhs.count()
						: details::unit_cast<common_type>(lhs).count() / details::unit_cast<common_type>(rhs).count();

					constexpr auto result_dimension = Dimension - Dimension2;
					if constexpr (result_dimension == 0)
					{
						return amount;
					}
					else
					{
						using dimension_common_type = basic_unit<typename common_type::rep, typename common_type::ratio, typename common_type::unit_values, typename common_type::system, result_dimension>;
						return dimension_common_type{amount};
					}
				}

				template<class Rep2>
					requires(!basic_unit_type<Rep2>)
				[[nodiscard]] friend constexpr auto operator%(const basic_unit& lhs, const Rep2& rhs) noexcept
				{
					using common_type = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio, UnitValues, System, Dimension>>;

					common_type r {lhs};
					r %= rhs;
					return r;
				}

				[[nodiscard]] friend constexpr bool operator==(const basic_unit& lhs, const basic_unit& rhs) noexcept
				{
					return lhs.count() == rhs.count();
				}

				[[nodiscard]] friend constexpr bool operator<(const basic_unit& lhs, const basic_unit& rhs) noexcept
				{
					return lhs.count() < rhs.count();
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr bool operator==(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					using common_type = typename std::common_type_t<basic_unit, basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>>;

					return common_type{lhs}.count() == common_type{rhs}.count();
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr bool operator<(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					using common_type = typename std::common_type_t<basic_unit, basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>>;

					return common_type{lhs}.count() < common_type{rhs}.count();
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr bool operator!=(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					return !(lhs == rhs);
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr bool operator<=(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					return !(rhs < lhs);
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr bool operator>(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					return rhs < lhs;
				}

				template<class Rep2, cmoon::ratio_type Ratio2>
				[[nodiscard]] friend constexpr bool operator>=(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>& rhs) noexcept
				{
					return !(lhs < rhs);
				}
			private:
				rep amount_ {};
		};

		template<basic_unit_type Unit, dimension_type Dimension>
		using convert_unit_dimension = basic_unit<typename Unit::rep, typename Unit::ratio, typename Unit::unit_values, typename Unit::system, Dimension>;

		template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		[[nodiscard]] constexpr ToBasicUnit floor(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
		{
			const auto unit2 = unit_cast<ToBasicUnit>(unit);
			if (unit2 > unit)
			{
				return unit2 - ToBasicUnit{1};
			}

			return unit2;
		}

		template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		[[nodiscard]] constexpr ToBasicUnit ceil(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
		{
			const auto unit2 = unit_cast<ToBasicUnit>(unit);
			if (unit2 < unit)
			{
				return unit2 + ToBasicUnit{1};
			}

			return unit2;
		}

		template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		[[nodiscard]] constexpr ToBasicUnit round(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
		{
			const auto unit0 = floor<ToBasicUnit>(unit);
            const auto unit1 = unit0 + ToBasicUnit{1};
            const auto diff0 = unit - unit0;
            const auto diff1 = unit1 - unit;
            if (diff0 == diff1)
            {
                if (cmoon::is_odd(unit0.count()))
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

		template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		[[nodiscard]] constexpr ToBasicUnit abs(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
		{
			return unit >= basic_unit<Rep, Ratio, UnitValues, System, Dimension>::zero() ? unit : -unit;
		}

		template<class Rep, basic_unit_type... Units>
			requires(cmoon::is_unique_v<typename Units::unit_values...> &&
						sizeof...(Units) >= 2
					)
		class basic_derived_unit
		{
			using storage_t = cmoon::basic_rational<Rep>;

			template<basic_unit_type... Units2>
			static constexpr bool same_dimensions = (... && (Units::dimension == Units2::dimension));

			template<basic_unit_type... Units2>
			static constexpr bool same_unit_values = (... && std::same_as<typename Units::unit_values, typename Units2::unit_values>);

			template<basic_unit_type... Units2>
			static constexpr bool same_systems = (... && std::same_as<typename Units::system, typename Units2::system>);

			template<basic_unit_type Unit1, basic_unit_type Unit2>
			static constexpr bool same_measurement = std::same_as<typename Unit1::unit_values, typename Unit2::unit_values> &&
													 std::same_as<typename Unit1::system, typename Unit2::system>;

			public:
				using rep = Rep;
				using units = std::tuple<Units...>;
				using numerator_units = cmoon::tuples::filter_t<is_numerator_unit, units>;
				using denominator_units = cmoon::tuples::filter_t<is_denominator_unit, units>;
				static constexpr auto num_units = sizeof...(Units);
			private:
				using driver_unit = std::tuple_element_t<0, units>;
			public:
				template<basic_unit_type T>
				struct contains_unit : std::bool_constant<(... || same_measurement<T, Units>)> {};

				template<class T>
				static constexpr auto contains_unit_v = contains_unit<T>::value;

				static constexpr basic_derived_unit zero() noexcept
                {
                    return basic_derived_unit{driver_unit::template unit_values<rep>::zero()};
                }

                static constexpr basic_derived_unit min() noexcept
                {
                    return basic_derived_unit{driver_unit::template unit_values<rep>::min()};
                }

                static constexpr basic_derived_unit max() noexcept
                {
                    return basic_derived_unit{driver_unit::template unit_values<rep>::max()};
                }

				constexpr basic_derived_unit() = default;
				constexpr basic_derived_unit(const basic_derived_unit&) = default;
				constexpr basic_derived_unit(basic_derived_unit&&) noexcept = default;

				constexpr basic_derived_unit& operator=(const basic_derived_unit&) = default;
				constexpr basic_derived_unit& operator=(basic_derived_unit&&) noexcept = default;

				constexpr explicit basic_derived_unit(const rep& n)
					: amount_{n} {}

				template<basic_unit_type... Units2>
                    requires(same_unit_values<Units2...>&& same_systems<Units2...> && same_dimensions<Units2...>)
                constexpr basic_derived_unit(const basic_derived_unit<rep, Units2...>& other)
                    : basic_derived_unit{convert_amount(other)} {}

				template<basic_unit_type... Units2>
                    requires(same_unit_values<Units2...>&& same_systems<Units2...> && same_dimensions<Units2...>)
                constexpr basic_derived_unit& operator=(const basic_derived_unit<rep, Units2...>& other)
                {
                    amount_ = convert_amount(other);
                    return *this;
                }

                [[nodiscard]] constexpr rep count() const noexcept
                {
                    return amount_;
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

                constexpr basic_derived_unit& operator--() noexcept
                {
                    --amount_;
                    return *this;
                }

                constexpr basic_derived_unit operator--(int) noexcept
                {
                    return basic_derived_unit{amount_--};
                }

				template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...>&& same_dimensions<Units2...>)
                constexpr basic_derived_unit& operator+=(const basic_derived_unit<Rep2, Units2...>& other) noexcept
                {
					amount_ += convert_amount(other);
                    return *this;
                }

				template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...>&& same_dimensions<Units2...>)
                constexpr basic_derived_unit& operator-=(const basic_derived_unit<Rep2, Units2...>& other) noexcept
                {
					amount_ -= convert_amount(other);
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

				template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] friend constexpr bool operator==(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
					return amount_ == basic_derived_unit{rhs}.count();
                }

				template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] friend constexpr bool operator!=(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    return !(lhs == rhs);
                }

                template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] friend constexpr bool operator<(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    using t = result_ratio<Units2...>;

                    if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
                    {
                        return lhs.count() < rhs.count();
                    }
                    else
                    {
                        return lhs.count() < basic_derived_unit<Rep, Units...>{rhs}.count();
                    }
                }

                template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] friend constexpr bool operator>(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    return rhs < lhs;
                }

                template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] friend constexpr bool operator<=(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    return !(rhs < lhs);
                }

                template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] friend constexpr bool operator>=(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    return !(lhs < rhs);
                }

                template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] constexpr friend auto operator+(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    auto r {lhs};
                    r += rhs;
                    return r;
                }

                template<class Rep2, basic_unit_type... Units2>
                    requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
                [[nodiscard]] constexpr friend auto operator-(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
                {
                    auto r {lhs};
                    r -= rhs;
                    return r;
                }

                template<class Rep2>
					requires(!basic_unit_type<Rep2> && !basic_derived_unit_type<Rep2>)
                [[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const Rep2& rhs) noexcept
                {
                    auto r {lhs};
                    r *= rhs;
                    return r;
                }

				template<class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, dimension_type Dimension2>
				[[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
				{
					using unit_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>;
					using result_t = derived_result_one_t<unit_t, multiply_op>;
					using common_t = typename result_t::rep;
					const auto amount = static_cast<common_t>(static_cast<common_t>(lhs.count()) * static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<result_ratio_t<unit_t>, common_t>);

					if constexpr (basic_derived_unit_type<result_t>)
					{
						return result_t{amount};
					}
					else
					{
						return result_t{static_cast<common_t>(amount)};
					}
				}

				template<class Rep2, basic_unit_type... Units2>
				[[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
				{
					using result_t = derived_result_many_t<multiply_op, Units2...>;
					using common_t = std::common_type_t<Rep, Rep2>;
					const auto amount = static_cast<common_t>(static_cast<common_t>(lhs.count()) * static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<result_ratio_t<Units2...>, common_t>);

					if constexpr (basic_derived_unit_type<result_t>)
					{
						return result_t{amount};
					}
					else
					{
						return result_t{static_cast<common_t>(amount)};
					}
				}

                template<class Rep2>
					requires(!basic_unit_type<Rep2> && !basic_derived_unit_type<Rep2>)
                [[nodiscard]] constexpr friend auto operator/(const basic_derived_unit& lhs, const Rep2& rhs) noexcept
                {
                    auto r {lhs};
                    r /= rhs;
                    return r;
                }

				template<class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, dimension_type Dimension2>
				[[nodiscard]] constexpr friend auto operator/(const basic_derived_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
				{
					using unit_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>;
					using result_t = derived_result_one_t<unit_t, divide_op>;
					using common_t = typename result_t::rep;

					const auto amount = static_cast<common_t>(static_cast<common_t>(lhs.count()) / static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<cmoon::ratio_reciprocal<result_ratio_t<unit_t>>, common_t>);

					return result_t{amount};
				}

				template<class Rep2, basic_unit_type... Units2>
				[[nodiscard]] constexpr friend auto operator/(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Units2...>& rhs) noexcept
				{
					using result_t = derived_result_many_t<divide_op, Units2...>;
					using common_t = std::common_type_t<Rep, Rep2>;
					const auto amount = static_cast<common_t>(static_cast<common_t>(lhs.count()) / static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<cmoon::ratio_reciprocal<result_ratio_t<Units2...>>, common_t>);

					return result_t{amount};
				}

				/*
 				  Every thing below here should be private. Anything that is public
				  is only because I am not allowed to declare that this class is also
				  a friend to itself with different types of units. For example:

				    template<class Rep2, basic_unit_type... Units2>
					friend class basic_derived_unit<Rep2, Units2...>;

				  The above would fix having the below structs from needing to be
				  public. However, Visual Studio 19 does not allow me to do this.

				*/

				template<class Rep2, basic_unit_type... Units2>
				basic_derived_unit<Rep2, Units2...> static get_tuple_derived_t(std::tuple<Units2...>);

				struct multiply_op
				{
					[[nodiscard]] static constexpr dimension_type value(dimension_type D1, dimension_type D2) noexcept
					{
						return D1 + D2;
					}
				};

				struct divide_op
				{
					[[nodiscard]] static constexpr dimension_type value(dimension_type D1, dimension_type D2) noexcept
					{
						return D1 - D2;
					}
				};

				template<basic_unit_type NewUnit, class Operation>
				struct derived_result_one
				{
					private:
						template<basic_unit_type E>
						struct result_impl
						{
							using type = std::tuple<E>;
						};

						template<basic_unit_type E>
							requires(same_measurement<E, NewUnit> && (Operation::value(E::dimension, NewUnit::dimension) != 0))
						struct result_impl<E>
						{
							private:
								using common_type = std::common_type_t<E, convert_unit_dimension<NewUnit, E::dimension>>;
							public:
								static constexpr auto dimension_result = Operation::value(E::dimension, NewUnit::dimension);
								using type = std::tuple<convert_unit_dimension<common_type, dimension_result>>;
						};

						template<basic_unit_type E>
							requires(same_measurement<E, NewUnit> && (Operation::value(E::dimension, NewUnit::dimension) == 0))
						struct result_impl<E>
						{
							using type = std::tuple<>;
						};

						template<basic_unit_type E>
						using result_impl_t = typename result_impl<E>::type;

						using full_tuple_type = std::conditional_t<contains_unit_v<NewUnit>,
													decltype(std::tuple_cat(std::declval<result_impl_t<Units>>()...)),
													std::tuple<Units..., NewUnit>>;

						template<class EndingTuple>
						struct end_type_picker
						{
							using type = decltype(get_tuple_derived_t<typename NewUnit::rep>(
								std::declval<EndingTuple>()));
						};

						template<class EndingTuple>
							requires(std::tuple_size_v<EndingTuple> == 1)
						struct end_type_picker<EndingTuple>
						{
							using type = std::tuple_element_t<0, EndingTuple>;
						};

						template<class EndingTuple>
						using end_type_picker_t = typename end_type_picker<EndingTuple>::type;
					public:
						using type = end_type_picker_t<full_tuple_type>;
				};

				template<basic_unit_type NewUnit, class Operation>
				using derived_result_one_t = typename derived_result_one<NewUnit, Operation>::type;

				template<class Operation, basic_unit_type First, basic_unit_type... Rest>
				struct derived_result_many
				{
					private:
						// We have exhausted our Rest... list and accept the final result
						template<class T>
						struct dispatcher
						{
							using type = T;
						};

						// Got more to go
						template<basic_derived_unit_type T>
							requires(requires{ typename T::template derived_result_many<Operation, Rest...>::type; })
						struct dispatcher<T>
						{
							using type = typename T::template derived_result_many<Operation, Rest...>::type;
						};

						// Must be only one left in Rest...
						template<basic_unit_type T>
						struct dispatcher<T>
						{
							using tuple_helper = std::tuple<Rest...>;
							using tuple_first = std::tuple_element_t<0, tuple_helper>;
							using type = std::conditional_t<
								std::is_same_v<Operation, multiply_op>,
								decltype(std::declval<T>() * std::declval<tuple_first>()),
								decltype(std::declval<T>() / std::declval<tuple_first>())
							>;
						};

						using result_t = derived_result_one_t<First, Operation>;
					public:			
						using type = typename dispatcher<result_t>::type;
				};

				template<class Operation, basic_unit_type First, basic_unit_type... Rest>
				using derived_result_many_t = typename derived_result_many<Operation, First, Rest...>::type;
			private:
				rep amount_ {0};

				template<basic_unit_type... Units2>
				struct result_ratio
				{
					private:
						template<basic_unit_type E>
						struct result_impl
						{
							using type = cmoon::ratio_multiply_many_t<std::conditional_t<
									std::is_same_v<typename E::unit_values, typename Units::unit_values> &&
									std::is_same_v<typename E::system, typename Units::system>,
									std::conditional_t<numerator_unit<Units>,
										std::ratio_divide<
											typename E::ratio,
											typename Units::ratio
										>,
										std::ratio_divide<
											typename Units::ratio,
											typename E::ratio
										>
									>,
									std::ratio<1>
							>...>;
						};

						template<basic_unit_type E>
						using result_impl_t = typename result_impl<E>::type;
					public:
						using type = cmoon::ratio_canonical<cmoon::ratio_multiply_many_t<
							result_impl_t<Units2>...
						>>;
				};

				template<basic_unit_type... Units2>
				using result_ratio_t = typename result_ratio<Units2...>::type;

				template<class Rep2, basic_unit_type... Units2>
				[[nodiscard]] static constexpr rep convert_amount(const basic_derived_unit<Rep2, Units2...>& other) noexcept
				{
					using t = result_ratio_t<Units2...>;

					if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
					{
						return other.count();
					}
					else
					{
						using common_type = std::common_type_t<Rep, Rep2>;
						return static_cast<rep>(other.count() * cmoon::rational_ratio<t, common_type>);
					}
				}
		};

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

        template<class CharT, class Traits, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
        std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit)
        {
            os << unit.count() << System::template suffix_v<basic_unit<Rep, Ratio, UnitValues, System, Dimension>>;
			if constexpr (Dimension != 1)
			{
				os << '^' << Dimension;
			}
			return os;
        }
	}

	template<class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, measures::dimension_type Dimension, typename CharT>
	struct formatter<measures::basic_unit<Rep, Ratio, UnitValues, System, Dimension>, CharT> : public details::base_formatter<measures::basic_unit<Rep, Ratio, UnitValues, System, Dimension>, CharT>
	{
		template<class OutputIt>
		auto format(const measures::basic_unit<Rep, Ratio, UnitValues, System, Dimension>& val, basic_format_context<OutputIt, CharT>& ctx)
		{
			std::basic_stringstream<CharT> ss;
			ss << val;
			const auto str = ss.str();
			return details::write_string_view(std::basic_string_view<CharT>{str.data(), str.size()}, ctx, this->parser);
		}
	};
}