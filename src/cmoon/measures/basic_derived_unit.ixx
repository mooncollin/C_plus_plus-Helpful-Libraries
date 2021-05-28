module;

#include "cmoon/string/literals.hpp"

export module cmoon.measures.basic_derived_unit;

import <tuple>;
import <concepts>;
import <ratio>;
import <type_traits>;
import <iostream>;
import <format>;
import <string>;

import cmoon.ratio;
import cmoon.math;
import cmoon.string;
import cmoon.meta;

import cmoon.measures.is_basic_derived_unit;
import cmoon.measures.basic_unit;

namespace cmoon::measures
{
	export
	template<basic_derived_unit_type ToDerivedUnit, class Rep, cmoon::ratio_type Ratio, basic_unit_type... Units>
		requires(std::same_as<typename ToDerivedUnit::units, std::tuple<Units...>>)
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
	template<class Rep, cmoon::ratio_type Ratio, basic_unit_type... Units>
		requires(cmoon::is_unique_v<typename Units::unit_values...> &&
					sizeof...(Units) >= 1
				)
	class basic_derived_unit
	{
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
			using units = std::tuple<Units...>;
			using numerator_units = cmoon::tuples::filter_t<is_numerator_unit, units>;
			using denominator_units = cmoon::tuples::filter_t<is_denominator_unit, units>;
			static constexpr auto num_units = sizeof...(Units);
		private:
			using driver_unit = std::tuple_element_t<0, units>;
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

			template<cmoon::ratio_type Ratio2, basic_unit_type... Units2>
                requires(same_unit_values<Units2...>&& same_systems<Units2...> && same_dimensions<Units2...>)
            constexpr basic_derived_unit(const basic_derived_unit<rep, Ratio2, Units2...>& other)
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

			template<class Rep2, cmoon::ratio_type Ratio2, class UnitValues, class System, dimension_type Dimension2>
			[[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>& rhs) noexcept
			{
				using unit_t = basic_unit<Rep2, Ratio2, UnitValues, System, Dimension2>;
				using result_t = derived_result_one_t<unit_t, multiply_op>;
				using common_t = std::common_type_t<rep, Rep2>;
				const auto amount = static_cast<common_t>(lhs.count()) * static_cast<common_t>(rhs.count()) * cmoon::rational_ratio<result_ratio_t<unit_t>, common_t>;

				return result_t{static_cast<common_t>(amount)};
			}

			template<cmoon::ratio_type Ratio2, class UnitValues, class System, dimension_type Dimension2>
			[[nodiscard]] constexpr friend auto operator*(const basic_unit<rep, Ratio2, UnitValues, System, Dimension2>& lhs, const basic_derived_unit& rhs) noexcept
			{
				return rhs * lhs;
			}

			template<class Rep2, cmoon::ratio_type Ratio2, basic_unit_type... Units2>
			[[nodiscard]] constexpr friend auto operator*(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Ratio2, Units2...>& rhs) noexcept
			{
				using result_t = derived_result_many_t<multiply_op, Units2...>;
				using common_t = std::common_type_t<basic_derived_unit, basic_derived_unit<Rep2, Ratio2, Units...>>;
				using ratio_t = typename std::ratio_multiply<typename common_t::ratio, result_ratio_t<Units2...>>::type;

				const auto amount = static_cast<typename common_t::rep>(lhs.count()) * static_cast<typename common_t::rep>(rhs.count()) * cmoon::rational_ratio<ratio_t, typename common_t::rep>;

				return result_t{static_cast<typename common_t::rep>(amount)};
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

			template<class Rep2, cmoon::ratio_type Ratio2, basic_unit_type... Units2>
			[[nodiscard]] constexpr friend auto operator/(const basic_derived_unit& lhs, const basic_derived_unit<Rep2, Ratio2, Units2...>& rhs) noexcept
			{
				using result_t = derived_result_many_t<divide_op, Units2...>;
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

			template<cmoon::ratio_type Ratio2, basic_unit_type... Units2>
            [[nodiscard]] friend constexpr bool operator==(const basic_derived_unit& lhs, const basic_derived_unit<rep, Ratio2, Units2...>& rhs) noexcept
            {
				return lhs == basic_derived_unit{rhs};
            }

			template<cmoon::ratio_type Ratio2, basic_unit_type... Units2>
            [[nodiscard]] friend constexpr bool operator!=(const basic_derived_unit& lhs, const basic_derived_unit<rep, Ratio2, Units2...>& rhs) noexcept
            {
                return !(lhs == rhs);
            }

			template<cmoon::ratio_type Ratio2, basic_unit_type... Units2>
			[[nodiscard]] friend constexpr std::strong_ordering operator<=>(const basic_derived_unit& lhs, const basic_derived_unit<rep, Ratio2, Units2...>& rhs) noexcept
			{
				return lhs <=> basic_derived_unit{rhs};
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

			// Using this only for type deduction
			template<class Rep2, cmoon::ratio_type Ratio2, basic_unit_type... Units2>
			basic_derived_unit<Rep2, Ratio2, Units2...> static get_tuple_derived_t(std::tuple<Units2...>);
				
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

					// If we already have this new unit as our set of units,
					// then produce the resulting tuple by traversing through
					// our list of units, returning a tuple of zero or one types
					// depending on the result of the operation on the two types.
					// Otherwise, append the new type to the end.
					using full_tuple_type = std::conditional_t<contains_unit_v<NewUnit>,
												decltype(std::tuple_cat(std::declval<result_impl_t<Units>>()...)),
												std::tuple<Units..., NewUnit>>;

					template<class EndingTuple>
					struct end_type_picker
					{
						using type = decltype(get_tuple_derived_t<typename NewUnit::rep, Ratio>(
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
	void derived_unit_output_stream_helper(std::basic_ostream<CharT, Traits>& os, const cmoon::meta::type_list<First, Rest...>)
	{
		output_unit_details<First>(os);

		if constexpr (sizeof...(Rest) > 0)
		{
			// Print the rest of the units, separated by a '*'
			((os << CharT('*'), output_unit_details<Rest>(os)), ...);
		}
	}

	export
	template<class CharT, class Traits, basic_derived_unit_type T>
	std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const T& d_unit)
	{
		os << d_unit.count();

		if constexpr (T::system::template suffix_v<typename T::ratio, CharT> != cmoon::choose_str_literal<CharT>(STR_LITERALS("")))
		{
			os << T::system::template suffix_v<typename T::ratio, CharT>;
		}

		if constexpr (suffix_v<T, CharT> == cmoon::choose_str_literal<CharT>(STR_LITERALS("")))
		{
			derived_unit_output_stream_helper(os, cmoon::meta::make_type_list<typename d_unit::units>(cmoon::meta::extract_tuple_types));
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