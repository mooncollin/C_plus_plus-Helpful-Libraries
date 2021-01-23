#pragma once

#include <type_traits>

#include "cmoon/measures/measure.hpp"
#include "cmoon/ratio.hpp"
#include "cmoon/string.hpp"

namespace cmoon
{
	namespace measures
	{
		template<class Rep>
		struct electric_current_values;

		template<class Rep, class System, cmoon::ratio_type Ratio = std::ratio<1>, dimension_type Dimension = 1>
		using electric_current = basic_unit<Rep, Ratio, electric_current_values<Rep>, System, Dimension>;

		namespace details
		{
			template<class Rep, class System, cmoon::ratio_type Ratio, dimension_type Dimension>
			std::true_type is_electric_current_base_impl(const electric_current<Rep, System, Ratio, Dimension>&);
			std::false_type is_electric_current_base_impl(...);

			template<class U>
			constexpr auto is_based_in_electric_current = decltype(is_electric_current_base_impl(std::declval<U>()))::value;
		}

		template<class T>
		struct is_electric_current : std::bool_constant<details::is_based_in_electric_current<T>> {};

		template<class T>
		constexpr bool is_electric_current_v = is_electric_current<T>::value;

		template<class T>
		concept electric_current_type = is_electric_current_v<T>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_attoampere = electric_current<Rep, metric_system, std::atto, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_femtoampere = electric_current<Rep, metric_system, std::femto, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_picoampere = electric_current<Rep, metric_system, std::pico, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_nanoampere = electric_current<Rep, metric_system, std::nano, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_microampere = electric_current<Rep, metric_system, std::micro, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_milliampere = electric_current<Rep, metric_system, std::milli, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_centiampere = electric_current<Rep, metric_system, std::centi, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_deciampere = electric_current<Rep, metric_system, std::deci, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_decaampere = electric_current<Rep, metric_system, std::deca, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_hectoampere = electric_current<Rep, metric_system, std::hecto, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_kiloampere = electric_current<Rep, metric_system, std::kilo, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_megaampere = electric_current<Rep, metric_system, std::mega, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_gigaampere = electric_current<Rep, metric_system, std::giga, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_teraampere = electric_current<Rep, metric_system, std::tera, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_exaampere = electric_current<Rep, metric_system, std::exa, Dimension>;

		template<class Rep, dimension_type Dimension = 1>
		using basic_ampere = electric_current<Rep, metric_system, std::ratio<1>, Dimension>;

		using attoampere = basic_attoampere<std::intmax_t>;
		using femtoampere = basic_femtoampere<std::intmax_t>;
		using picoampere = basic_picoampere<std::intmax_t>;
		using nanoampere = basic_nanoampere<std::intmax_t>;
		using microampere = basic_microampere<std::intmax_t>;
		using milliampere = basic_milliampere<std::intmax_t>;
		using centiampere = basic_centiampere<std::intmax_t>;
		using deciampere = basic_deciampere<std::intmax_t>;
		using decaampere = basic_decaampere<std::intmax_t>;
		using hectoampere = basic_hectoampere<std::intmax_t>;
		using kiloampere = basic_kiloampere<std::intmax_t>;
		using megaampere = basic_megaampere<std::intmax_t>;
		using gigaampere = basic_gigaampere<std::intmax_t>;
		using teraampere = basic_teraampere<std::intmax_t>;
		using exaampere = basic_exaampere<std::intmax_t>;
		using ampere = basic_ampere<std::intmax_t>;

		template<class Rep>
		struct electric_current_values
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

		template<class Rep, cmoon::ratio_type Ratio, dimension_type Dimension, class CharT>
		struct suffix<electric_current<Rep, metric_system, Ratio, Dimension>, CharT>
		{
			static constexpr std::basic_string_view<CharT> value{cmoon::choose_str_literal<CharT>(STR_LITERALS("A"))};
		};
	}
}