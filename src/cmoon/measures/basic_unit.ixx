export module cmoon.measures.basic_unit;

import <cstddint>;
import <type_traits>;
import <numeric>;
import <ratio>;
import <concepts>;
import <iostream>;
import <sstream>;

import cmoon.ratio;
import cmoon.format;
import cmoon.format.base_formatter;
import cmoon.format.write_string_view;

import cmoon.measures.dimension_type;
import cmoon.measures.is_basic_unit;
import cmoon.measures.suffix;

namespace std
{
	export
	template<class Rep1, cmoon::ratio_type Ratio1, class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, cmoon::measures::dimension_type Dimension>
	struct common_type<cmoon::measures::basic_unit<Rep1, Ratio1, UnitValues, System, Dimension>, cmoon::measures::basic_unit<Rep2, Ratio2, UnitValues, System, Dimension>>
	{
		private:
            using num_ = std::gcd(Ratio1::num, Ratio2::num);
            using den_ = std::gcd(Ratio1::den, Ratio2::den);
            using cr_  = std::common_type_t<Rep1, Rep2>;
            using r_   = std::ratio<num_::value, (Ratio1::den / den_::value) * Ratio2::den>;
        public:
            using type = cmoon::measures::basic_unit<cr_, r_, UnitValues, System>;
	};
}

