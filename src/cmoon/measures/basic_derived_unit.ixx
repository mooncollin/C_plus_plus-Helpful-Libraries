module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.basic_derived_unit;

import <tuple>;
import <concepts>;
import <ratio>;
import <type_traits>;
import <functional>;
import <iostream>;
import <format>;
import <string>;

import cmoon.ratio;
import cmoon.math;
import cmoon.string;
import cmoon.meta;
import cmoon.tuple;

import cmoon.measures.basic_unit;
import cmoon.measures.ratio;
import cmoon.measures.dimension_type;

namespace cmoon::measures
{
	export
	template<class Rep, class Ratio, basic_unit_type... Units>
		requires(cmoon::is_unique_v<typename Units::unit_values...> &&
				 sizeof...(Units) >= 1
				)
	class basic_derived_unit;

	// For type checking only
	template<class Rep, class Ratio, class... Units>
	std::true_type is_basic_derived_unit_base_impl(const basic_derived_unit<Rep, Ratio, Units...>&);

	std::false_type is_basic_derived_unit_base_impl(...);

	template<class U>
	constexpr auto is_based_in_basic_derived_unit = decltype(is_basic_derived_unit_base_impl(std::declval<U>()))::value;

	export
	template<class T>
	struct is_basic_derived_unit : std::bool_constant<is_based_in_basic_derived_unit<T>> {};

	export
	template<class T>
	constexpr bool is_basic_derived_unit_v = is_basic_derived_unit<T>::value;

	export
	template<class T>
	concept basic_derived_unit_type = is_basic_derived_unit_v<T>;

	export
	template<basic_derived_unit_type ToDerivedUnit, class Rep, class Ratio, basic_unit_type... Units>
		requires(std::same_as<typename ToDerivedUnit::units, cmoon::meta::type_list<Units...>>)
	[[nodiscard]] constexpr ToDerivedUnit unit_cast(const basic_derived_unit<Rep, Ratio, Units...>& unit) noexcept
	{
		using ratio = typename std::ratio_divide<Ratio, typename ToDerivedUnit::ratio>::type;
		using common_type = std::common_type_t<typename ToDerivedUnit::rep, Rep>;

		if constexpr (std::same_as<ToDerivedUnit, basic_derived_unit<Rep, Ratio, Units...>>)
		{
			return unit;
		}
		else if constexpr (std::ratio_equal<Ratio, typename ToDerivedUnit::ratio>::value)
		{
			return ToDerivedUnit(static_cast<typename ToDerivedUnit::rep>(unit.count()));
		}
		else
		{
			return ToDerivedUnit(
				static_cast<typename ToDerivedUnit::rep>(
					static_cast<common_type>(unit.count()) * cmoon::rational_ratio<ratio, common_type>
				)
			);
		}
	}
	
	export
	template<class Rep, class Ratio, basic_unit_type... Units>
		requires(cmoon::is_unique_v<typename Units::unit_values...> &&
				 sizeof...(Units) >= 1
				)
	class basic_derived_unit
	{
		template<class Rep2, basic_unit_type... Units2>
		friend class basic_derived_unit;

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
			using ratio = Ratio;
			using units = cmoon::meta::type_list<Units...>;
			using numerator_units = units::template filter<is_numerator_unit>;
			using denominator_units = units::template filter<is_denominator_unit>;
		private:
			using driver_unit = units::template type<0>;
		public:
			using system = typename driver_unit::system;

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

			template<class Ratio2, basic_unit_type... Units2>
                requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
            constexpr basic_derived_unit(const basic_derived_unit<rep, Ratio2, Units2...>& other)
                : basic_derived_unit{convert_amount(other)} {}

			template<basic_unit_type... Units2>
                requires(same_unit_values<Units2...> && same_systems<Units2...> && same_dimensions<Units2...>)
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

            [[nodiscard]] constexpr friend basic_derived_unit operator+(const basic_derived_unit& lhs, const basic_derived_unit& rhs) noexcept
            {
				return basic_derived_unit{lhs.count() + rhs.count()};
            }

            [[nodiscard]] constexpr friend basic_derived_unit operator-(const basic_derived_unit& lhs, const basic_derived_unit& rhs) noexcept
            {
				return basic_derived_unit{lhs.count() - rhs.count()};
            }

            [[nodiscard]] constexpr friend basic_derived_unit operator*(const basic_derived_unit& lhs, const rep& rhs) noexcept
            {
				return basic_derived_unit{lhs.count() * rhs};
            }

            [[nodiscard]] constexpr friend basic_derived_unit operator/(const basic_derived_unit& lhs, const rep& rhs) noexcept
            {
                return basic_derived_unit{lhs.count() / rhs};
            }