namespace cmoon::measures
{
    // Used to bypass dimension requirements
	template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		requires(std::same_as<typename ToBasicUnit::system, System>)
	[[nodiscard]] constexpr ToBasicUnit unit_cast_impl(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
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

	export
	template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
		requires(std::same_as<typename ToBasicUnit::system, System> &&
					ToBasicUnit::dimension == Dimension)
	[[nodiscard]] constexpr ToBasicUnit unit_cast(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
	{
		return unit_cast_impl<ToBasicUnit>(unit);
	}

	export
	template<class ToBasicUnit, class Rep, class Ratio, class UnitValues, class System, dimension_type Dimension>
		requires(basic_unit_type<ToBasicUnit> && cmoon::ratio_type<Ratio> && (std::same_as<typename ToBasicUnit::system, System> ||
					requires(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) { UnitValues::template system_cast<ToBasicUnit>(unit); }))
	[[nodiscard]] constexpr ToBasicUnit system_cast(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit)
	{
		if constexpr (std::same_as<typename ToBasicUnit::system, System>)
		{
			return unit_cast<ToBasicUnit>(unit);
		}
		else
		{
			return UnitValues::template system_cast<ToBasicUnit>(unit);
		}
	}

	export
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

				template<cmoon::ratio_type Ratio2>
				constexpr basic_unit(const basic_unit<rep, Ratio2, unit_values, system, Dimension>& other) noexcept
				: amount_{unit_cast<basic_unit>(other).count()} {}

			constexpr explicit basic_unit(const rep& n) noexcept
				: amount_{n} {}

			template<cmoon::ratio_type Ratio2>
			constexpr basic_unit& operator=(const basic_unit<rep, Ratio2, unit_values, system, Dimension>& other) noexcept
			{
				amount_ = unit_cast<basic_unit>(other).count();
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

			[[nodiscard]] friend constexpr basic_unit operator+(const basic_unit& lhs, const basic_unit& rhs) noexcept
			{
				return basic_unit{lhs.count() + rhs.count()};
			}

			template<class Rep2, cmoon::ratio_type Ratio2>
			[[nodiscard]] friend constexpr auto operator+(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, unit_values, system, dimension>& rhs) noexcept
			{
				using common_t = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio2, unit_values, system, dimension>>;
				return common_t{common_t{lhs}.count() + common_t{rhs}.count()};
			}

			[[nodiscard]] friend constexpr basic_unit operator-(const basic_unit& lhs, const basic_unit& rhs) noexcept
			{
				return basic_unit{lhs.count() - rhs.count()};
			}

			template<class Rep2, cmoon::ratio_type Ratio2>
			[[nodiscard]] friend constexpr auto operator-(const basic_unit& lhs, const basic_unit<Rep2, Ratio2, unit_values, system, dimension>& rhs) noexcept
			{
				using common_t = std::common_type_t<basic_unit, basic_unit<Rep2, Ratio2, unit_values, system, dimension>>;
				return common_t{common_t{lhs}.count() - common_t{rhs}.count()};
			}

			[[nodiscard]] friend constexpr basic_unit operator*(const basic_unit& lhs, const rep& rhs) noexcept
			{
				return basic_unit{lhs.count() * rhs};
			}

			[[nodiscard]] friend constexpr basic_unit operator*(const rep& lhs, const basic_unit& rhs) noexcept
			{
				return basic_unit{lhs * rhs.count()};
			}

			[[nodiscard]] friend constexpr basic_unit operator/(const basic_unit& lhs, const rep& rhs) noexcept
			{
				return basic_unit{lhs.count() / rhs};
			}

			[[nodiscard]] friend constexpr auto operator/(const rep& lhs, const basic_unit& rhs) noexcept
			{
				return basic_unit<rep, ratio, unit_values, system, -Dimension>{lhs / rhs.count()};
			}

			[[nodiscard]] friend constexpr basic_unit operator%(const basic_unit& lhs, const rep& rhs) noexcept
			{
				return basic_unit{lhs.count() % rhs};
			}

			template<cmoon::ratio_type Ratio2, dimension_type Dimension2>
			[[nodiscard]] friend constexpr auto operator*(const basic_unit& lhs, const basic_unit<rep, Ratio2, unit_values, system, Dimension2>& rhs) noexcept
			{
				using lhs_t = basic_unit;
				using rhs_t = basic_unit<rep, Ratio2, unit_values, system, Dimension>;
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

			template<cmoon::ratio_type Ratio2, dimension_type Dimension2>
			[[nodiscard]] friend constexpr auto operator/(const basic_unit& lhs, const basic_unit<rep, Ratio2, unit_values, system, Dimension2>& rhs) noexcept
			{
				using lhs_t = basic_unit;
				using rhs_t = basic_unit<rep, Ratio2, unit_values, system, Dimension>;
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
				*this = *this + other;
				return *this;
			}

			constexpr basic_unit& operator-=(const basic_unit& other) noexcept
			{
				*this = *this - other;
				return *this;
			}

            constexpr basic_unit& operator*=(const rep& other) noexcept
            {
                *this = *this * other;
                return *this;
            }

            constexpr basic_unit& operator/=(const rep& other) noexcept
            {
                *this = *this / other;
                return *this;
            }

            constexpr basic_unit& operator %=(const rep& other) noexcept
            {
                *this = *this % other;
                return *this;
            }

			[[nodiscard]] friend constexpr bool operator==(const basic_unit&, const basic_unit&) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const basic_unit&, const basic_unit&) noexcept = default;
			[[nodiscard]] friend constexpr auto operator<=>(const basic_unit&, const basic_unit&) noexcept = default;

			template<cmoon::ratio_type Ratio2>
			[[nodiscard]] friend constexpr bool operator==(const basic_unit& lhs, const basic_unit<rep, Ratio2, unit_values, system, Dimension>& rhs) noexcept
			{
				return lhs == basic_unit{rhs};
			}

			template<cmoon::ratio_type Ratio2>
			[[nodiscard]] friend constexpr bool operator!=(const basic_unit& lhs, const basic_unit<rep, Ratio2, unit_values, system, Dimension>& rhs) noexcept
			{
				return !(lhs == rhs);
			}

			template<cmoon::ratio_type Ratio2>
			[[nodiscard]] friend constexpr auto operator<=>(const basic_unit& lhs, const basic_unit<rep, Ratio2, unit_values, system, Dimension>& rhs) noexcept
			{
				return lhs <=> basic_unit{rhs};
			}
		private:
			rep amount_ {};
	};

	export
	template<basic_unit_type Unit, dimension_type Dimension>
	using convert_unit_dimension = basic_unit<typename Unit::rep, typename Unit::ratio, typename Unit::unit_values, typename Unit::system, Dimension>;

	export
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

	export
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

	export
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

	export
	template<basic_unit_type ToBasicUnit, class Rep, cmoon::ratio_type Ratio, class UnitValues, class System, dimension_type Dimension>
	[[nodiscard]] constexpr ToBasicUnit abs(const basic_unit<Rep, Ratio, UnitValues, System, Dimension>& unit) noexcept
	{
		return unit >= basic_unit<Rep, Ratio, UnitValues, System, Dimension>::zero() ? unit : -unit;
	}

	export
	template<basic_unit_type T, class CharT, class Traits>
	void output_unit_details(std::basic_ostream<CharT, Traits>& os)
	{
		os << T::system::template suffix_v<typename T::ratio, CharT>;
		os << suffix_v<T, CharT>;

		if constexpr (!suffix_dimension_v<T, CharT>)
		{
			if constexpr (T::dimension != 1)
			{
				os << CharT('^') << T::dimension;
			}
		}
	}

	export
	template<class CharT, class Traits, basic_unit_type T>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const T& unit)
    {
        os << unit.count();
		output_unit_details<T>(os);
		return os;
    }
}

namespace cmoon
{
	export
	template<cmoon::measures::basic_unit_type T, typename CharT>
	struct formatter<T, CharT> : public base_formatter<T, CharT>
	{
		template<class OutputIt>
		auto format(const T& val, basic_format_context<OutputIt, CharT>& ctx)
		{
			std::basic_stringstream<CharT> ss;
			ss.imbue(ctx.locale());
			ss << val;
			const auto str = ss.str();
			return write_string_view(std::basic_string_view<CharT>{str.data(), str.size()}, ctx, this->parser);
		}
	};
}