			template<class Rep2, class Ratio2, class UnitValues, class System, dimension_type Dimension2>
			[[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
			{
				using unit_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>;
				using result_t = derived_result_one_t<unit_t, std::plus<>>;
				using common_t = std::common_type_t<rep, Rep2>;
				const auto amount = static_cast<common_t>(lhs.count()) * static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<result_ratio_t<unit_t>, common_t>;

				return result_t{static_cast<common_t>(amount)};
			}

			template<class Ratio2, class UnitValues, class System, dimension_type Dimension2>
			[[nodiscard]] constexpr friend auto operator*(const basic_unit<rep, Ratio2, UnitValues, System, Dimension2>& lhs, const basic_derived_unit& rhs) noexcept
			{
				return rhs * lhs;
			}

			template<class Rep2, class Ratio2, basic_unit_type... Units2>
			[[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Ratio2, Units2...>& rhs) noexcept
			{
				using result_t = derived_result_many_t<std::plus<>, Units2...>;
				using common_t = std::common_type_t<basic_derived_unit, basic_derived_unit<Rep2, Ratio2, Units...>>;
				using ratio_t = typename std::ratio_multiply<typename common_t::ratio, result_ratio_t<Units2...>>::type;

				const auto amount = static_cast<typename common_t::rep>(lhs.count()) * static_cast<typename common_t::rep>(rhs.count()) * cmoon::rational_ratio<ratio_t, typename common_t::rep>;

				return result_t{static_cast<typename common_t::rep>(amount)};
			}

			template<class Rep2, class Ratio2, class UnitValues, class System, dimension_type Dimension2>
			[[nodiscard]] constexpr friend auto operator/(const basic_derived_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
			{
				using unit_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>;
				using result_t = derived_result_one_t<unit_t, std::minus<>>;
				using common_t = typename result_t::rep;

				const auto amount = static_cast<common_t>(static_cast<common_t>(lhs.count()) / static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<cmoon::ratio_reciprocal<result_ratio_t<unit_t>>, common_t>);

				return result_t{amount};
			}

			template<class Rep2, class Ratio2, basic_unit_type... Units2>
			[[nodiscard]] constexpr friend auto operator/(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Ratio2, Units2...>& rhs) noexcept
			{
				using result_t = derived_result_many_t<std::minus<>, Units2...>;
				using common_t = std::common_type_t<basic_derived_unit, basic_derived_unit<Rep2, Ratio2, Units...>>;
				using ratio_t = typename std::ratio_multiply<typename common_t::ratio, result_ratio_t<Units2...>>::type;

				const auto amount = static_cast<typename common_t::rep>(lhs.count()) / static_cast<typename common_t::rep>(rhs.count()) * cmoon::rational_ratio<ratio_t, typename common_t::rep>;

				return result_t{amount};
			}

            constexpr basic_derived_unit& operator+=(const basic_derived_unit& other) noexcept
            {
				*this = *this + other;
                return *this;
            }

            constexpr basic_derived_unit& operator-=(const basic_derived_unit& other) noexcept
            {
				*this = *this - other;
                return *this;
            }

            constexpr basic_derived_unit& operator*=(const rep& other) noexcept
            {
                *this = *this * other;
                return *this;
            }

            constexpr basic_derived_unit& operator/=(const rep& other) noexcept
            {
                *this = *this / other;
                return *this;
            }

			[[nodiscard]] friend constexpr bool operator==(const basic_derived_unit&, const basic_derived_unit&) noexcept = default;
			[[nodiscard]] friend constexpr bool operator!=(const basic_derived_unit&, const basic_derived_unit&) noexcept = default;
			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const basic_derived_unit&, const basic_derived_unit&) noexcept = default;

			template<class Ratio2, basic_unit_type... Units2>
            [[nodiscard]] friend constexpr bool operator==(const basic_derived_unit& lhs, const basic_derived_unit<rep, Ratio2, Units2...>& rhs) noexcept
            {
				return lhs == basic_derived_unit{rhs};
            }

			template<class Ratio2, basic_unit_type... Units2>
            [[nodiscard]] friend constexpr bool operator!=(const basic_derived_unit& lhs, const basic_derived_unit<rep, Ratio2, Units2...>& rhs) noexcept
            {
                return !(lhs == rhs);
            }

			template<class Ratio2, basic_unit_type... Units2>
			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const basic_derived_unit& lhs, const basic_derived_unit<rep, Ratio2, Units2...>& rhs) noexcept
			{
				return lhs <=> basic_derived_unit{rhs};
			}
		private:
			rep amount_ {0};

			template<basic_unit_type NewUnit, class Operation>
			struct derived_result_one
			{
				private:
					template<basic_unit_type E>
					struct merge_same_measurement : std::type_identity<E> {};

					template<basic_unit_type E>
						requires(same_measurement<E, NewUnit>)
					struct merge_same_measurement<E>
					{
						private:
							using common_type = std::common_type_t<E, convert_unit_dimension<NewUnit, E::dimension>>;
							static constexpr auto dimension_result = Operation{}(E::dimension, NewUnit::dimension);
						public:
							using type = convert_unit_dimension<common_type, dimension_result>;
					};

					template<basic_unit_type E>
					struct not_zero_dimension
					{
						static constexpr auto value = Operation{}(E::dimension, NewUnit::dimension) != 0;
					};

					// If we already have this new unit as our set of units,
					// then produce the resulting tuple by traversing through
					// our list of units, returning a list of zero or more types
					// depending on the result of the operation on the two types.
					// Otherwise, append the new type to the end.
					using full_type = std::conditional_t<contains_unit_v<NewUnit>,
											typename cmoon::meta::filter<not_zero_dimension, Units...>::template transform<merge_same_measurement>,
											cmoon::meta::type_list<Units..., NewUnit>>;

					template<class TypeList>
					struct final_type_picker : std::type_identity<
						typename TypeList::template complete_type<basic_derived_unit, typename NewUnit::rep, Ratio>
					> {};

					template<class TypeList>
						requires(TypeList::size() == 1)
					struct final_type_picker<TypeList> : std::type_identity<
							typename TypeList::template type<0>
					> {};
				public:
					using type = typename final_type_picker<full_type>::type;
			};

			template<basic_unit_type NewUnit, class Operation>
			using derived_result_one_t = typename derived_result_one<NewUnit, Operation>::type;

			template<class Operation, basic_unit_type First, basic_unit_type... Rest>
			struct derived_result_many
			{
				private:
					// We have exhausted our Rest... list and accept the final result
					template<class T>
					struct dispatcher : std::type_identity<T> {};

					// Got more to go
					template<basic_derived_unit_type T>
					struct dispatcher<T> : std::type_identity<
						typename T::template derived_result_many<Operation, Rest...>::type
					> {};

					// Must be only one left in Rest...
					template<basic_unit_type T>
					struct dispatcher<T>
					{
						private:
							using first_type = cmoon::meta::get_type<0, Rest...>;
						public:
							using type = std::conditional_t<
								std::same_as<Operation, std::plus<>>,
								decltype(std::declval<T>() * std::declval<first_type>()),
								decltype(std::declval<T>() / std::declval<first_type>())
							>;
					};
				public:
					using type = typename dispatcher<derived_result_one_t<First, Operation>>::type;
			};

			template<class Operation, basic_unit_type First, basic_unit_type... Rest>
			using derived_result_many_t = typename derived_result_many<Operation, First, Rest...>::type;

			template<basic_unit_type... Units2>
			struct result_ratio
			{
				private:
					template<basic_unit_type E>
					struct result_impl
					{
						using type = cmoon::ratio_multiply_many<std::conditional_t<
								std::same_as<typename E::unit_values, typename Units::unit_values> &&
								std::same_as<typename E::system, typename Units::system>,
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
				public:
					using type = cmoon::ratio_canonical<cmoon::ratio_multiply_many<
						typename result_impl<Units2>::type...
					>>;
			};

			template<basic_unit_type... Units2>
			using result_ratio_t = typename result_ratio<Units2...>::type;

			template<class Rep2, class Ratio2, basic_unit_type... Units2>
			[[nodiscard]] static constexpr rep convert_amount(const basic_derived_unit<Rep2, Ratio2, Units2...>& other) noexcept
			{
				// Both have the same units as I only need to common representations and ratios.
				using common_t = std::common_type_t<basic_derived_unit, basic_derived_unit<Rep2, Ratio2, Units...>>;

				using t = typename std::ratio_multiply<typename common_t::ratio, result_ratio_t<Units2...>>::type;

				if constexpr (std::ratio_equal_v<t, std::ratio<1, 1>>)
				{
					return other.count();
				}
				else
				{
					return static_cast<rep>(other.count() * cmoon::rational_ratio<t, typename common_t::rep>);
				}
			}
	};
	
	template<class CharT, class Traits, basic_unit_type First, basic_unit_type... Rest>
	void derived_unit_output_stream_helper(std::basic_ostream<CharT, Traits>& os)
	{
		output_unit_details<First>(os);

		if constexpr (sizeof...(Rest) > 0)
		{
			// Print the rest of the units, separated by a '*'
			((os << CharT('*'), output_unit_details<Rest>(os)), ...);
		}
	}

	export
	template<class CharT, class Traits, class Rep, class Ratio, basic_unit_type... Units>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_derived_unit<Rep, Ratio, Units...>& d_unit)
	{
		os << d_unit.count();

		if constexpr (T::system::template suffix_v<typename T::ratio, CharT> != cmoon::choose_str_literal<CharT>(STR_LITERALS("")))
		{
			os << T::system::template suffix_v<typename T::ratio, CharT>;
		}

		if constexpr (suffix_v<T, CharT> == cmoon::choose_str_literal<CharT>(STR_LITERALS("")))
		{
			derived_unit_output_stream_helper<Units...>(os);
		}
		else
		{
			os << suffix_v<T, CharT>;
		}
		return os;
	}
}

namespace std
{
	export
	template<cmoon::measures::basic_derived_unit_type T>
	struct formatter<T> : public std::formatter<std::string>
	{
		private:
			using base = std::formatter<std::string>;
		public:
			template<class FormatContext>
			auto format(const T& val, FormatContext& ctx)
			{
				std::stringstream ss;
				ss.imbue(ctx.locale());
				ss << val;
				return base::format(ss.str(), ctx);
			}
	};
}